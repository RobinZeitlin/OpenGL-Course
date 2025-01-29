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

#include "MeshData.h"

#include "../engine/messages/MeshMessage.h"

#include <vector>
#include <unordered_set>

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

    void create_thread();

    void start_worker_thread();
    void worker();

    void recieve_message(MeshMessage* newMessage);

    Mesh* get_mesh(std::string objName);
    MeshData* load_mesh(std::string objPath);

    std::queue<MeshMessage*> messages;
    std::unordered_map<std::string, Mesh*> files;
    std::string filePath = "../res/objmodels/";
    std::string fileFormat = ".obj";

    bool bLoadingThreadIsRunning = false;
    std::mutex meshLoadingMutex;

private:
    std::thread workThread;
    std::mutex messageMutex;
    std::mutex filesMutex;
    std::condition_variable workerCondition;

    std::queue<MeshToLoad*> meshDataToLoad;

    std::unordered_set<std::string> messageSet;

    std::function<void(MeshData*)> meshCreationCallback;
};