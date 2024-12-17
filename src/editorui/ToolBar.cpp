#include "Toolbar.h"

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../../src/engine/MeshOptimizer.h"

#include "../Game.h"

void Toolbar::draw(Game* game)
{
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Tools")) {

            if (game->selectedObject != nullptr) {
                if (ImGui::Button("Focus on object")) {
                    gameInstance->flyingCamera->focus_object(game->selectedObject->transform.position);
                }
            }

            if (game->selectedObject != nullptr) {
                if (ImGui::Button("Optimize Selected Objects Mesh")) {
                    ImGui::OpenPopup("OptimizeMesh");
                }
            }

            if (ImGui::BeginPopup("OptimizeMesh")) {
                MeshOptimizer optimizer = MeshOptimizer::get_instance();
                optimizer.optimize_mesh(game->selectedObject->mesh);
                ImGui::EndPopup();
            }

            if (ImGui::Button("Camera Settings")) {
                ImGui::OpenPopup("CameraSettings");
            }

            if (ImGui::BeginPopup("CameraSettings")) {
                Camera* camera = game->flyingCamera;
                ImGui::SliderFloat("Camera Speed", &camera->speed, 0.0f, 100.0f);
                ImGui::EndPopup();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
