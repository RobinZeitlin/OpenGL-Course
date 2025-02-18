#pragma once

#include "../Component.h"

#include "../components/MeshComponent.h"

class CollisionComponent : public Component {
public:
	CollisionComponent(GameObject* attachedTo)
		: Component(attachedTo) { }

	void draw_inspector_widget() override;
	void update() override;

	void onTriggerEnter(GameObject* other);

	float radius = 1.0f;

	enum class CollisionType {
		Sphere,
		Box
	};

	CollisionType type = CollisionType::Sphere;
};