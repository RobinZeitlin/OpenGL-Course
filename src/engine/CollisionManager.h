#pragma once

#include "components/CollisionComponent.h"

class CollisionManager {
public:
	CollisionManager() { }
	~CollisionManager() { }

	void CompareCollision(CollisionComponent a, CollisionComponent b);
};