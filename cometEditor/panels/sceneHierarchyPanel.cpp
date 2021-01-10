#include "sceneHierarchyPanel.h"

#include <comet/components.h>

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

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 1.0f));
        auto& registry = activeScene.getRegistry();
        registry.each([&] (auto entityId)
        {
            drawEntity({entityId, &activeScene});
        });
        ImGui::PopStyleVar();

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
        {
            m_selectedEntity = {};
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::drawEntity(Entity entity)
    {
        ImGuiTreeNodeFlags flags = treeNodesFlags;
        bool isSelected = (m_selectedEntity == entity);
        flags |= isSelected ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;

        if (ImGui::BeginTable("##tableDrawEntity", 2, tableFlags))
        {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch | ImGuiTableFlags_Resizable, 4.0f);
            ImGui::TableNextColumn();

            if (isSelected)
            {
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.30f, 0.50f, 0.70f, 1.00f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.30f, 0.50f, 0.70f, 1.00f));
            }

            auto expand = ImGui::TreeNodeEx((void*)(uint64_t)entity.getId(), flags, "%s (id: %d)", entity.getComponent<NameComponent>().name.c_str(), (uint32_t)entity.getId());

            if (ImGui::IsItemClicked())
            {
                m_selectedEntity = entity;
            }

            ImGui::TableNextColumn();
            if (ImGui::Button("..."))
                ImGui::OpenPopup("entityOptionsPopup");

            if (expand)
                ImGui::TreePop();

            if (isSelected)
                ImGui::PopStyleColor(2);

            if (ImGui::BeginPopup("entityOptionsPopup"))
            {
                ImGui::PushID((uint32_t)entity.getId());
                if (ImGui::MenuItem("Delete"))
                {
                    auto activeScene = entity.getScene();
                    if (activeScene)
                    {
                        activeScene->destroyEntity(entity);
                        activeScene->reload();
                    }
                }
                ImGui::PopID();
                ImGui::EndPopup();
            }
        }
        ImGui::EndTable();
    }
}
