#include <iostream>
#include <vector>

#include <glfw3.h>
#include <glad/glad.h>

#include "src/shaders/Shader.h"
#include "src/camera/Camera.h"

#include "src/rendering/Texture.h" 

#include "src/GameObject.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <chrono>

#include "src/Game.h"
#include "src/Editor.h"

GLFWwindow* window;

Editor* editor;

Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));

float lastX = 320, lastY = 240;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const int width = 1000;
const int height = 800;

Game* gGameInstance = nullptr;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {
        return;
    }

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.process_mouse_movement(xOffset, yOffset);
}

int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1000, 800, "GEngine", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    gameInstance = new Game(&camera);
    editor = new Editor(window, gameInstance);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera.input(window, deltaTime);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstMouse = true;
        }

        gameInstance->draw();
        gameInstance->update();

        editor->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
