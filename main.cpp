#include <iostream>
#include <glfw3.h>
#include <glad/glad.h>

#include "src/shaders/Shader.h"
#include "src/Cube.h"
#include "src/camera/Camera.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

GLFWwindow* window;
Camera camera;

float lastX = 320, lastY = 240;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 0;
	}

	glEnable(GL_DEPTH_TEST);

	Shader* shader = new Shader();
	shader->Initialize("src/shaders/vertex.shader", "src/shaders/fragment.shader");

	Cube* cube = new Cube();

	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Use();

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);

		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view = camera.GetViewMatrix();
		
		glm::mat4 transform = glm::mat4(1.0f);

		shader->SetMatrix4(projection, "projection");
		shader->SetMatrix4(view, "view");
		shader->SetMatrix4(transform, "transform");

		cube->Draw(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}