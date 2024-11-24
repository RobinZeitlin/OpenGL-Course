#pragma once

#include "rendering/Mesh.h"
#include "shaders/Shader.h"

#include "camera/Camera.h"

class GameObject {
public:
	GameObject(Mesh* mesh, Shader* shader, Camera* camera, glm::vec3 pos)
		: mesh(mesh), shader(shader), camera(camera), position(pos) { }

	void change_name(std::string newName) { name = newName; }
	void draw();

	std::string name = "Nothing";

	glm::vec3 position;

	Camera* camera;
	Shader* shader;
	Mesh* mesh;
};