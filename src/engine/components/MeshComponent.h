#pragma once

#include "../Component.h"

class MeshComponent : public Component{
public:
	MeshComponent(GameObject* attachedTo)
		: Component(attachedTo) { }

	void draw_inspector_widget() override;
	void update() override;
};