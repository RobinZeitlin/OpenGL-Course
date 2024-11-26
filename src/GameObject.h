#pragma once

#include <string>

#include "camera/Camera.h"
#include "engine/Transform.h"

class Mesh;
class Shader;
class Texture;

class GameObject {
public:
	GameObject(Mesh* mesh, Shader* shader, Texture* texture, Camera* camera, glm::vec3 pos);

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