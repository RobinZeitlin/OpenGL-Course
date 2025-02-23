#include "MeshOptimizer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "../rendering/Mesh.h"
#include "../rendering/Vertex.h"

MeshOptimizer::MeshOptimizer() {}

MeshOptimizer::~MeshOptimizer() {}

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

    // To ensure unique vertices, we'll use a map (similar to OBJLoader)
    std::unordered_map<int64_t, int> vertexMap;
    std::vector<Vertex> optimizedVertices;
    std::vector<unsigned int> optimizedIndices;

    // Write vertices and texture coordinates
    for (size_t i = 0; i < mesh->vertexCount / 8; ++i) {  // Correcting to 8 for position, tex, and normals
        float x = mesh->meshVertices[i * 8 + 0];
        float y = mesh->meshVertices[i * 8 + 1];
        float z = mesh->meshVertices[i * 8 + 2];
        float u = mesh->meshVertices[i * 8 + 3];
        float v = mesh->meshVertices[i * 8 + 4];
        float nx = mesh->meshVertices[i * 8 + 5];
        float ny = mesh->meshVertices[i * 8 + 6];
        float nz = mesh->meshVertices[i * 8 + 7];

        // Check if the vertex already exists
        glm::vec3 pos(x, y, z);
        glm::vec2 tex(u, v);
        glm::vec3 normal(nx, ny, nz);

        Vertex vtx = { pos.x, pos.y, pos.z, tex.x, tex.y, normal.x, normal.y, normal.z };

        // Generate a unique key for the vertex, including normals
        int64_t key = (static_cast<int64_t>(vtx.x) << 56) |
            (static_cast<int64_t>(vtx.y) << 48) |
            (static_cast<int64_t>(vtx.z) << 40) |
            (static_cast<int64_t>(vtx.u) << 32) |
            (static_cast<int64_t>(vtx.v) << 24) |
            (static_cast<int64_t>(vtx.nx) << 16) |
            (static_cast<int64_t>(vtx.ny) << 8) |
            static_cast<int64_t>(vtx.nz);

        auto found = vertexMap.find(key);
        if (found == vertexMap.end()) {
            // Add new vertex if not already present
            vertexMap[key] = optimizedVertices.size();
            optimizedVertices.push_back(vtx);
        }

        // After ensuring uniqueness, add the vertex index
        optimizedIndices.push_back(vertexMap[key]);
    }

    // Write vertex data in OBJ format
    for (const auto& vtx : optimizedVertices) {
        newFile << "v " << vtx.x << " " << vtx.y << " " << vtx.z << "\n";
        newFile << "vt " << vtx.u << " " << vtx.v << "\n";
        newFile << "vn " << vtx.nx << " " << vtx.ny << " " << vtx.nz << "\n";
    }

    // Write face data (indices)
    for (size_t i = 0; i < optimizedIndices.size() / 3; ++i) {
        unsigned int idx1 = optimizedIndices[i * 3 + 0] + 1;
        unsigned int idx2 = optimizedIndices[i * 3 + 1] + 1;
        unsigned int idx3 = optimizedIndices[i * 3 + 2] + 1;

        newFile << "f " << idx1 << " " << idx2 << " " << idx3 << "\n";
    }

    newFile.close();
    std::cout << "Optimized mesh written to: " << fullFilePath << std::endl;
}

