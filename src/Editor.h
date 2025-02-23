#pragma once

#include <iostream>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "editorui/Hierarchy.h"
#include "editorui/Inspector.h"
#include "editorui/Toolbar.h"
#include "editorui/Log.h"
#include "editorui/ContentBrowser.h"

class Game;

class Editor {
public:
	Editor(GLFWwindow* window, Game* game);
	~Editor();

	void draw();

	void SetupImGuiStyle();

	GLFWwindow* window;
	Game* game;

	Hierarchy* hierarchy;
	Inspector* inspector;
	Toolbar* toolbar;
	Log* log;
	ContentBrowser* contentBrowser;
};