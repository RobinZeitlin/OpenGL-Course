#pragma once

#include <iostream>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "editorui/Hierarchy.h"
#include "editorui/Inspector.h"
#include "editorui/Toolbar.h"

class Game;

class Editor {
public:
	Editor(GLFWwindow* window, Game* game);
	~Editor();

	void draw();

	GLFWwindow* window;
	Game* game;

	Hierarchy* hierarchy;
	Inspector* inspector;
	Toolbar* toolbar;
};