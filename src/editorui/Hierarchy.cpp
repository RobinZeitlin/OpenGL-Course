#include "Hierarchy.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../Game.h"

void Hierarchy::draw(Game* game, std::vector<GameObject*> objects)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    if (ImGui::Begin("Hierarchy", nullptr, window_flags))
    {
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
