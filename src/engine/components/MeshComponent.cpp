#include "MeshComponent.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <filesystem>

#include "../../Game.h"

void MeshComponent::draw_inspector_widget() {
    if (ImGui::CollapsingHeader("Rendering")) {
        ImGui::Text("Texture : ");
        ImGui::SameLine();
        if (ImGui::ImageButton("Texture", gameInstance->selectedObject->texture->textureObject, ImVec2(50, 50))) {
            ImGui::OpenPopup("TexturePopUp");
        }

        ImGui::SetNextWindowSize(ImVec2(200, 150));
        if (ImGui::BeginPopup("TexturePopUp")) {
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
                    Texture* texture = TextureLoader::get_instance().get_texture(item.c_str());

                    auto sObj = gameInstance->selectedObject;

                    if (sObj->mesh == nullptr) return;

                    sObj->change_texture(texture);
                }
            }

            ImGui::EndPopup();
        }

        ImGui::Text("Model : ");
        ImGui::SameLine();

        if (ImGui::ImageButton("Model", gameInstance->defaultIcon->textureObject, ImVec2(50, 50))) {
            ImGui::OpenPopup("ModelPopUp");
        }

        ImGui::SetNextWindowSize(ImVec2(200, 150));
        if (ImGui::BeginPopup("ModelPopUp")) {
            std::vector<std::string> items;
            std::string filePath = "res/objmodels/";
            std::filesystem::path folderPath = std::filesystem::current_path() / filePath;

            for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
                if (entry.is_regular_file()) {
                    std::string modelName = entry.path().stem().string();
                    items.push_back(modelName);
                }
            }

            ImGui::Text("Files found : %s", std::to_string(items.size()).c_str());
            for (auto item : items) {
                if (ImGui::Selectable(item.c_str())) {
                    Mesh* mesh = OBJLoader::get_instance().get_mesh(item.c_str());

                    auto sObj = gameInstance->selectedObject;
                    sObj->change_mesh(mesh);
                }
            }

            ImGui::EndPopup();
        }
    }
}

void MeshComponent::update() {
}
