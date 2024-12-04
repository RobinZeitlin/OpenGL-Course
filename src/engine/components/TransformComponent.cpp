#include "TransformComponent.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <filesystem>

#include "../../Game.h"

void TransformComponent::draw_inspector_widget() {
    if (ImGui::CollapsingHeader("Transform")) {
        ImGui::DragFloat3("Position", &gameInstance->selectedObject->transform.position[0], 0.1f);
        ImGui::DragFloat3("Rotation", &gameInstance->selectedObject->transform.eulerAngles[0], 0.1f);
        ImGui::DragFloat3("Scale", &gameInstance->selectedObject->transform.scale[0], 0.1f);
    }
}

void TransformComponent::update() {
}