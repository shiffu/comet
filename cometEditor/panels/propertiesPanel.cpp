#include "propertiesPanel.h"

#include <comet/components.h>
#include <comet/resourceManager.h>
#include <comet/materialRegistry.h>
#include <comet/staticMesh.h>
#include <comet/propertiesExposer.h>
#include <comet/camera.h>

#include <core/imguiUtils.h>

#include <string>
#include <sstream>
#include <utility>
#include <tuple>

namespace comet
{
    static ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersInnerV;
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

        if (ImGui::BeginPopup("addComponentPopup"))
        {
            // Mesh Component
            if (!selectedEntity.hasComponent<MeshComponent>())
            {
                if (ImGui::BeginMenu("Mesh"))
                {
                    std::pair<const char*, const char*> predefinedMeshes[]  = {
                        {"Cube", "cube.obj"},
                        {"Cylinder", "cylinder.obj"},
                        {"Torus", "torus.obj"},
                        {"Scooter", "scooterBlender.obj"},
                        {"Dragon", "dragon.obj"}
                    };

                    for(auto& predefineMesh : predefinedMeshes)
                    {
                        if (ImGui::MenuItem(predefineMesh.first))
                        {
                            auto meshHandler = ResourceManager::getInstance().loadStaticMesh(predefineMesh.second);
                            selectedEntity.addComponent<MeshComponent>(meshHandler.resourceId);
                            activeScene.reload();
                        }
                    }
                    ImGui::EndMenu();
                }
            }

            // Material Component
            if (!selectedEntity.hasComponent<MaterialComponent>())
            {
                if (ImGui::BeginMenu("Material"))
                {
                    if (ImGui::BeginMenu("existing"))
                    {
                        auto& materialInstances = MaterialRegistry::getInstance().getMaterialInstances();

                        for(auto& materialInstance : materialInstances)
                        {
                            // First instance is the Comet default one
                            if (materialInstance->getInstanceId() == 0)
                            {
                                continue;
                            }
                            if (ImGui::MenuItem(materialInstance->getName().c_str()))
                            {
                                selectedEntity.addComponent<MaterialComponent>(materialInstance->getInstanceId());
                                activeScene.reload();
                            }
                        }
                        ImGui::EndMenu();
                    }

                    if (ImGui::MenuItem("New"))
                    {
                        auto count = MaterialRegistry::getInstance().getMaterialInstanceCount();
                        std::stringstream ss;
                        ss << "new" << count;
                        auto newMaterialInstance = MaterialRegistry::getInstance().createMaterialInstance(ss.str().c_str());
                        newMaterialInstance->setDiffuse({0.8f, 0.8f, 0.8f});
                        newMaterialInstance->setSpecular({0.8f, 0.8f, 0.8f});
                        newMaterialInstance->setShininess(1.1f);

                        selectedEntity.addComponent<MaterialComponent>(newMaterialInstance->getInstanceId());
                        activeScene.reload();
                    }

                    ImGui::EndMenu();
                }
            }

            // Camera Component
            if (!selectedEntity.hasComponent<CameraComponent>())
            {
                if (ImGui::MenuItem("Camera"))
                {
                    selectedEntity.addComponent<CameraComponent>();
                }
            }            

            // NativeScript Component
            if (!selectedEntity.hasComponent<NativeScriptComponent>())
            {
                if (ImGui::BeginMenu("NativeScript"))
                {
                    if (ImGui::MenuItem("New"))
                    {
                        selectedEntity.addComponent<NativeScriptComponent>();
                    }
                    ImGui::EndMenu();
                }
            }

            ImGui::EndPopup();
        }

        if (selectedEntity.hasComponent<NameComponent>())
            drawNameComponent(selectedEntity);

        if (selectedEntity.hasComponent<TransformComponent>())
            drawTransformComponent(selectedEntity);

        if (selectedEntity.hasComponent<MeshComponent>())
            drawMeshComponent(selectedEntity);

        if (selectedEntity.hasComponent<MaterialComponent>())
            drawMaterialComponent(selectedEntity);

        if (selectedEntity.hasComponent<CameraComponent>())
            drawCameraComponent(selectedEntity);

        if (selectedEntity.hasComponent<NativeScriptComponent>())
            drawNativeScriptComponent(selectedEntity);

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

    template<typename T, typename FUNC>
    void drawRemovableComponent(const char* title, Entity& entity, FUNC callback)
    {
        headerFlags |= ImGuiTreeNodeFlags_AllowItemOverlap;
        bool expand = ImGui::CollapsingHeader(title, headerFlags);

        ImGui::PushID(title);

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
            entity.removeComponent<T>();
            entity.getScene()->reload();
            ImGui::PopID();
            return;
        }

        if (expand)
        {
            callback(entity);
        }
        ImGui::PopID();
    }

    void PropertiesPanel::drawMeshComponent(Entity& entity)
    {
        drawRemovableComponent<MeshComponent>("Mesh", entity, [](Entity& entity)
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
                ImGui::TableNextColumn();
                ImGui::Text("Mesh Type");
                ImGui::TableNextColumn();
                ImGui::Text("%s", staticMesh->getName().c_str());
            }
            ImGui::EndTable();
        });
    }

    void PropertiesPanel::drawMaterialComponent(Entity& entity)
    {
        drawRemovableComponent<MaterialComponent>("Material", entity, [](Entity& entity)
        {
            if (ImGui::BeginTable("##tableComponent", 2, tableFlags))
            {
                static constexpr float firstColWidth = 145.0f;
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, firstColWidth);
                auto& materialComponent = entity.getComponent<MaterialComponent>();
                auto material = MaterialRegistry::getInstance().getMaterialInstance(materialComponent.materialInstanceId);

                ImGui::TableNextColumn();
                ImGui::Text("Material Instance ID");
                ImGui::TableNextColumn();
                ImGui::Text("%d", materialComponent.materialInstanceId);

                ImGui::TableNextColumn();
                ImGui::Text("Material Name");
                ImGui::TableNextColumn();
                constexpr size_t size = 128;
                static char buffer[size];
                auto& materialName = material->getName();
                memset(buffer, 0, size);
                strncpy(buffer, materialName.c_str(), size);
                ImGui::InputText("##Name", buffer, size);
                materialName = buffer;

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
        });
    }

    void PropertiesPanel::drawCameraComponent(Entity& entity)
    {
        drawRemovableComponent<CameraComponent>("Camera", entity, [](Entity& entity)
        {
            if (ImGui::BeginTable("##tableComponent", 2, tableFlags))
            {
                static constexpr float firstColWidth = 145.0f;
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, firstColWidth);
                auto& cameraComponent = entity.getComponent<CameraComponent>();
                auto& camera = cameraComponent.camera;

                // Primary?
                // static bool primaryCamera = true;
                ImGui::TableNextColumn();
                ImGui::Text("Is Primary");
                ImGui::TableNextColumn();
                ImGui::Checkbox("##isPrimary", &cameraComponent.isPrimary);

                // Projection Type
                static int projectionType = Camera::ProjectionType::PERSPECTIVE;
                ImGui::TableNextColumn();
                ImGui::Text("Projection");
                ImGui::TableNextColumn();
                ImGui::RadioButton("Perspective", &projectionType, Camera::ProjectionType::PERSPECTIVE); ImGui::SameLine();
                ImGui::RadioButton("Orthographic", &projectionType, Camera::ProjectionType::ORTHOGRAPHIC); ImGui::SameLine();

                if(projectionType == Camera::ProjectionType::PERSPECTIVE)
                {
                    ImGui::TableNextColumn();
                    ImGui::Text("FOV");
                    ImGui::TableNextColumn();
                    auto fov = glm::degrees(camera.getFOV());
                    ImGui::DragFloat("##fov", &fov, 1.0f, 0.0f, 90.0f);
                    camera.setFOV(glm::radians(fov));
                }

                // Zoom
                ImGui::TableNextColumn();
                ImGui::Text("Zoom");
                ImGui::TableNextColumn();
                auto zoom = camera.getZoom();
                ImGui::DragFloat("##zoom", &zoom, 0.1f, 0.1f, 100.0f);
                camera.setZoom(zoom);

                // Aspect Ratio
                ImGui::TableNextColumn();
                ImGui::Text("Aspect ratio");
                ImGui::TableNextColumn();
                auto aspectRatio = camera.getAspectRatio();
                ImGui::DragFloat("##aspectRatio", &aspectRatio, 0.1f, 0.0f, 10.0f);
                camera.setAspectRatio(aspectRatio);

                // Near Plan
                ImGui::TableNextColumn();
                ImGui::Text("Near Plan");
                ImGui::TableNextColumn();
                auto nearPlan = camera.getNear();
                ImGui::DragFloat("##nearPlan", &nearPlan, 0.1f, 0.1f, 10.0f);
                camera.setNear(nearPlan);

                // Far Plan
                ImGui::TableNextColumn();
                ImGui::Text("Far Plan");
                ImGui::TableNextColumn();
                auto farPlan = camera.getFar();
                ImGui::DragFloat("##farPlan", &farPlan, 10.0f, 100.0f, 4000.0f);
                camera.setFar(farPlan);
            }
            ImGui::EndTable();
        });
    }    

    void PropertiesPanel::drawNativeScriptProperties(PropertiesExposerInterface* propertiesExposer, const std::string& propertyName)
    {
        auto valueWrapper = propertiesExposer->getPropertyValue(propertyName);
        if (valueWrapper)
        {
            ImGui::PushID(propertyName.c_str());
            switch (valueWrapper->typeTag)
            {
                case NativeScriptPropertyTypeTag::FLOAT:
                {
                    auto value = std::get<NativeScriptPropertyTypeTag::FLOAT>(valueWrapper->value);

                    // Read / Write Property
                    if (propertiesExposer->hasSetter(propertyName))
                    {
                        ImGui::TableNextColumn();
                        ImGui::DragFloat("##setterFloat", &value, 0.001f, -5.0f, 5.0f, "%.5f");
                        valueWrapper->value = value;
                        propertiesExposer->setPropertyValue(propertyName, *valueWrapper);
                    }
                    // Read only Property
                    else
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("%.5f", value);                                
                    }
                    break;
                }

                case NativeScriptPropertyTypeTag::VEC3:
                {
                    auto value = std::get<NativeScriptPropertyTypeTag::VEC3>(valueWrapper->value);

                    // Read / Write Property
                    if (propertiesExposer->hasSetter(propertyName))
                    {
                        ImGui::TableNextColumn();
                        ImGui::DragFloat3("##setterVec3", &value[0], 0.001f, -5.0f, 5.0f, "%.5f");
                        valueWrapper->value = value;
                        propertiesExposer->setPropertyValue(propertyName, *valueWrapper);
                    }
                    // Read only Property
                    else
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("%.5f", value.x); ImGui::SameLine();
                        ImGui::Text("%.5f", value.y); ImGui::SameLine();
                        ImGui::Text("%.5f", value.z);
                    }
                    break;
                }
            }
            ImGui::PopID();
        }
    }

    void PropertiesPanel::drawNativeScriptComponent(Entity& entity)
    {
        drawRemovableComponent<NativeScriptComponent>("NativeScript", entity, [](Entity& entity)
        {
            auto& scriptComponent = entity.getComponent<NativeScriptComponent>();
            bool nativeScriptLibPathUpdated{false};
            bool selectedScriptUpdated{false};

            if (ImGui::BeginTable("##tableNativeScript", 2, tableFlags))
            {
                static constexpr float firstColWidth = 120.0f;
                ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, firstColWidth);

                // Script Lib filepath
                constexpr size_t size = 256;
                static char buffer[size];
                auto& scriptLibFilepath = scriptComponent.scriptLibFilepath;
                memset(buffer, 0, size);
                strncpy(buffer, scriptLibFilepath.c_str(), size);

                ImGui::TableNextColumn();
                ImGui::Text("Script Lib filepath");
                ImGui::TableNextColumn();
                nativeScriptLibPathUpdated = ImGui::InputText("##LibFilepath", buffer, size);
                scriptLibFilepath = buffer;

                // Getting the available exported scripts from the Scrip Lib
                auto isGetScriptsFuncBound = scriptComponent.runtimeBindGetScripts(scriptComponent.scriptLibFilepath.c_str());
                std::vector<const char*> scriptsName{"<No Script>"};
                if (isGetScriptsFuncBound)
                {
                    scriptsName = scriptComponent.getScripts();
                }

                static int scriptIndex = 0; 
                const char* currentSelectedScript = scriptsName[scriptIndex];
                if (ImGui::BeginCombo("##scriptCombo", currentSelectedScript))
                {
                    for (int i = 0; i < scriptsName.size(); i++)
                    {
                        const bool isSelected = (scriptIndex == i);
                        if (ImGui::Selectable(scriptsName[i], isSelected) && scriptIndex != i)
                        {
                            scriptIndex = i;
                            selectedScriptUpdated = true;
                            scriptComponent.selectedScriptName = scriptsName[i];
                        }

                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }

                // Displaying the available and exported properties of the script
                if (scriptComponent.instance)
                {
                    ImGui::PushID(scriptComponent.selectedScriptName.c_str());
                    auto propertiesExposer = scriptComponent.instance->getPropertiesExposer();
                    if (propertiesExposer)
                    {
                        ImGui::TableNextRow();
                        for(auto& propertyName : propertiesExposer->getPropertiesName())
                        {
                            ImGui::TableNextColumn();
                            ImGui::Text("%s", propertyName.c_str());
                            drawNativeScriptProperties(propertiesExposer, propertyName);
                        }
                    }
                    ImGui::PopID();
                }
            }
            ImGui::EndTable();

            // Load Button management (disabled/enabled)
            ImGui::Separator();
            static bool isLoadButtonDisabled = true;
            static bool disableLoadButton = true;

            if (nativeScriptLibPathUpdated || selectedScriptUpdated)
            {
                isLoadButtonDisabled = disableLoadButton = false;
            }

            {
                ItemDisabler buttonDisabler{isLoadButtonDisabled};

                bool isScriptBound = false;
                auto availableWidth = ImGui::GetContentRegionAvail().x;
                ImGui::Indent(availableWidth * 0.2f);
                if ( ImGui::Button("Load", ImVec2(availableWidth * 0.6f, 0.0f)) )
                {
                    // If we already have a script bound, we need to destroy it first
                    if (scriptComponent.instance)
                    {
                        scriptComponent.instance->onDestroy();
                        scriptComponent.destroyScript(&scriptComponent);
                        scriptComponent.instance = nullptr;
                    }

                    isScriptBound = scriptComponent.runtimeBind(scriptComponent.scriptLibFilepath.c_str(), scriptComponent.selectedScriptName.c_str());
                    if (isScriptBound)
                    {
                        entity.getScene()->instantiateNativeScriptComponent(scriptComponent);
                        disableLoadButton = true;
                    }
                }
                ImGui::Unindent();                
            }

            isLoadButtonDisabled = disableLoadButton;
        });
    }    
}
