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

    glm::mat4 myProjection;
    glm::mat4 myView;

    float yaw;
    float pitch;

    float mouseSensitivity;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);

    void process_mouse_movement(float xOffset, float yOffset);
    void input(GLFWwindow* window, float deltaTime);

    glm::mat4 GetViewMatrix();

private:
    void updateCameraVectors();
};
