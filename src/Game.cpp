#include "Game.h"

#include <vector>

#include "LightSource.h"

Game* gameInstance = nullptr;

Game::Game(Camera* camera) 
    : flyingCamera(camera) {

    gameInstance = this;

    basicShader = new Shader();
    basicShader->Initialize("../src/shaders/vertex.shader", "../src/shaders/fragment.shader");

    gridShader = new Shader();
    gridShader->Initialize("../src/shaders/gridvertex.shader", "../src/shaders/gridfragment.shader");

    defaultIcon = TextureLoader::get_instance().get_texture("defaulticon");

    //auto mesh = OBJLoader::get_instance().get_mesh("cube");

    auto object = new GameObject(
        nullptr,
        basicShader,
        TextureLoader::get_instance().get_texture("defaulttexture"),
        flyingCamera,
        glm::vec3(0, 0, 0));

    object->change_name(("Object" + std::to_string(objects.size())).c_str());
    objects.push_back(object);

    spawn_light();

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

void Game::spawn_object()
{
}


void Game::spawn_light()
{

}

void Game::delete_object(GameObject* gameObject) {
    auto it = std::find(objects.begin(), objects.end(), gameObject);
    if (it != objects.end()) {
        objects.erase(it);
    }

    selectedObject = nullptr;
    delete gameObject;
}
