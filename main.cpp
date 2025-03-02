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
Shader* shadowShader;

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
    std::filesystem::path currentPath = std::filesystem::current_path();

    if (!glfwInit())
    {
        std::cout << "Failed to initialize glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1200, 1000, "GEngine", NULL, NULL);
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

#pragma region Shadow Mapping 

    shadowShader = new Shader();
    shadowShader->Initialize(
        "src/shaders/shadowmap.vert",
        "src/shaders/shadowmap.frag");

    // Framebuffer for Shadow Map
    unsigned int shadowMapFBO;
    glGenFramebuffers(1, &shadowMapFBO);

    // Texture for Shadow Map FBO
    unsigned int shadowMapWidth = 2048, shadowMapHeight = 2048;
    unsigned int shadowMap;
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // Prevents darkness outside the frustrum
    float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    // Needed since we don't touch the color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glm::mat4 orthoProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
	glm::mat4 lightView = glm::lookAt(20.0f * glm::vec3(1), glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightProjection = orthoProjection * lightView;

	shadowShader->SetMatrix4(lightProjection, "lightProjection");

#pragma endregion

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		glEnable(GL_DEPTH_TEST);

		glViewport(0, 0, shadowMapWidth, shadowMapHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

        gameInstance->draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
