#include "OBJLoader.h"

#include "../rendering/Mesh.h"
#include "../engine/memory/MemoryStatus.h"
#include "../engine/MeshOptimizer.h"
#include "../rendering/Vertex.h"

#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>

OBJLoader::OBJLoader() : messageMutex(), filesMutex(), bLoadingThreadIsRunning(false)
{
}

OBJLoader::~OBJLoader() {
    clean_up();
}

void OBJLoader::clean_up() {
    bLoadingThreadIsRunning = false;

    if (bLoadingThreadIsRunning && workThread.joinable()) {
        workThread.join();
    }

    for (auto& [key, file] : files) {
        delete file;
    }
    files.clear();
}

void OBJLoader::start_worker_thread() {
    if (!bLoadingThreadIsRunning) {
        bLoadingThreadIsRunning = true;
        workThread = std::thread(&OBJLoader::worker, this);
    }
}

void OBJLoader::worker() {
    while (true) {
        MeshMessage* message = nullptr;

        {
            std::unique_lock<std::mutex> lock(messageMutex);
            workerCondition.wait(lock, [this] {
                return !messages.empty() || !bLoadingThreadIsRunning;
                });

            if (!bLoadingThreadIsRunning && messages.empty()) {
                return;
            }

            message = messages.front();
            messages.pop();
        }

        if (message) {
            std::string meshName = message->get_mesh_name();
            delete message;

            MeshData* newMeshData = load_mesh(meshName);
            if (newMeshData) {
                {
                    std::lock_guard<std::mutex> lock(filesMutex);
                    MeshToLoad* newMeshToLoad = new MeshToLoad(meshName, newMeshData);
                    meshDataToLoad.push(newMeshToLoad);
                }
            }

            {
                std::lock_guard<std::mutex> lock(messageMutex);
                messageSet.erase(meshName);
            }
        }
    }
}

void OBJLoader::recieve_message(MeshMessage* newMessage) {

    std::string meshName = newMessage->get_mesh_name();

    {
        std::lock_guard<std::mutex> lock(messageMutex);

        if (messageSet.find(meshName) == messageSet.end()) {
            messages.push(newMessage);
            messageSet.insert(meshName);
            std::cout << "Message received: " << meshName << std::endl;
        }
        else {
            delete newMessage;
        }
    }

    workerCondition.notify_one();

    if (!bLoadingThreadIsRunning)
        start_worker_thread();
}

Mesh* OBJLoader::get_mesh(std::string objName) {
    std::lock_guard<std::mutex> lock(meshLoadingMutex);

    if (files[objName]) {
        std::cout << "Mesh finals vertexcount = " << files[objName]->vertexCount / 8 << std::endl;
        std::cout << "Mesh finals indexcount = " << files[objName]->indexCount / 3 << std::endl;
        return files[objName];
    }

    recieve_message(new MeshMessage(objName));

    while (!meshDataToLoad.empty()) {
        MeshToLoad* newMeshToLoad = meshDataToLoad.front();
        MeshData* newMeshData = newMeshToLoad->meshData;

        Mesh* newMesh = new Mesh(
            (float*)newMeshData->vertexData.data(),
            newMeshData->vertexData.size() * sizeof(Vertex),
            (unsigned int*)newMeshData->indices.data(),
            newMeshData->indices.size() * sizeof(VertexIndex)
        );

        std::cout << "Mesh created" << std::endl;

        files[newMeshToLoad->meshName] = newMesh;

        meshDataToLoad.pop();

        delete newMeshData;
        delete newMeshToLoad;
    }

    return nullptr;
}

MeshData* OBJLoader::load_mesh(const std::string objName) {
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
        }

        if (prefix == "vt") {
            glm::vec2 textureCoordinate;
            iss >> textureCoordinate.x >> textureCoordinate.y;
            uvs.push_back(textureCoordinate);
        }

        if (prefix == "vn") {
            glm::vec3 normalCoordinates;
            iss >> normalCoordinates.x >> normalCoordinates.y >> normalCoordinates.z;
            normals.push_back(normalCoordinates);
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
            }
        }
    }

    file.close();

    std::vector<Vertex> vertexData;
    std::vector<VertexIndex> indices;

    // insert the data into the vertexData and indices vectors
    for (int i = 0; i < positionIndices.size(); i++) {
        int pos_i = positionIndices[i];
        int uv_i = uvIndices[i];
        int normal_i = normalIndices[i];

        glm::vec3 pos = positions[pos_i];
        glm::vec2 uv = uvs[uv_i];
        glm::vec3 normal = normals[normal_i];
        
        Vertex v = {
            pos.x, pos.y, pos.z,
            0, 0,
            0, 0, 0,
        };

        vertexData.push_back(v);

        VertexIndex vi = { 
            (unsigned int)pos_i, 
            (unsigned int)uv_i, 
            (unsigned int)normal_i 
        };

        indices.push_back(vi);
    }

    std::cout << "Vertex Data:\n";
    for (size_t i = 0; i < vertexData.size(); ++i) {
        const auto& v = vertexData[i];
        std::cout << "Vertex " << i << ": "
            << "Position(" << v.x << ", " << v.y << ", " << v.z << "), "
            << "UV(" << v.u << ", " << v.v << "), "
            << "Normal(" << v.nx << ", " << v.ny << ", " << v.nz << ")\n";
    }

    std::cout << "VertexData size : " << vertexData.size() << std::endl;

    std::cout << "\nIndices:\n";
    for (size_t i = 0; i < indices.size(); ++i) {
        const auto& idx = indices[i];
        std::cout << "Index " << i << ": "
            << "Position Index = " << idx.p << ", "
            << "UV Index = " << idx.t << ", "
            << "Normal Index = " << idx.n << "\n";
    }

    std::cout << "indice size : " << indices.size() / 3 << std::endl;

    MeshData* newMeshData = new MeshData();
    newMeshData->indices = indices;
    newMeshData->vertexData = vertexData;

    return newMeshData;
}