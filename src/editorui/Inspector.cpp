#include "Inspector.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../Game.h"

Inspector::Inspector()
{ }

void Inspector::draw(Game* game)
{
    ImGui::Begin("Inspector");

    if (game->selectedObject != nullptr) {
        ImGui::InputText("Name", &game->selectedObject->name[0], game->selectedObject->name.size() + 1);

        ImGui::SameLine();

        const char* visibleTxt = game->selectedObject->bIsVisible ? "O" : "-";
        if (ImGui::Button(visibleTxt, ImVec2(20, 20))) {
            game->selectedObject->bIsVisible = !game->selectedObject->bIsVisible;
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("Change Visibility");
        }

        if (ImGui::Button("Delete", ImVec2(60, 20))) {
            game->delete_object(game->selectedObject);
        }

        if (ImGui::CollapsingHeader("Transform")) {
            ImGui::DragFloat3("Position", &game->selectedObject->transform.position[0], 0.1f);
            ImGui::DragFloat3("Rotation", &game->selectedObject->transform.eulerAngles[0], 0.1f);
            ImGui::DragFloat3("Scale", &game->selectedObject->transform.scale[0], 0.1f);
        }

        if (ImGui::CollapsingHeader("Rendering")) {
            ImGui::Text("Texture : ");
            ImGui::SameLine();
            if (ImGui::ImageButton("Texture", game->selectedObject->texture->textureObject, ImVec2(50, 50))) { 
                
            }
        }
    }
    ImGui::End();
}
