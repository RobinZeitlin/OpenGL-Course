#include "Hierarchy.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <filesystem>

#include "../Game.h"

void Hierarchy::draw(Game* game, std::vector<GameObject*> objects)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    if (ImGui::Begin("Hierarchy", nullptr, window_flags))
    {
        if (ImGui::Button("Add")) {
            ImGui::SetNextWindowSize(ImVec2(200, 150));
            game->spawn_object();
        }

        if (ImGui::BeginPopup("GameObjectPopup")) {
            std::vector<std::string> items;
            std::string filePath = "res/textures/";
            std::filesystem::path folderPath = std::filesystem::current_path() / filePath;

            for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
                if (entry.is_regular_file()) {
                    std::string textureName = entry.path().stem().string();
                    items.push_back(textureName);
                }
            }

            ImGui::Text("Files found : %s", std::to_string(items.size()).c_str());
            for (auto item : items) {
                if (ImGui::Selectable(item.c_str())) {
                    
                }
            }

            ImGui::EndPopup();
        }

        if (ImGui::BeginTabBar("HierarchyTabBar"))
        {
            if (ImGui::BeginTabItem("Hierarchy"))
            {
                for (size_t i = 0; i < objects.size(); i++)
                {
                    auto thisObject = objects[i];
                    bool isSelected = (game->selectedObject == thisObject);
                    if (ImGui::Selectable(thisObject->name.c_str(), isSelected))
                    {
                        game->selectedObject = thisObject;
                    }
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}
