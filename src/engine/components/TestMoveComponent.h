#pragma once

#include "Component.h"

class TestMoveComponent : public Component {
public:
	TestMoveComponent(GameObject* attachedTo)
		: Component(attachedTo) { }

	void draw_inspector_widget() override;
	void update() override;

private:
	float speed = 0.0f;
};