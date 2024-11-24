#include "GameObject.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

void GameObject::draw()
{
    shader->Use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 transform = glm::mat4(1.0f);

    transform = glm::translate(transform, position);

    shader->SetMatrix4(projection, "projection");
    shader->SetMatrix4(view, "view");
    shader->SetMatrix4(transform, "transform");

    this->mesh->Draw(shader);
}
