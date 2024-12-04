#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "camera/Camera.h"
#include "engine/Transform.h"
#include "engine/components/Component.h"

#include "engine/ComponentManager.h"

class Mesh;
class Shader;
class Texture;

class GameObject {
public:
	GameObject(Mesh* mesh, Shader* shader, Texture* texture, Camera* camera, glm::vec3 pos);

	~GameObject() {
		delete mesh;
		delete compManager;
	}

	std::unordered_map<std::string, Component*> components;

	void change_name(std::string newName) { name = newName; }
	void draw();

	void change_texture(Texture* newTexture);
	void change_mesh(Mesh* newMesh);

	void add_component(std::string nameOfComponent);
	bool is_component_already_added(std::string nameOfComponent);

	bool bIsVisible = true;
	std::string name = "Nothing";

	Transform transform;
	Shader* shader;
	Mesh* mesh;
	Texture* texture;

	ComponentManager* compManager;
private:
	Camera* camera;
};