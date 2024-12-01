#include "Log.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../Game.h"

void Log::draw(Game* game)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;

    if (ImGui::Begin("UtilWindow", nullptr, window_flags))
    {
        if (ImGui::BeginTabBar("Util"))
        {
            if (ImGui::BeginTabItem("Log"))
            {
                ImGui::Text("[Warning] Log Not Implemented");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}
