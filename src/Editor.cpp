#include "Editor.h"

#include "Game.h"

Editor::Editor(GLFWwindow* window, Game* game)
    : window(window), game(game)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    SetupImGuiStyle();
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

void Editor::SetupImGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);

    colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);

    colors[ImGuiCol_Button] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.44f, 0.85f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.18f, 0.30f, 0.60f, 1.00f);

    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.44f, 0.85f, 1.00f);

    colors[ImGuiCol_Header] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.44f, 0.85f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.44f, 0.85f, 1.00f);

    colors[ImGuiCol_Tab] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.44f, 0.85f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.18f, 0.30f, 0.60f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.30f, 0.60f, 1.00f);

    colors[ImGuiCol_PlotLines] = ImVec4(0.88f, 0.91f, 0.91f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.26f, 0.44f, 0.85f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.88f, 0.91f, 0.91f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.26f, 0.44f, 0.85f, 1.00f);

    colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.44f, 0.85f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.44f, 0.85f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.26f, 0.44f, 0.85f, 1.00f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);

    style.WindowRounding = 4.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.TabRounding = 4.0f;
}
