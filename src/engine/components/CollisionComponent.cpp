#include "CollisionComponent.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <filesystem>

#include "../../Game.h"

void CollisionComponent::draw_inspector_widget() {
    if (ImGui::CollapsingHeader("Collision")) {
        const char* current_type = (type == CollisionType::Sphere) ? "Sphere" : "Box";

        if (ImGui::BeginCombo("Collision Type", current_type)) {
            if (ImGui::Selectable("Sphere", type == CollisionType::Sphere)) {
                type = CollisionType::Sphere;
            }
            if (ImGui::Selectable("Box", type == CollisionType::Box)) {
                type = CollisionType::Box;
            }
            ImGui::EndCombo();
        }

        if (type == CollisionType::Sphere) {
            radius = 1.0f;
            ImGui::SliderFloat("Radius", &radius, 0.1f, 5.0f, "%.2f");

            float window_width = ImGui::GetContentRegionAvail().x;
            float size = 50.0f;
            float icon_x = (window_width - size) * 0.5f;

            ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
            ImVec2 center(cursor_pos.x + icon_x + size * 0.5f, cursor_pos.y + size * 0.5f);
            ImColor green = ImColor(0, 255, 0);

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddCircle(center, radius * 5.0f, green, 32, 2.0f);

            ImGui::Dummy(ImVec2(window_width, size + 5.0f));

            char radius_text[32];
            sprintf_s(radius_text, "Radius: %.2f", radius);

            float text_width = ImGui::CalcTextSize(radius_text).x;
            float text_x = (window_width - text_width);
            ImGui::SetCursorPosX(text_x);
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", radius_text);
        }
        else if (type == CollisionType::Box) {
            glm::vec3 scale = gameObject->transform.scale;

            float window_width = ImGui::GetContentRegionAvail().x;
            float size = 50.0f;
            float icon_x = (window_width - size) * 0.5f;

            ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
            ImVec2 center(cursor_pos.x + icon_x + size * 0.5f, cursor_pos.y + size * 0.5f);
            ImColor green = ImColor(0, 255, 0);

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRect(ImVec2(center.x - size * 0.5f, center.y - size * 0.5f),
                ImVec2(center.x + size * 0.5f, center.y + size * 0.5f),
                green, 0.0f, 0, 2.0f);

            ImGui::Dummy(ImVec2(window_width, size + 5.0f));

            char scale_text[64];
            sprintf_s(scale_text, "Scale: (%.2f, %.2f, %.2f)", scale.x, scale.y, scale.z);

            float text_width = ImGui::CalcTextSize(scale_text).x;
            float text_x = (window_width - text_width) * 0.5f;
            ImGui::SetCursorPosX(text_x);

            ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", scale_text);
        }
    }
}


void CollisionComponent::update() {

}

void CollisionComponent::onTriggerEnter(GameObject* other)
{
    std::cout << "Collision with" << other->name << std::endl;
}
