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
    //std::vector<float> textureCoordinates;
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

            vertices.push_back(0.f);
            vertices.push_back(0.f);

            std::cout << "v = "
                "X : " << vertexPoint.x <<
                " Y : " << vertexPoint.y <<
                " Z : " << vertexPoint.z <<
                std::endl;
        }

        /*if (prefix == "vt") {
            glm::vec2 textureCoordinate;

            iss >> textureCoordinate.x >> textureCoordinate.y;

            textureCoordinates.push_back(textureCoordinate.x);
            textureCoordinates.push_back(textureCoordinate.y);

            std::cout << "vt = " << "X : " << textureCoordinate.x << " Y : " << textureCoordinate.y << std::endl;
        }*/

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

    /*std::vector<float> vertexData;
    for (size_t i = 0; i < vertices.size() / 3; ++i) {
        vertexData.push_back(vertices[i * 3]);
        vertexData.push_back(vertices[i * 3 + 1]);
        vertexData.push_back(vertices[i * 3 + 2]);

        vertexData.push_back(textureCoordinates[i * 2]);
        vertexData.push_back(textureCoordinates[i * 2 + 1]);

        std::cout << "v = " << "X : " << vertices[i * 3] << " Y : " << vertices[i * 3 + 1] << " Z : " << vertices[i * 3 + 2] << " vt : u : " << textureCoordinates[i * 2] << " vt : v : " << textureCoordinates[i * 2 + 1] << std::endl;
    }*/

    Mesh* newMesh = new Mesh(vertices.data(), vertices.size() * sizeof(float), indices.data(), indices.size() * sizeof(unsigned int));
    return newMesh;
}