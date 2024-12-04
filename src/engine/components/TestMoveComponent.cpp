#include "TestMoveComponent.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <filesystem>

#include "../../Game.h"

void TestMoveComponent::draw_inspector_widget() {
    if (ImGui::CollapsingHeader("Move")) {
        ImGui::DragFloat("Speed", &speed, 0.1f, -10, 10);
    }
}

void TestMoveComponent::update() {
    gameObject->transform.position += glm::vec3(0, speed / 100, 0);
}