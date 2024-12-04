#pragma once

class GameObject;

class Component {
public:
	Component(GameObject* attachedTo) 
		: gameObject(attachedTo) { }

	virtual void draw_inspector_widget() = 0;
	virtual void update() = 0;

	GameObject* gameObject = nullptr;
};