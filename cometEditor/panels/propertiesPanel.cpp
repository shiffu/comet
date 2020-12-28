#include "propertiesPanel.h"
#include <comet/resourceManager.h>
#include <comet/materialRegistry.h>
#include <comet/phongMaterial.h>
#include <comet/staticMesh.h>

#include <imgui/imgui.h>

#include <string>
#include <utility>
#include <tuple>

namespace comet
{
    static ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterV;
    static ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;

    void PropertiesPanel::onDraw(Scene& activeScene, Entity& selectedEntity)
    {
        ImGui::Begin("Properties");
        static const char* addCompText = "Add Component...";
        auto textWidth = ImGui::CalcTextSize(addCompText).x;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 10.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 10.0f));
        ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.5f - (textWidth * 0.5f));
        if (ImGui::Button(addCompText))
        {
            ImGui::OpenPopup("addComponentPopup");
        }
        ImGui::Separator();
        ImGui::PopStyleVar(2);

        std::pair<const char*, const char*> predefinedMeshes[]  = {
            {"Cube", "cube.obj"},
            {"Cylinder", "cylinder.obj"},
            {"Torus", "torus.obj"},
            {"Scooter", "scooterBlender.obj"},
            {"Dragon", "dragon.obj"}
        };

        if (ImGui::BeginPopup("addComponentPopup"))
        {
            if (!selectedEntity.hasComponent<MeshComponent>())
            {
                if (ImGui::BeginMenu("Mesh"))
                {
                    for(auto& predefineMesh : predefinedMeshes)
                    {
                        if (ImGui::MenuItem(predefineMesh.first))
                        {
                            auto material = MaterialRegistry::getInstance().createMaterial<PhongMaterial>();
                            auto meshHandler = ResourceManager::getInstance().loadStaticMesh(predefineMesh.second);
                            selectedEntity.addComponent<MeshComponent>(meshHandler.resourceId, material->getTypeHash(), material->getInstanceId());
                            activeScene.prepare();
                        }
                    }
                    ImGui::EndMenu();
                }
            }
            ImGui::Selectable("Light");
            ImGui::EndPopup();
        }

        if (selectedEntity.hasComponent<NameComponent>())
            drawNameComponent(selectedEntity);

        if (selectedEntity.hasComponent<TransformComponent>())
            drawTransformComponent(selectedEntity);

        if (selectedEntity.hasComponent<MeshComponent>())
            drawMeshComponent(selectedEntity);

        ImGui::End();
    }

    void PropertiesPanel::drawNameComponent(Entity& entity)
    {
        if (ImGui::CollapsingHeader("Name", headerFlags))
        {
            if (ImGui::BeginTable("##Name", 2, tableFlags))
            {
                static constexpr float firstColWidth = 120.0f;
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, firstColWidth);

                ImGui::TableNextRow();
                constexpr size_t size = 128;
                static char buffer[size];
                auto& name = entity.getComponent<NameComponent>().name;
                memset(buffer, 0, size);
                strncpy(buffer, name.c_str(), size);

                ImGui::TableNextColumn();
                ImGui::Text("Name");
                ImGui::TableNextColumn();
                ImGui::InputText("##Name", buffer, size);
                name = buffer;
            }
            ImGui::EndTable();
        }
    }

    void PropertiesPanel::drawVec3(const char* label, glm::vec3& vec, float firstColWidth, float resetValue /* = 0.0f*/)
    {
        static constexpr float speed = 0.07f;
        auto boldFont = ImGui::GetIO().Fonts->Fonts[1];
        
        ImGui::PushID(label);
        ImGui::TableNextColumn();
        ImGui::Text(label);
        
        std::tuple<const char*, const char*, float*, glm::vec3> columns[]  = {
            {"X", "##x", &vec.x, {1.0f, 0.0f, 0.0f}},
            {"Y", "##y", &vec.y, {0.0f, 1.0f, 0.0f}},
            {"Z", "##z", &vec.z, {0.0f, 0.0f, 1.0f}}
        };

        for (auto& [label, id, value, colorSelector] : columns)
        {
            ImGui::TableNextColumn();
            ImGui::PushFont(boldFont);
            auto buttonColor = colorSelector * 0.7f;
            auto buttonHoveredColor = colorSelector * 0.85f;
            auto buttonActiveColor = colorSelector * 1.0f;
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHoveredColor.x, buttonHoveredColor.y, buttonHoveredColor.z, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActiveColor.x, buttonActiveColor.y, buttonActiveColor.z, 1.0f));
            if (ImGui::Button(label))
            {
                *value = resetValue;
            }
            ImGui::PopFont();
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::DragFloat(id, value, speed, -FLT_MAX, FLT_MAX, "%.2f");
        }
        ImGui::PopID();
    }

    void PropertiesPanel::drawTransformComponent(Entity& entity)
    {
        if (ImGui::CollapsingHeader("Transform", headerFlags))
        {
            if (ImGui::BeginTable("##tableTransformComponent", 4, tableFlags))
            {
                static constexpr float firstColWidth = 120.0f;
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, firstColWidth);
                auto& transformComponent = entity.getComponent<TransformComponent>();
                
                auto& translation = transformComponent.translation;
                drawVec3("Translation", translation, firstColWidth);

                auto& rotation = transformComponent.rotation;
                rotation.x = glm::degrees(rotation.x);
                rotation.y = glm::degrees(rotation.y);
                rotation.z = glm::degrees(rotation.z);
                drawVec3("Rotation", rotation, firstColWidth);
                rotation.x = glm::radians(rotation.x);
                rotation.y = glm::radians(rotation.y);
                rotation.z = glm::radians(rotation.z);

                auto& scale = transformComponent.scale;
                drawVec3("Scale", scale, firstColWidth, 1.0f);
            }
            ImGui::EndTable();
        }
    }

    void PropertiesPanel::drawMeshComponent(Entity& entity)
    {
        headerFlags |= ImGuiTreeNodeFlags_AllowItemOverlap;
        bool expand = ImGui::CollapsingHeader("Mesh", headerFlags);

        static const char* deleteText = "X";
        auto textWidth = ImGui::CalcTextSize(deleteText).x;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.30f, 0.50f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.45f, 0.65f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.30f, 0.50f, 0.70f, 1.00f));

        ImGui::SameLine(ImGui::GetContentRegionAvail().x - textWidth);
        auto clicked = ImGui::Button(deleteText);
        ImGui::PopStyleColor(3);
        if (clicked)
        {
            entity.removeComponent<MeshComponent>();
            entity.getScene()->prepare();
            return;
        }

        if (expand)
        {
            if (ImGui::BeginTable("##tableMeshComponent", 2, tableFlags))
            {
                static constexpr float firstColWidth = 145.0f;
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, firstColWidth);
                auto& meshComponent = entity.getComponent<MeshComponent>();

                ImGui::TableNextColumn();
                ImGui::Text("Mesh Type ID");
                ImGui::TableNextColumn();
                ImGui::Text("%d", meshComponent.meshTypeId);

                auto staticMesh = ResourceManager::getInstance().getStaticMesh(meshComponent.meshTypeId).resource;
                auto material = (PhongMaterial*)MaterialRegistry::getInstance().getMaterial(meshComponent.materialTypeId, meshComponent.materialInstanceId);

                ImGui::TableNextColumn();
                ImGui::Text("Mesh Type");
                ImGui::TableNextColumn();
                ImGui::Text("%s", staticMesh->getName().c_str());

                ImGui::TableNextColumn();
                ImGui::Text("Material Type ID");
                ImGui::TableNextColumn();
                ImGui::Text("%d", meshComponent.materialTypeId);

                ImGui::TableNextColumn();
                ImGui::Text("Material Instance ID");
                ImGui::TableNextColumn();
                ImGui::Text("%d", meshComponent.materialInstanceId);

                ImGui::TableNextColumn();
                ImGui::Text("Material Name");
                ImGui::TableNextColumn();
                ImGui::Text("%s", material->getName().c_str());

                ImGui::TableNextColumn();
                ImGui::Text("Material Diffuse");
                ImGui::TableNextColumn();
                auto& diffuseColor = material->getDiffuse();
                float diffuseData[] = {diffuseColor.x, diffuseColor.y, diffuseColor.z};
                ImGui::ColorEdit3("##diffuse", diffuseData);
                material->setDiffuse(glm::vec3(diffuseData[0], diffuseData[1], diffuseData[2]));

                ImGui::TableNextColumn();
                ImGui::Text("Material Specular");
                ImGui::TableNextColumn();
                auto& specularColor = material->getSpecular();
                float specularData[] = {specularColor.x, specularColor.y, specularColor.z};
                ImGui::ColorEdit3("##specular", specularData);
                material->setSpecular(glm::vec3(specularData[0], specularData[1], specularData[2]));

                ImGui::TableNextColumn();
                ImGui::Text("Material Shininess");
                ImGui::TableNextColumn();
                auto shininess = material->getShininess();
                shininess = std::max(1.0f, shininess);
                ImGui::DragFloat("##shininess", &shininess, 0.5f, 1.0f, 200.0f);
                material->setShininess(shininess);
            }
            ImGui::EndTable();
        }
    }
}
