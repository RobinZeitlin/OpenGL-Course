#include "MeshOptimizer.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

#include "../rendering/Mesh.h"

MeshOptimizer::MeshOptimizer() { }

MeshOptimizer::~MeshOptimizer() { }

void MeshOptimizer::optimize_mesh(Mesh* mesh) {
    std::string meshName = mesh->meshName;
    std::string fullFilePath = filePath + meshName + "Optimized.obj";

    std::ofstream newFile(fullFilePath.c_str());

    if (!newFile.is_open()) {
        std::cerr << "Failed to open file: " << fullFilePath << std::endl;
        return;
    }
    else {
        std::cout << "File opened correctly: " << fullFilePath << std::endl;
    }

    for (size_t i = 0; i < mesh->vertexCount / 5; ++i) {
        float x = mesh->meshVertices[i * 5 + 0];
        float y = mesh->meshVertices[i * 5 + 1];
        float z = mesh->meshVertices[i * 5 + 2];
        float u = mesh->meshVertices[i * 5 + 3];
        float v = mesh->meshVertices[i * 5 + 4];

        newFile << "v " << x << " " << y << " " << z << "\n";

        newFile << "vt " << u << " " << v << "\n";
    }

    for (size_t i = 0; i < mesh->indexCount / 3; ++i) {
        unsigned int idx1 = mesh->meshIndices[i * 3 + 0] + 1;
        unsigned int idx2 = mesh->meshIndices[i * 3 + 1] + 1;
        unsigned int idx3 = mesh->meshIndices[i * 3 + 2] + 1;

        newFile << "f " << idx1 << " " << idx2 << " " << idx3 << "\n";
    }

    newFile.close();
    std::cout << "Optimized mesh written to: " << fullFilePath << std::endl;
}
