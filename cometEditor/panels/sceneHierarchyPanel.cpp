#include "sceneHierarchyPanel.h"

#include <imgui/imgui.h>

namespace comet
{

    static ImGuiTableFlags tableFlags = ImGuiTableFlags_None;
    static ImGuiTreeNodeFlags treeNodesFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed;

    void SceneHierarchyPanel::onDraw(Scene& activeScene)
    {
        ImGui::Begin("Scene Hierarchy");
        static const char* createEntityText = "Create Empty Entity";

        auto textWidth = ImGui::CalcTextSize(createEntityText).x;
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 10.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10.0f, 10.0f));
        ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.5f - (textWidth * 0.5f));
        if (ImGui::Button(createEntityText))
        {
            m_selectedEntity = activeScene.createEntity();
        }
        ImGui::Separator();
        ImGui::PopStyleVar(2);

        auto& registry = activeScene.getRegistry();
        registry.each([&] (auto entityId)
        {
            drawEntity({entityId, &activeScene});
        });

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
        {
            m_selectedEntity = {};
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::drawEntity(Entity entity)
    {
        ImGuiTreeNodeFlags flags = treeNodesFlags;
        flags |= (m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;

        if (ImGui::BeginTable("##tableDrawEntity", 2, tableFlags))
        {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch | ImGuiTableFlags_Resizable, 4.0f);
            ImGui::TableNextColumn();
            if (ImGui::TreeNodeEx((void*)(uint64_t)entity.getId(), flags, "%s (id: %d)", entity.getComponent<NameComponent>().name.c_str(), (uint32_t)entity.getId()))
            {
                ImGui::TreePop();
            }

            if (ImGui::IsItemClicked())
            {
                m_selectedEntity = entity;
            }

            ImGui::TableNextColumn();
            if (ImGui::Button("..."))
                ImGui::OpenPopup("entityOptionsPopup");

            if (ImGui::BeginPopup("entityOptionsPopup"))
            {
                ImGui::PushID((uint32_t)entity.getId());
                if (ImGui::MenuItem("Delete"))
                {
                    auto activeScene = entity.getScene();
                    if (activeScene)
                    {
                        activeScene->destroyEntity(entity);
                        activeScene->prepare();
                    }
                }
                ImGui::PopID();
                ImGui::EndPopup();
            }
        }
        ImGui::EndTable();
    }
}
