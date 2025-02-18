#pragma once

#include "components/CollisionComponent.h"

class CollisionManager {
public:
	CollisionManager() { }
	~CollisionManager() { }

	void update();
	void compare_collision(CollisionComponent a, CollisionComponent b);
};