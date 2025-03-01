#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "camera/Camera.h"
#include "engine/Transform.h"
#include "engine/Component.h"

#include "engine/ComponentManager.h"

class Mesh;
class Shader;
class Texture;
class CollisionComponent;

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
	void update();

	void change_texture(Texture* newTexture);
	void change_specular_texture(Texture* newTexture);
	void change_albedo_texture(Texture* newTexture);

	void change_mesh(Mesh* newMesh);

	void add_component(std::string nameOfComponent);
	bool is_component_already_added(std::string nameOfComponent);

	CollisionComponent* get_collision_component();

	bool bIsVisible = true;
	std::string name = "Nothing";

	Transform transform;
	Shader* shader;
	Mesh* mesh;

	Texture* texture;
	Texture* albedoMap;
	Texture* specularMap;

	Shader* gizmoShader;

	ComponentManager* compManager;

	Camera* camera;
private:

};