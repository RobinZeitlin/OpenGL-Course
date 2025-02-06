#include "LightComponent.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <filesystem>

#include "../../Game.h"

void LightComponent::draw_inspector_widget() {
    if (ImGui::CollapsingHeader("Light")) {
        // Dropdown menu for Light Type
        const char* current_type = nullptr;

        switch (type) {
        case LightType::Directional: current_type = "Directional"; break;
        case LightType::Point: current_type = "Point"; break;
        case LightType::Spot: current_type = "Spot"; break;
        }

        if (ImGui::BeginCombo("Light Type", current_type)) {
            if (ImGui::Selectable("Directional", type == LightType::Directional)) {
                type = LightType::Directional;
            }
            if (ImGui::Selectable("Point", type == LightType::Point)) {
                type = LightType::Point;
            }
            if (ImGui::Selectable("Spot", type == LightType::Spot)) {
                type = LightType::Spot;
            }
            ImGui::EndCombo();
        }

        ImGui::ColorEdit3("Light Color", lightColor);
    }
}

void LightComponent::update() {

}