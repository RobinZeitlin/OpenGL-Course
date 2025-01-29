#include "OBJLoader.h"

#include "../rendering/Mesh.h"

#include "../engine/memory/MemoryStatus.h"
#include "../engine/MeshOptimizer.h"

#include <unordered_map>

OBJLoader::OBJLoader()
{
}

OBJLoader::~OBJLoader() {
    clean_up();
}

void OBJLoader::clean_up() {
    if (bLoadingThreadIsRunning && workThread.joinable()) {
        workThread.join();
    }

    for (auto& [key, file] : files)
    {
        delete file;
    }
    files.clear();
}

void OBJLoader::create_thread() {
    if (workThread.joinable()) {
        workThread.join();
    }

    while (!messages.empty()) {
        std::cout << "messages exist!" << std::endl;

        std::string objToLoad = messages.front()->get_mesh_name();
        std::promise<MeshData*> promise;
        std::future<MeshData*> future = promise.get_future();

        workThread = std::thread([this, objToLoad, promise = std::move(promise)]() mutable {
            MeshData* meshData = load_mesh(objToLoad);
            promise.set_value(meshData);
            });

        workThread.detach();

        MeshData* result = future.get();

        if (result) {
            Mesh* newMesh = new Mesh((float*)result->vertexData.data(), result->vertexData.size() * sizeof(Vertex), result->indices.data(), result->indices.size() * sizeof(unsigned int));
            newMesh->meshName = objToLoad;
            files[objToLoad] = newMesh;

            std::cout << "mesh Loaded : " << objToLoad << std::endl;
            messages.pop();
            delete result;
        }
        else {
            std::cerr << "Failed to load mesh: " << objToLoad << std::endl;
        }
    }

    bLoadingThreadIsRunning = false;
}

void OBJLoader::recieve_message(MeshMessage* newMessage) {
    messages.push(newMessage);

    if (!bLoadingThreadIsRunning) {
        bLoadingThreadIsRunning = true;
        create_thread();
    }
}

Mesh* OBJLoader::get_mesh(std::string objName) {
    if (files[objName]) {
        std::cout << "Returning OBJ" << std::endl;
        return files[objName];
    }

    auto* meshMessage = new MeshMessage(objName);
    recieve_message(meshMessage);

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
    std::vector<unsigned int> indices;

    struct VertexIndices {
        uint16_t a, b, c, d;
    };
    std::unordered_map<int64_t, int> map;

    for (int i = 0; i < positionIndices.size(); i++) {
        int pos_i = positionIndices[i];
        int uv_i = uvIndices[i];
        int normal_i = normalIndices[i];

        VertexIndices ii = {
            (uint16_t)pos_i,
            (uint16_t)uv_i,
            (uint16_t)normal_i,
            0,
        };
        uint64_t ii_as_u64 = *(uint64_t*)&ii;

        //std::string ii = std::to_string(pos_i) + ", " + std::to_string(uv_i) + ", " + std::to_string(normal_i);

        glm::vec3 pos = positions[pos_i];
        glm::vec2 uv = uvs[uv_i];
        glm::vec3 normal = normals[normal_i];
        Vertex v = {
            pos.x, pos.y, pos.z,
            uv.x, uv.y,
            normal.x, normal.y, normal.z,
        };

        auto found = map.find(ii_as_u64);
        if (found == map.end()) {
            map[ii_as_u64] = vertexData.size();
            indices.push_back(vertexData.size());
            vertexData.push_back(v);
        }
        else {
            indices.push_back(found->second);
        }
    }

    MeshData* newMeshData = new MeshData();
    newMeshData->indices = indices;
    newMeshData->vertexData = vertexData;
    //newMeshData->vertexData = testVertexData;
    //newMeshData->indices = testIndices;

    std::cout << "Finished Loading with vertice count " << positions.size() << std::endl;

    return newMeshData;
}
