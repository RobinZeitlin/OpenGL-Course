#include "Inspector.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <filesystem>

#include "../Game.h"

#include "../engine/memory/MemoryStatus.h"

Inspector::Inspector()
{ }

void Inspector::draw(Game* game)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    if (ImGui::Begin("Inspector", nullptr, window_flags))
    {
        if (ImGui::BeginTabBar("InspectorTabBar"))
        {
            if (ImGui::BeginTabItem("Components"))
            {
                draw_component_window(game);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Profiler"))
            {
                auto [availablePhysical, availableVirtual] = MemoryStatus::get_instance().get_available_memory();

                ImGui::Text(("Physical Memory: " + std::to_string(static_cast<int>(availablePhysical)) + " MB").c_str());
                ImGui::Text(("Virtual Memory: " + std::to_string(static_cast<int>(availableVirtual)) + " MB").c_str());
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}

void Inspector::draw_component_window(Game* game) {
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

        for (auto components : game->selectedObject->components) {
            components->draw_inspector_widget();
        }
    }
}
