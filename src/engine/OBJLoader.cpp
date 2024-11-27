#include "OBJLoader.h"

#include "../rendering/Mesh.h"

OBJLoader::OBJLoader()
{
}

OBJLoader::~OBJLoader() {

}

bool OBJLoader::init() {
    std::string filePath = "res/objmodels/";
    std::filesystem::path folderPath = std::filesystem::current_path() / filePath;
    int i = 1;
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string objName = entry.path().stem().string();
            auto mesh = loadMesh(filePath + objName);
            files[objName] = mesh;
            if (mesh != nullptr) {
                std::cout << std::format("Model \"{}\" ({} / {}) Loaded", objName, i, files.size()) << std::endl;
            }
            i++;
        }
    }

    return true;
}

Mesh* OBJLoader::loadMesh(const std::string objPath) {
    std::ifstream file(objPath + ".obj");
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
    return newMesh;
}