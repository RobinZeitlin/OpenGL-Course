#pragma once

#include "../Component.h"

class LightComponent : public Component {
public:
	LightComponent(GameObject* attachedTo)
		: Component(attachedTo) { }

	void draw_inspector_widget() override;
	void update() override;

	enum class LightType {
		Directional,
		Point,
		Spot
	};

	LightType type = LightType::Spot;
	float lightColor[3] = { 1.0f, 1.0f, 1.0f };
};