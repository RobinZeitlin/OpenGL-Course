#include "Toolbar.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../Game.h"

void Toolbar::draw(Game* game)
{
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Tools")) {
            if (game->selectedObject != nullptr) {
                if (ImGui::MenuItem("Focus on Object - F")) {
                    game->camera->focus_object(game->selectedObject->transform.position);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
