#include "Hierarchy.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../Game.h"

void Hierarchy::draw(Game* game, std::vector<GameObject*> objects)
{
    ImGui::Begin("Hierarchy");

    if (!objects.empty()) {
        for (size_t i = 0; i < objects.size(); i++) {
            auto thisObject = objects[i];
            bool isSelected = (game->selectedObject == thisObject);
            if (ImGui::Selectable(thisObject->name.c_str(), isSelected)) {
                game->selectedObject = thisObject;
            }
            if (!thisObject->bIsVisible) {
                ImGui::SameLine();
                ImGui::Text("-");
            }
        }
    }
    ImGui::End();
}
