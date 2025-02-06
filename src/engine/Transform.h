#pragma once

#include "glm.hpp"

class Transform {
public:
	glm::vec3 position;
	glm::vec3 eulerAngles;
	glm::vec3 scale;

    Transform() : position(0.0f), eulerAngles(0.0f), scale(1.0f) {}

    glm::vec3 get_forward() const {
        glm::mat4 rotation = glm::mat4(1.0f);
        rotation = glm::rotate(rotation, glm::radians(eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
        rotation = glm::rotate(rotation, glm::radians(eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotation = glm::rotate(rotation, glm::radians(eulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f)); 
        glm::vec4 forward = rotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        return glm::normalize(glm::vec3(forward));
    }
};