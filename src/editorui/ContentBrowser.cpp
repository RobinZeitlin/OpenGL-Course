#include "ContentBrowser.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../Game.h"

void ContentBrowser::draw(Game* game)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;

    if (ImGui::Begin("UtilWindow", nullptr, window_flags))
    {
        if (ImGui::BeginTabBar("Util"))
        {
            if (ImGui::BeginTabItem("ContentBrowser"))
            {
                ImGui::Text("Content");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}
