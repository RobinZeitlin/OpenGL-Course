#include "OBJLoader.h"

#include "../rendering/Mesh.h"

#include "../engine/memory/MemoryStatus.h"
#include "../engine/MeshOptimizer.h"

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
        std::promise<meshInfo*> promise;
        std::future<meshInfo*> future = promise.get_future();

        workThread = std::thread([this, objToLoad, promise = std::move(promise)]() mutable {
            meshInfo* meshData = load_mesh(objToLoad);
            promise.set_value(meshData); 
            });

        workThread.detach();

        meshInfo* result = future.get();

        if (result) {
            Mesh* newMesh = new Mesh(result->vertexData.data(), result->vertexData.size() * sizeof(float), result->indices.data(), result->indices.size() * sizeof(unsigned int));
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

meshInfo* OBJLoader::load_mesh(const std::string objName) {
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

    std::vector<float> vertices;
    std::vector<float> textureCoordinates;
    std::vector<unsigned int> indices;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            glm::vec3 vertexPoint;
            iss >> vertexPoint.x >> vertexPoint.y >> vertexPoint.z;
            vertices.push_back(vertexPoint.x);
            vertices.push_back(vertexPoint.y);
            vertices.push_back(vertexPoint.z);
        }

        if (prefix == "vt") {
            glm::vec2 textureCoordinate;
            iss >> textureCoordinate.x >> textureCoordinate.y;
            textureCoordinates.push_back(textureCoordinate.x);
            textureCoordinates.push_back(textureCoordinate.y);
        }

        if (prefix == "f") {
            for (int i = 0; i < 3; i++) {
                std::string vert_word_str;
                iss >> vert_word_str;
                if (vert_word_str.size() == 0) break;

                std::istringstream vert_word(vert_word_str);
                int vert_index;
                vert_word >> vert_index;
                indices.push_back(vert_index - 1);
            }
        }
    }

    file.close();

    std::vector<float> vertexData;
    for (size_t i = 0; i < vertices.size() / 3; ++i) {
        vertexData.push_back(vertices[i * 3]);
        vertexData.push_back(vertices[i * 3 + 1]);
        vertexData.push_back(vertices[i * 3 + 2]);

        vertexData.push_back(textureCoordinates[i * 2]);
        vertexData.push_back(textureCoordinates[i * 2 + 1]);
    }

    meshInfo* newMeshData = new meshInfo();
    newMeshData->indices = indices;
    newMeshData->vertexData = vertexData;

    std::cout << "Finished Loading with vertice count " << vertices.size() << std::endl;

    return newMeshData;
}
