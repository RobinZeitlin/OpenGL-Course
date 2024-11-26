#include "GameObject.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "rendering/Mesh.h"
#include "shaders/Shader.h"
#include "rendering/Texture.h"

GameObject::GameObject(Mesh* mesh, Shader* shader, Texture* texture, Camera* camera, glm::vec3 pos) 
    : mesh(mesh), shader(shader), texture(texture), camera(camera)
{
    transform.position = pos;
    transform.scale = glm::vec3(1);

    mesh->apply_texture(texture);
}

void GameObject::draw()
{
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

    this->mesh->Draw(shader);
}
