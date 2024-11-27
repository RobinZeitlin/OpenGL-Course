#include "Game.h"

Game::Game(Camera* camera) 
    : camera(camera) {

    objLoader = new OBJLoader();
    if (!objLoader->init()) return;

    Shader* shader = new Shader();
    shader->Initialize("src/shaders/vertex.shader", "src/shaders/fragment.shader");

    Texture* texture = new Texture("res/textures/defaulttexture.png");
    defaultIcon = new Texture("res/textures/defaulticon.png");

    for (size_t i = 0; i < 3; i++) {
        auto mesh = objLoader->getMesh("diamant");
        auto object = new GameObject(mesh, shader, texture, camera, glm::vec3(3 * i, 0, -3));
        object->change_name(("Object" + std::to_string(i)).c_str());
        objects.push_back(object);
    }
}

Game::~Game() {
    for (size_t i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
}

void Game::update() {

}

void Game::draw() {
    for (size_t i = 0; i < objects.size(); i++) {
        if (objects[i] == nullptr) continue;
        if (!objects[i]->bIsVisible) continue;
            objects[i]->draw();
    }
}

void Game::delete_object(GameObject* gameObject) {
    auto it = std::find(objects.begin(), objects.end(), gameObject);
    if (it != objects.end()) {
        objects.erase(it);
    }

    selectedObject = nullptr;
    delete gameObject;
}
