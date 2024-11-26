#include "OBJLoader.h"

#include "../rendering/Mesh.h"

OBJLoader::OBJLoader()
{
}

OBJLoader::~OBJLoader() {

}

void OBJLoader::init() {
    std::string filePath = "res/objmodels/";
    std::filesystem::path folderPath = std::filesystem::current_path() / filePath;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string objName = entry.path().stem().string();
            auto mesh = loadMesh(filePath + objName);
            files[objName] = mesh;
            if (mesh != nullptr) {
                std::cout << "Loaded : " + objName << std::endl;
            }
        }
    }

    std::cout << "Loaded files" << std::endl;
}

Mesh* OBJLoader::loadMesh(const std::string objPath) {
    std::cout << objPath << std::endl;

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
            vertices.push_back(vertexPoint.z);

            std::cout << "v = "
                "X : " << vertexPoint.x << 
                " Y : " << vertexPoint.y << 
                " Z : " << vertexPoint.z << 
                std::endl;
        }

        if (prefix == "vt") {
            glm::vec2 textureCoordinate;

            iss >> textureCoordinate.x >> textureCoordinate.y;

            textureCoordinates.push_back(textureCoordinate.x);
            textureCoordinates.push_back(textureCoordinate.y);

            std::cout << "vt = " << "X : " << textureCoordinate.x << " Y : " << textureCoordinate.y << std::endl;
        }

        if (prefix == "f") {
        }
    }

    file.close();

    std::vector<float> vertexData;
    for (size_t i = 0; i < indices.size(); ++i) {
        unsigned int idx = indices[i];
        vertexData.push_back(vertices[idx * 3]);
        vertexData.push_back(vertices[idx * 3 + 1]);
        vertexData.push_back(vertices[idx * 3 + 2]);

        vertexData.push_back(textureCoordinates[idx * 2]);
        vertexData.push_back(textureCoordinates[idx * 2 + 1]);
    }

    Mesh* newMesh = new Mesh(vertexData.data(), vertexData.size() * sizeof(float), indices.data(), indices.size());
    return newMesh;
}