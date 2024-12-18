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

#include <condition_variable>

#include <glm.hpp>

#include "messages/MeshMessage.h"

class Mesh;

class OBJLoader {
public:
	static OBJLoader& get_instance() {
		static OBJLoader instance;
		return instance;
	}
	OBJLoader();
	~OBJLoader();

	void clean_up();

	Mesh* get_mesh(std::string objName);
	Mesh* load_mesh(std::string objPath);

	void start_thread();
	void stop_thread();
	void worker_thread();

	std::mutex filesMutex;
	std::thread loadingThread;
	bool running = false;

	void process_message(Message* message);

	std::unordered_map<std::string, Mesh*> files;

	std::string filePath = "res/objmodels/";
	std::string fileFormat = ".obj";

	std::queue<Message*> messageQueue;
};