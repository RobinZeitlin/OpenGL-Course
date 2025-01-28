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

#include <vector>

class Mesh;

class OBJLoader {
public:
    // Vertex data: position (3 floats), UV (2 floats), normal (3 floats)
    std::vector<float> testVertexData = {
        // Front face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Vertex 1
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Vertex 2
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // Vertex 3
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // Vertex 4

        // Back face
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Vertex 5
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Vertex 6
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, // Vertex 7
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, // Vertex 8

        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f, // Vertex 9
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f, // Vertex 10
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f, // Vertex 11
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f, // Vertex 12

        // Top face
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Vertex 13
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Vertex 14
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Vertex 15
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Vertex 16

        // Left face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Vertex 17
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Vertex 18
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // Vertex 19
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // Vertex 20

        // Right face
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Vertex 21
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Vertex 22
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, // Vertex 23
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f  // Vertex 24
    };

    // Index data (unsigned int)
    std::vector<unsigned int> testIndices = {
        0, 1, 2, 0, 2, 3,    // Front face
        4, 5, 6, 4, 6, 7,    // Back face
        8, 9, 10, 8, 10, 11,  // Bottom face
        12, 13, 14, 12, 14, 15, // Top face
        16, 17, 18, 16, 18, 19, // Left face
        20, 21, 22, 20, 22, 23  // Right face
    };

	static OBJLoader& get_instance() {
		static OBJLoader instance;
		return instance;
	}

	~OBJLoader();

	void clean_up();
	void create_thread();

	void recieve_message(MeshMessage* newMessage);

	Mesh* get_mesh(std::string objName);
	MeshInfo* load_mesh(std::string objPath);

	std::queue<MeshMessage*> messages;

	std::unordered_map<std::string, Mesh*> files;
	std::string filePath = "res/objmodels/";
	std::string fileFormat = ".obj";

	bool bLoadingThreadIsRunning = false;

private:
	std::thread workThread;
};