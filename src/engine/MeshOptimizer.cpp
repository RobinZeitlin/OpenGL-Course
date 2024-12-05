#include "MeshOptimizer.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <format>
#include <sstream>
#include <fstream>

#include "../rendering/Mesh.h"

MeshOptimizer::MeshOptimizer() { }

MeshOptimizer::~MeshOptimizer() { }

void MeshOptimizer::optimize_mesh(Mesh* mesh) {
	std::string meshName = mesh->meshName;
	std::string fullFilePath = filePath + meshName + "Optimized.obj";

	std::ofstream newFile(fullFilePath.c_str());

	if (!newFile.is_open()) {
		std::cout << "Failed to open file." << std::endl;
		return;
	}
	else
	{
		std::cout << "File opened correctly." << std::endl;
	}

	//write vertex data.

	for (size_t i = 0; i < mesh->vertexCount / 4; i++) {
		float x = mesh->meshVertices[i * 5 + 0];
		float y = mesh->meshVertices[i * 5 + 1];
		float z = mesh->meshVertices[i * 5 + 2];
		float u = mesh->meshVertices[i * 5 + 3];
		float v = mesh->meshVertices[i * 5 + 4];

		newFile << "v "
			<< x << " "
			<< y << " "
			<< z << "\n";

		newFile << "vt "
			<< u << " "
			<< v << "\n";
	}
	
	newFile.close();
}
