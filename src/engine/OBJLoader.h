#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <format>
#include <unordered_map>
#include <chrono>
#include <queue>

#include <fstream>
#include <sstream>

#include <glm.hpp>

#include <thread>
#include <future>

#include "MeshInfo.h"

#include "../engine/messages/MeshMessage.h"

class Mesh;

class OBJLoader {
public:
	static OBJLoader& get_instance() {
		static OBJLoader instance;
		return instance;
	}

	~OBJLoader();

	void clean_up();
	void create_thread();

	void recieve_message(MeshMessage* newMessage);

	Mesh* get_mesh(std::string objName);
	meshInfo* load_mesh(std::string objPath);

	std::queue<MeshMessage*> messages;

	std::unordered_map<std::string, Mesh*> files;
	std::string filePath = "res/objmodels/";
	std::string fileFormat = ".obj";

	bool bLoadingThreadIsRunning = false;

private:
	std::thread workThread;
};