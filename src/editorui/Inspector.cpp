#include "Inspector.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <filesystem>

#include "../Game.h"

Inspector::Inspector()
{ }

void Inspector::draw(Game* game)
{
    ImGui::Begin("Inspector");

    if (game->selectedObject != nullptr) {
        ImGui::InputText("Name", &game->selectedObject->name[0], game->selectedObject->name.size() + 1);

        ImGui::SameLine();

        const char* visibleTxt = game->selectedObject->bIsVisible ? "O" : "-";
        if (ImGui::Button(visibleTxt, ImVec2(20, 20))) {
            game->selectedObject->bIsVisible = !game->selectedObject->bIsVisible;
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("Change Visibility");
        }

        if (ImGui::Button("Delete", ImVec2(60, 20))) {
            game->delete_object(game->selectedObject);
        }

        if (ImGui::CollapsingHeader("Transform")) {
            ImGui::DragFloat3("Position", &game->selectedObject->transform.position[0], 0.1f);
            ImGui::DragFloat3("Rotation", &game->selectedObject->transform.eulerAngles[0], 0.1f);
            ImGui::DragFloat3("Scale", &game->selectedObject->transform.scale[0], 0.1f);
        }

        if (ImGui::CollapsingHeader("Rendering")) {
            ImGui::Text("Texture : ");
            ImGui::SameLine();
            if (ImGui::ImageButton("Texture", game->selectedObject->texture->textureObject, ImVec2(50, 50))) { 
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

                        auto sObj = game->selectedObject;
                        sObj->change_texture(texture);
                    }
                }

                ImGui::EndPopup();
            }

            ImGui::Text("Model : ");
            ImGui::SameLine();

            if (ImGui::ImageButton("Model", game->defaultIcon->textureObject, ImVec2(50, 50))) {
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

                        auto sObj = game->selectedObject;
                        sObj->change_mesh(mesh);
                    }
                }

                ImGui::EndPopup();
            }
        }
    }
    ImGui::End();
}
