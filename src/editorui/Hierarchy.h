#pragma once

#include <iostream>
#include <vector>

#include "../GameObject.h"

class Game;

class Hierarchy {
public:
	void draw(Game* game, std::vector<GameObject*> objects);
};