#include "Editor.h"

#include "Game.h"

Editor::Editor(GLFWwindow* window, Game* game)
    : window(window), game(game)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    hierarchy = new Hierarchy();
    inspector = new Inspector();
    toolbar = new Toolbar();
}

Editor::~Editor()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete hierarchy;
    delete inspector;
    delete toolbar;
}

void Editor::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    hierarchy->draw(game, game->objects);
    inspector->draw(game);
    toolbar->draw(game);
    log->draw(game);
    contentBrowser->draw(game);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
