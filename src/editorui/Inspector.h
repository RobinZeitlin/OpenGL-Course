#pragma once

class Game;

class Inspector {
public:
	Inspector();

	void draw(Game* game);
	void draw_component_window(Game* game);
};