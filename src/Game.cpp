#include "Game.h"

#include <vector>

#include "LightSource.h"

Game* gameInstance = nullptr;

Game::Game(Camera* camera) 
    : flyingCamera(camera) {

    gameInstance = this;

    basicShader = new Shader();
    basicShader->Initialize(
        "src/shaders/vertex.shader", 
        "src/shaders/fragment.shader");

    gridShader = new Shader();
    gridShader->Initialize(
        "src/shaders/gridvertex.shader", 
        "src/shaders/gridfragment.shader");

    gizmoShader = new Shader();
    gizmoShader->Initialize(
        "src/shaders/gizmovertex.shader", 
        "src/shaders/gizmofragment.shader");

    defaultIcon = TextureLoader::get_instance().get_texture("defaulticon");

    spawn_light();

    flyingCamera->lightSource = lightSource;

    editorGrid = new EditorGrid(15, 15);

    memoryStatus = new MemoryStatus();
    collisionManager = new CollisionManager();
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

    // update the objects
    for (size_t i = 0; i < objects.size(); i++) {
		if (objects[i] == nullptr) continue;
		objects[i]->update();
	}

    // check overlapping collision between the objects
    for (size_t i = 0; i < objects.size(); i++) {
        if (objects[i] == nullptr) continue;

        CollisionComponent* collCompA = objects[i]->get_collision_component();
        if (collCompA != nullptr) {
            for (size_t j = i + 1; j < objects.size(); j++) {
                if (objects[j] == nullptr) continue;

                CollisionComponent* collCompB = objects[j]->get_collision_component();
                if (collCompB != nullptr) {
                    collisionManager->compare_collision(*collCompA, *collCompB);
                }
            }
        }
    }
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
    auto mesh = OBJLoader::get_instance().get_mesh("cube");

    auto object = new GameObject(
        nullptr,
        basicShader,
        TextureLoader::get_instance().get_texture("boxtexture.png"),
        flyingCamera,
        glm::vec3(0, 0, 0));

    object->gizmoShader = gizmoShader;
    object->change_name(("Object" + std::to_string(objects.size())).c_str());
    objects.push_back(object);
}


void Game::spawn_light()
{
    auto mesh = OBJLoader::get_instance().get_mesh("cube");

    auto object = new GameObject(
        nullptr,
        basicShader,
        TextureLoader::get_instance().get_texture("light.png"),
        flyingCamera,
        glm::vec3(0, 0, 0));

    object->change_name(("Light" + std::to_string(objects.size())).c_str());
    objects.push_back(object);

    lightSource = object;
    lightSource->transform.scale = glm::vec3(0.1f, 0.1f, 0.1f);

    lightSource->add_component("LightComponent");
}

void Game::delete_object(GameObject* gameObject) {
    auto it = std::find(objects.begin(), objects.end(), gameObject);
    if (it != objects.end()) {
        objects.erase(it);
    }

    selectedObject = nullptr;
    delete gameObject;
}
