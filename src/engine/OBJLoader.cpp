#include "OBJLoader.h"

#include "../rendering/Mesh.h"

#include "../engine/memory/MemoryStatus.h"
#include "../engine/MeshOptimizer.h"

OBJLoader::~OBJLoader() {
    clean_up();
}

void OBJLoader::clean_up() {
    for (auto& [key, file] : files)
    {
        delete file;
    }
    files.clear();
}

Mesh* OBJLoader::load_mesh(const std::string objName) {
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
            vertices.push_back(vertexPoint.z); }

        if (prefix == "vt") {
            glm::vec2 textureCoordinate;

            iss >> textureCoordinate.x >> textureCoordinate.y;

            textureCoordinates.push_back(textureCoordinate.x);
            textureCoordinates.push_back(textureCoordinate.y);}

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

    return newMesh;
}