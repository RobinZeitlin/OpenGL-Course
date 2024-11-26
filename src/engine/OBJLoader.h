#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <unordered_map>

#include <fstream>
#include <sstream>

#include <glm.hpp>

class Mesh;

class OBJLoader {
public:
	OBJLoader();
	~OBJLoader();

	void init();

	Mesh* getMesh(std::string objName) {
		if (files[objName])
		{
			return files[objName];
		}

		std::cout << "no mesh found" << std::endl;

		return nullptr;
	}

	Mesh* loadMesh(std::string objPath);

	std::unordered_map<std::string, Mesh*> files;
};