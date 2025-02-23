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

#include "engine/memory/MemoryStatus.h"
#include "engine/CollisionManager.h"

class LightSource;

class Game {
public: 
	Game(Camera* camera);
	~Game();

	void update();
	void draw();
	
	void spawn_object();
	void spawn_light();
	void delete_light(GameObject* light);
	void delete_object(GameObject* gameObject);

	std::vector<GameObject*> objects;
	std::vector<GameObject*> lightsInScene;

	GameObject* selectedObject;

	Camera* flyingCamera;

	Texture* defaultIcon;
	Shader* basicShader;
	Shader* gridShader;
	Shader* gizmoShader;

	EditorGrid* editorGrid;
	MemoryStatus* memoryStatus;
	CollisionManager* collisionManager;
};

extern Game* gameInstance;