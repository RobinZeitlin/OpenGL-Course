#include "Inspector.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <filesystem>
#include <iostream>

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
    GameObject* selectedObject = game->selectedObject;
    if (game == nullptr || selectedObject == nullptr) return;
    if (selectedObject->components.empty()) return;

    ImGui::InputText("Name", &selectedObject->name[0], selectedObject->name.size() + 1);

    ImGui::SameLine();

    const char* visibleTxt = selectedObject->bIsVisible ? "O" : "-";
    if (ImGui::Button(visibleTxt, ImVec2(20, 20))) {
        selectedObject->bIsVisible = !selectedObject->bIsVisible;
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
        ImGui::SetTooltip("Change Visibility");
    }

    if (ImGui::Button("Delete", ImVec2(60, 20))) {
        if (selectedObject->is_component_already_added("LightComponent")) {
			game->delete_light(selectedObject);
			return;
        }

        game->delete_object(selectedObject);
        return;
    }

    for (auto& [name, component] : selectedObject->components) {
        if (component != nullptr) {
            component->draw_inspector_widget();
        }
    }

    draw_add_component_button();
}


void Inspector::draw_add_component_button()
{
    std::string addComponentTxt = "Add Component +";
    ImVec2 size = ImGui::CalcTextSize(addComponentTxt.c_str());
    ImVec2 offset = ImVec2(8);
    size.x += offset.x;
    size.y += offset.y;

    if (ImGui::Button(addComponentTxt.c_str(), size)) {
        ImGui::OpenPopup("AddComponent");
    }

    if (ImGui::BeginPopup("AddComponent")) {
        std::vector<std::string> items;
        std::string filePath = "src/engine/components";
        std::filesystem::path folderPath = std::filesystem::current_path() / filePath;

        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                std::string textureName = entry.path().filename().string();
                if (textureName.ends_with(".h")) {
                    textureName.erase(textureName.size() - 2);

                    if(gameInstance->selectedObject->is_component_already_added(textureName) == false)
                    items.push_back(textureName);
                }
            }
        }

        ImGui::Text("Files found : %s", std::to_string(items.size()).c_str());
        for (auto item : items) {
            if (ImGui::Selectable(item.c_str())) {
                gameInstance->selectedObject->add_component(item);
            }
        }

        ImGui::EndPopup();
    }
}