#pragma once

#include <string>

class Mesh;

class MeshOptimizer {
public:
	static MeshOptimizer& get_instance() {
		static MeshOptimizer instance;
		return instance;
	}

	MeshOptimizer();
	~MeshOptimizer();

	void optimize_mesh(Mesh* mesh);

	std::string filePath = "res/objmodels/";
};