#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glfw3.h>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float mouseSensitivity;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

    void ProcessMouseMovement(float xOffset, float yOffset);

    glm::mat4 GetViewMatrix();

private:
    void updateCameraVectors();
};
