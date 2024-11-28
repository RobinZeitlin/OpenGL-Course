#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), mouseSensitivity(0.1f) {
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

void Camera::process_mouse_movement(float xOffset, float yOffset) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateCameraVectors();
}

void Camera::input(GLFWwindow* window, float deltaTime) {
    float cameraSpeed = 10.0f * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += front * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= front * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= right * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += right * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position += glm::vec3(0, 1, 0)  * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        position += glm::vec3(0, -1, 0) * cameraSpeed;
}

void Camera::focus_object(glm::vec3 targetPosition) {
    glm::vec3 direction = glm::normalize(targetPosition - position);

    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(worldUp, direction));
    glm::vec3 adjustedUp = glm::cross(direction, right);

    front = direction;
    up = adjustedUp;
    position = targetPosition - direction * 5.0f;
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::updateCameraVectors() {
    glm::vec3 calculatedFront;
    calculatedFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    calculatedFront.y = sin(glm::radians(pitch));
    calculatedFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(calculatedFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
