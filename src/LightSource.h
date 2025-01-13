#pragma once

#include "GameObject.h"

class LightSource : public GameObject {
public:
    LightSource(Mesh* mesh, Shader* shader, Texture* texture, Camera* camera, glm::vec3 pos)
        : GameObject(mesh, shader, texture, camera, pos) {
    }

    ~LightSource() {
    }
};