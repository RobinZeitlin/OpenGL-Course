#include "MeshOptimizer.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "../rendering/Mesh.h"
#include "../rendering/Vertex.h"

MeshOptimizer::MeshOptimizer() {}
MeshOptimizer::~MeshOptimizer() {}

void MeshOptimizer::optimize_mesh(Mesh* mesh) {
    if (!mesh || mesh->meshVerticesList.empty()) {
        std::cout << "[Error] Mesh is null or contains no vertices." << std::endl;
        return;
    }

    std::string dirPath = "res/objmodels/";
    std::string filePath = dirPath + mesh->meshName + "_Optimized.obj";

    std::ifstream fileCheck(filePath);
    if (fileCheck.good()) {
        std::cout << "[Warning] File already exists: " << filePath << std::endl;
        return;
    }

    std::ofstream newFile(filePath);
    if (!newFile) {
        std::cout << "[Error] Failed to open file for writing: " << filePath << std::endl;
        return;
    }

    std::unordered_map<Vertex, unsigned int, VertexHash> vertexMap;
    std::vector<Vertex> optimizedVertices;
    std::vector<unsigned int> optimizedIndices;

    optimizedVertices.reserve(mesh->meshVerticesList.size());
    optimizedIndices.reserve(mesh->meshIndiceList.size());

    for (const auto& vtx : mesh->meshVerticesList) {
        if (std::isnan(vtx.x) || std::isnan(vtx.y) || std::isnan(vtx.z)) {
            std::cout << "[Warning] Skipping NaN vertex." << std::endl;
            continue;
        }

        auto [it, inserted] = vertexMap.emplace(vtx, static_cast<unsigned int>(optimizedVertices.size()));
        if (inserted) optimizedVertices.push_back(vtx);
        optimizedIndices.push_back(it->second + 1);
    }

    for (const auto& v : optimizedVertices) {
        newFile << "v " << v.x << " " << v.y << " " << v.z << "\n"
            << "vt " << v.u << " " << v.v << "\n"
            << "vn " << v.nx << " " << v.ny << " " << v.nz << "\n";
    }

    for (size_t i = 0; i + 2 < optimizedIndices.size(); i += 3) {
        newFile << "f " << optimizedIndices[i] << " "
            << optimizedIndices[i + 1] << " "
            << optimizedIndices[i + 2] << "\n";
    }

    std::cout << "[Success] Optimized mesh written to: " << filePath << std::endl;
}
