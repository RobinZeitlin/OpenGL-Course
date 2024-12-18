#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <format>
#include <unordered_map>
#include <chrono>

#include <fstream>
#include <sstream>

#include <glm.hpp>

#include <thread>
#include <future>

#include "MeshInfo.h"

class Mesh;

class OBJLoader {
public:
	static OBJLoader& get_instance() {
		static OBJLoader instance;
		return instance;
	}

	~OBJLoader();

	void clean_up();

	void create_thread(std::string objToLoad);

	Mesh* get_mesh(std::string objName);

	meshInfo* load_mesh(std::string objPath);

	std::unordered_map<std::string, Mesh*> files;
	std::string filePath = "res/objmodels/";
	std::string fileFormat = ".obj";

private:
	std::thread workThread;
};