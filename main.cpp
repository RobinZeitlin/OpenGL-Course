#include <iostream>
#include <glfw3.h>
#include <glad/glad.h>

#include "src/shaders/Shader.h"
#include "src/Cube.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

GLFWwindow* window;

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

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Use();

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(1.5f, 1.5f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		glm::mat4 transform = glm::mat4(1.0f);
		float timeValue = glfwGetTime();
		float angle = timeValue * 50.0f;

		transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

		shader->SetMatrix4(projection, "projection");
		shader->SetMatrix4(view, "view");
		shader->SetMatrix4(transform, "transform");

		cube->Draw(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}