#pragma once

#include "../Component.h"

class Mesh;
class Shader;

class CollisionComponent : public Component {
public:
	CollisionComponent(GameObject* attachedTo);

	void draw_inspector_widget() override;
	void update() override;

	void on_trigger_enter(GameObject* other);

	void set_shader(Shader* newShader);

	Shader* shader = nullptr;
	Mesh* mesh = nullptr;

	float radius = 0.5f;

	enum class CollisionType {
		Sphere,
		Box
	};

	CollisionType type = CollisionType::Sphere;
	bool bMeshChanged = false;
};