#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace comet
{

    class ItemDisabler
    {
    public:
        ItemDisabler(bool disable)
            : m_disable(disable)
        {
            if (m_disable)
            {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }
        }

        ~ItemDisabler()
        {
            if (m_disable)
            {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }            
        }

    private:
        bool m_disable;
    };
    
} // namespace comet
