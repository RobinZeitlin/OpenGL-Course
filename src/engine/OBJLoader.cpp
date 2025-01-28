#include "OBJLoader.h"

#include "../rendering/Mesh.h"
#include "../engine/memory/MemoryStatus.h"
#include "../engine/MeshOptimizer.h"
#include "../rendering/Vertex.h"

#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>

OBJLoader::~OBJLoader() {
    clean_up();
}

void OBJLoader::clean_up() {
    if (bLoadingThreadIsRunning && workThread.joinable()) {
        workThread.join();
    }

    for (auto& [key, file] : files) {
        delete file;
    }
    files.clear();
}

void OBJLoader::create_thread() {
    if (workThread.joinable()) {
        workThread.join();
    }

    while (!messages.empty()) {
        std::cout << "Messages exist. Processing..." << std::endl;

        std::string objToLoad = messages.front()->get_mesh_name();
        std::promise<MeshInfo*> promise;
        std::future<MeshInfo*> future = promise.get_future();

        workThread = std::thread([this, objToLoad, promise = std::move(promise)]() mutable {
            //std::cout << "Loading mesh: " << objToLoad << std::endl;
            MeshInfo* meshData = load_mesh(objToLoad);
            if (meshData) {
                std::cout << "Mesh successfully loaded: " << objToLoad << std::endl;
            }
            else {
                std::cerr << "Failed to load mesh: " << objToLoad << std::endl;
            }
            promise.set_value(meshData);
        });

        workThread.detach();

        MeshInfo* result = future.get();

        if (result) {
            std::cout << "byte size : " << result->vertexData.size() * sizeof(Vertex) << std::endl;

            std::cout << "Indices (1-based): ";
            for (size_t i = 0; i < result->indices.size(); i++) {
                std::cout << result->indices[i].p << "/"
                    << result->indices[i].n << "/"
                    << result->indices[i].t << ", ";
            }
            std::cout << std::endl;

            Mesh* newMesh = new Mesh(
                (float*)result->vertexData.data(), 
                result->vertexData.size() * sizeof(Vertex), 
                (unsigned int*)result->indices.data(),
                result->indices.size() * sizeof(VertexIndex));

            newMesh->meshName = objToLoad;
            files[objToLoad] = newMesh;

            std::cout << "Mesh stored: " << objToLoad << std::endl;
            messages.pop();
            delete result;
        }
        else {
            std::cerr << "Failed to load mesh data for: " << objToLoad << std::endl;
        }
    }

    bLoadingThreadIsRunning = false;
}

void OBJLoader::recieve_message(MeshMessage* newMessage) {
    std::cout << "Received message to load mesh: " << newMessage->get_mesh_name() << std::endl;
    messages.push(newMessage);

    if (!bLoadingThreadIsRunning) {
        bLoadingThreadIsRunning = true;
        create_thread();
    }
}

Mesh* OBJLoader::get_mesh(std::string objName) {
    if (files[objName]) {
        std::cout << "Returning loaded mesh: " << objName << std::endl;
        return files[objName];
    }

    std::cout << "Mesh not found. Queuing load for: " << objName << std::endl;
    auto* meshMessage = new MeshMessage(objName);
    recieve_message(meshMessage);

    return nullptr;
}

MeshInfo* OBJLoader::load_mesh(const std::string objName) {
    float requiredMemory = 500.0f;

    auto [availablePhysical, availableVirtual] = MemoryStatus::get_instance().get_available_memory();

    if (availablePhysical < requiredMemory) {
        throw std::runtime_error("Insufficient physical memory to load the OBJ file.");
    }

    std::string objPath = filePath + objName;

    std::ifstream file(objPath + fileFormat);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << objPath << std::endl;
        return nullptr;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::vector<unsigned int> positionIndices;
    std::vector<unsigned int> uvIndices;
    std::vector<unsigned int> normalIndices;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 vertexPoint;
            iss >> vertexPoint.x >> vertexPoint.y >> vertexPoint.z;
            positions.push_back(vertexPoint);
            std::cout << "Vertex position: " << vertexPoint.x << ", " << vertexPoint.y << ", " << vertexPoint.z << std::endl;
        }

        if (prefix == "vt") {
            glm::vec2 textureCoordinate;
            iss >> textureCoordinate.x >> textureCoordinate.y;
            uvs.push_back(textureCoordinate);
            std::cout << "Texture coordinate: " << textureCoordinate.x << ", " << textureCoordinate.y << std::endl;
        }

        if (prefix == "vn") {
            glm::vec3 normalCoordinates;
            iss >> normalCoordinates.x >> normalCoordinates.y >> normalCoordinates.z;
            normals.push_back(normalCoordinates);
            std::cout << "Normal: " << normalCoordinates.x << ", " << normalCoordinates.y << ", " << normalCoordinates.z << std::endl;
        }

        if (prefix == "f") {
            for (int i = 0; i < 3; i++) {
                std::string vert_word_str;
                iss >> vert_word_str;
                if (vert_word_str.size() == 0) break;

                std::istringstream vert_word(vert_word_str);
                int position_index;
                int uv_index;
                int normal_index;

                vert_word >> position_index;
                vert_word.ignore(1);
                vert_word >> uv_index;
                vert_word.ignore(1);
                vert_word >> normal_index;
                vert_word.ignore(1);

                positionIndices.push_back(position_index - 1);
                uvIndices.push_back(uv_index - 1);
                normalIndices.push_back(normal_index - 1);

                // Debugging the face information
                std::cout << "Face index: Position=" << position_index
                    << ", UV=" << uv_index
                    << ", Normal=" << normal_index << std::endl;
            }
        }
    }

    file.close();

    std::vector<Vertex> vertexData;
    std::vector<VertexIndex> indices;

    for (int i = 0; i < positionIndices.size(); i++) {
        int pos_i = positionIndices[i];
        int uv_i = uvIndices[i];
        int normal_i = normalIndices[i];

        glm::vec3 pos;
        glm::vec2 uv;
        glm::vec3 normal;

        if (pos_i < 0 || pos_i >= positions.size()) {
            pos = glm::vec3(-1.0f);
        }
        else {
			pos = positions[pos_i];
		}

        if (uv_i < 0 || uv_i >= uvs.size()) {
            uv = glm::vec2(-1.0f);
        }
        else {
            uv = uvs[uv_i];
        }

        if (normal_i < 0 || normal_i >= normals.size()) {
            normal = glm::vec3(-1.0f);
        }
        else {
            normal = normals[normal_i];
        }

        Vertex v = {
            pos.x, pos.y, pos.z,
            uv.x, uv.y,
            normal.x, normal.y, normal.z,
        };

        vertexData.push_back(v);

        VertexIndex vi = { 
            (unsigned int)pos_i, 
            (unsigned int)uv_i, 
            (unsigned int)normal_i 
        };

        indices.push_back(vi);


        // Debugging the created vertex
        std::cout << "Vertex created: Position=(" << pos.x << ", " << pos.y << ", " << pos.z
            << "), UV=(" << uv.x << ", " << uv.y
            << "), Normal=(" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;
    }

    MeshInfo* newMeshData = new MeshInfo();
    newMeshData->indices = indices;
    newMeshData->vertexData = vertexData;

    std::cout << "Finished Loading with vertex count " << positions.size() << std::endl;

    return newMeshData;
}
