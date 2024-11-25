#pragma once

#include "rendering/Mesh.h"
#include "shaders/Shader.h"
#include "rendering/Texture.h"

#include "camera/Camera.h"

#include "engine/Transform.h"

class GameObject {
public:
	GameObject(Mesh* mesh, Shader* shader, Texture* texture, Camera* camera, glm::vec3 pos)
		: mesh(mesh), shader(shader), texture(texture), camera(camera) 
	{ 
		transform.position = pos;
		transform.scale = glm::vec3(1);

		mesh->apply_texture(texture);
	}

	~GameObject() {
		delete mesh;
	}
	void change_name(std::string newName) { name = newName; }
	void draw();

	void change_texture(Texture* newTexture) {
		texture = newTexture;
	}

	bool bIsVisible = true;
	std::string name = "Nothing";

	Transform transform;
	Shader* shader;
	Mesh* mesh;
	Texture* texture;

private:
	Camera* camera;
};