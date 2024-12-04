#pragma once

#include "Component.h"

class TransformComponent : public Component {
public:
	TransformComponent(GameObject* attachedTo)
		: Component(attachedTo) { }

	void draw_inspector_widget() override;
	void update() override;
};