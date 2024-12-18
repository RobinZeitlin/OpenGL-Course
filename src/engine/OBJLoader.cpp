#include "OBJLoader.h"
#include "../rendering/Mesh.h"
#include "../engine/memory/MemoryStatus.h"
#include "../engine/MeshOptimizer.h"

OBJLoader::OBJLoader() {
    start_thread();
}

OBJLoader::~OBJLoader() {
    clean_up();
    stop_thread();
}

void OBJLoader::clean_up() {
    for (auto& [key, file] : files) {
        delete file;
    }
    files.clear();
}

Mesh* OBJLoader::get_mesh(std::string objName) {
    std::lock_guard<std::mutex> lock(filesMutex);

    if (files[objName]) {
        std::cout << "Returning " << objName << " with vertexCount: " << files[objName]->vertexCount << std::endl;
        return files[objName];
    }

    auto* meshMessage = new MeshMessage(objName);
    messageQueue.push(meshMessage);
    std::cout << "Pushing mesh message: " << objName << std::endl;

    return files[objName];
}


void OBJLoader::start_thread() {
    if (!running) {
        std::cout << "Starting worker thread." << std::endl;
        running = true;
        loadingThread = std::thread(&OBJLoader::worker_thread, this);
    }
}

void OBJLoader::stop_thread() {
    if (running) {
        running = false;
        loadingThread.join();
    }
}

void OBJLoader::worker_thread() {
    while (running) {
        Message* message = nullptr;

        {
            std::lock_guard<std::mutex> lock(filesMutex); // lock the mutex before checking the message queue to prevent multiple threads accessing
            if (!messageQueue.empty()) {
                message = messageQueue.front();
                messageQueue.pop();
            }
        }

        if (message) {
            process_message(message);
            delete message;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // sleepy eepy thread 
    }
}

void OBJLoader::process_message(Message* message) {
    MessageType messageType = message->type;
    std::cout << "Processing message of type: " << static_cast<int>(messageType) << std::endl;

    switch (messageType) {
    case MessageType::String:
        if (message->msg == "load_mesh") {
            if (auto* meshMessage = dynamic_cast<MeshMessage*>(message)) {
                std::string meshName = meshMessage->get_mesh_name();
                std::cout << "Requested mesh: " << meshName << std::endl;

                std::lock_guard<std::mutex> lock(filesMutex); // once again lock the mutex to prevent multiple threads accessing
                if (!files[meshName]) {
                    std::cout << "Loading mesh: " << meshName << std::endl;
                    files[meshName] = load_mesh(meshMessage->get_mesh_name());
                }
            }
        }
        break;

    default:
        std::cerr << "Unhandled message type: " << static_cast<int>(messageType) << std::endl;
        break;
    }
}


Mesh* OBJLoader::load_mesh(const std::string objName) {
    std::cout << "Loading mesh " << objName << std::endl;
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

    Mesh* newMesh = new Mesh(vertexData.data(), vertexData.size() * sizeof(float), indices.data(), indices.size() * sizeof(unsigned int));
    newMesh->meshName = objName;

    std::cout << "Loaded Model" << std::endl;

    return newMesh;
}
