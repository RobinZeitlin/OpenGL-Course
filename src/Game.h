#pragma once

#include <vector>
#include <iostream>
#include <memory> 

#include "shaders/Shader.h"

#include "rendering/Mesh.h"
#include "rendering/Texture.h"

#include "camera/Camera.h"

#include "Cube.h"
#include "GameObject.h"

#include "engine/OBJLoader.h"

class Game {
public: 
	Game(Camera* camera);
	~Game();

	void update();
	void draw();

	void delete_object(GameObject* gameObject);

	std::vector<GameObject*> objects;
	GameObject* selectedObject;

	Camera* camera;
	OBJLoader* objLoader;

};