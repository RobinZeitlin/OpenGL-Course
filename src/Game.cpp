#include "Game.h"

#include <vector>

Game* gameInstance = nullptr;

Game::Game(Camera* camera) 
    : flyingCamera(camera) {

    gameInstance = this;

    basicShader = new Shader();
    basicShader->Initialize("src/shaders/vertex.shader", "src/shaders/fragment.shader");

    gridShader = new Shader();
    gridShader->Initialize("src/shaders/gridvertex.shader", "src/shaders/gridfragment.shader");

    defaultIcon = TextureLoader::get_instance().get_texture("defaulticon");

    for (size_t i = 0; i < 1; i++) {
        auto mesh = OBJLoader::get_instance().get_mesh("starship");

        auto object = new GameObject(
            mesh,
            basicShader,
            TextureLoader::get_instance().get_texture("defaulttexture"), 
            camera, 
            glm::vec3(3 * i, 0, -3));

        object->change_name(("Object" + std::to_string(i)).c_str());
        objects.push_back(object);
    }

    editorGrid = new EditorGrid(100, 100);

    memoryStatus = new MemoryStatus();
}

Game::~Game() {
    for (size_t i = 0; i < objects.size(); i++) {
        delete objects[i];
    }

    delete defaultIcon;
    delete flyingCamera;
    delete editorGrid;
}

void Game::update() {
    editorGrid->update(1);
}

void Game::draw() {
    for (size_t i = 0; i < objects.size(); i++) {
        if (objects[i] == nullptr) continue;
        if (!objects[i]->bIsVisible) continue;
            objects[i]->draw();
    }

    editorGrid->draw(gridShader, flyingCamera);
}

void Game::delete_object(GameObject* gameObject) {
    auto it = std::find(objects.begin(), objects.end(), gameObject);
    if (it != objects.end()) {
        objects.erase(it);
    }

    selectedObject = nullptr;
    delete gameObject;
}
