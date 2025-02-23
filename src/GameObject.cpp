#include "GameObject.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "rendering/Mesh.h"
#include "shaders/Shader.h"
#include "rendering/Texture.h"

#include "engine/components/TransformComponent.h"
#include "engine/components/MeshComponent.h"
#include "engine/components/TestMoveComponent.h"
#include "engine/components/LightComponent.h"
#include "engine/components/CollisionComponent.h"

#include <iostream>

GameObject::GameObject(Mesh* mesh, Shader* shader, Texture* texture, Camera* camera, glm::vec3 pos) 
    : mesh(mesh), shader(shader), texture(texture), camera(camera) {
    transform.position = pos;
    transform.scale = glm::vec3(1);

    if (mesh != nullptr) {
        mesh->apply_texture(texture);
    }
    else {
        std::cerr << "Error: mesh is nullptr" << std::endl;
    }

    compManager = new ComponentManager();
    add_component("TransformComponent");
    add_component("MeshComponent");
}

CollisionComponent* GameObject::get_collision_component()
{
    return is_component_already_added("CollisionComponent") ? static_cast<CollisionComponent*>(components["CollisionComponent"]) : nullptr;
}

void GameObject::add_component(std::string nameOfComponent) {
    auto component = compManager->create_component(nameOfComponent, this);
    components[nameOfComponent] = component;
}

bool GameObject::is_component_already_added(std::string nameOfComponent) {
    if (components[nameOfComponent] != nullptr) {
        return true;
    }

    return false;
}

void GameObject::draw() {
    if (mesh == nullptr || shader == nullptr) return;

    for (auto& [name, component] : components) {
        if (component != nullptr)
            component->update();
    }

    shader->Use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 transform = glm::mat4(1.0f);

    transform = glm::translate(transform, this->transform.position);
    transform = glm::rotate(transform, glm::radians(this->transform.eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(this->transform.eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(this->transform.eulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, this->transform.scale);

    shader->SetMatrix4(projection, "projection");
    shader->SetMatrix4(view, "view");
    shader->SetMatrix4(transform, "transform");

    if (camera->lightSources[0] != nullptr) {
        LightComponent* lightComp = static_cast<LightComponent*>(camera->lightSources[0]->components["LightComponent"]);
        glm::vec3 lightColor = glm::vec3(lightComp->lightColor[0], lightComp->lightColor[1], lightComp->lightColor[2]);
        glm::vec3 lightPos = camera->lightSources[0]->transform.position;

        shader->SetVector3f(lightPos, "lightPos");
        shader->SetVector3f(lightColor, "lightColor");
        shader->SetVector3f(camera->position, "viewPos");

        shader->SetInt(static_cast<int>(lightComp->type), "lightType");

        if (lightComp->type == LightComponent::LightType::Spot) {
            glm::vec3 lightDir = camera->lightSources[0]->transform.get_forward();
            shader->SetVector3f(lightDir, "lightDir");
        }
    }

    this->mesh->Draw(shader);
}

void GameObject::update()
{
    for (auto& [name, component] : components) {
		if (component != nullptr)
			component->update();
	}
}

void GameObject::change_texture(Texture* newTexture) {
    texture = newTexture;
    mesh->apply_texture(texture);
}

void GameObject::change_mesh(Mesh* newMesh) {
    mesh = newMesh;
    if (texture != nullptr);
    mesh->apply_texture(texture);   
}