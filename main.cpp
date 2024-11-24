#include <iostream>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glfw3.h>
#include <glad/glad.h>

#include "src/shaders/Shader.h"
#include "src/Cube.h"
#include "src/camera/Camera.h"

#include "src/rendering/Texture.h" 

#include "src/GameObject.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <chrono>

GLFWwindow* window;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = 320, lastY = 240;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 position = glm::vec3(1.0f);

std::vector<GameObject*> objects;
GameObject* selectedObject;

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

    window = glfwCreateWindow(1000, 800, "Hello World", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Shader* shader = new Shader();
    shader->Initialize("src/shaders/vertex.shader", "src/shaders/fragment.shader");

    Texture* texture = new Texture("res/textures/defaulttexture.png");

    Cube* cube = new Cube();
    cube->apply_texture(texture);
    auto object = new GameObject(cube, shader, &camera, { 1, 0, 0 });
    object->change_name("Object1");
    objects.push_back(object);

    Cube* cube2 = new Cube();
    cube2->apply_texture(texture);
    auto object2 = new GameObject(cube2, shader, &camera, { 0, 0, 0 });
    object2->change_name("Object2");
    objects.push_back(object2);

    Cube* cube3 = new Cube();
    cube3->apply_texture(texture);
    auto object3 = new GameObject(cube3, shader, &camera, { -1, 0, 0 });
    object3->change_name("Object3");
    objects.push_back(object3);

    while (!glfwWindowShouldClose(window))
    {
        // Calculate delta time
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
        }

        for (size_t i = 0; i < objects.size(); i++) {
            objects[i]->draw();
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hierarchy");
        if (!objects.empty()) {
            for (size_t i = 0; i < objects.size(); i++) {
                auto thisObject = objects[i];
                if (ImGui::Selectable(thisObject->name.c_str())) {
                    selectedObject = thisObject;
                }
            }
        }
        ImGui::End();

        ImGui::Begin("Inspector");
        if (selectedObject != nullptr)
        {
            ImGui::InputText("Name", &selectedObject->name[0], selectedObject->name.size() + 1);

            if (ImGui::CollapsingHeader("Transform")) {
                ImGui::DragFloat3("Position", &selectedObject->position[0], 0.1f);
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
