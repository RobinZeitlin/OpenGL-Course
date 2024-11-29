#pragma once

#include <vector>
#include <iostream>
#include <memory> 

#include "shaders/Shader.h"

#include "rendering/Mesh.h"
#include "rendering/Texture.h"

#include "camera/Camera.h"

#include "GameObject.h"

#include "engine/OBJLoader.h"
#include "engine/TextureLoader.h"
#include "engine/EditorGrid.h"

class Game {
public: 
	Game(Camera* camera);
	~Game();

	void update();
	void draw();

	void delete_object(GameObject* gameObject);

	std::vector<GameObject*> objects;
	GameObject* selectedObject;

	Camera* flyingCamera;

	Texture* defaultIcon;
	Shader* basicShader;
	Shader* gridShader;

	EditorGrid* editorGrid;
};

extern Game* gameInstance;