#pragma once

#include <imgui/imgui.h>

#include <string>
#include <vector>
#include <unordered_map>

namespace comet
{
    template<typename ScriptType, typename T>
    using GetterFuncType = T& (ScriptType::*)();

    template<typename ScriptType, typename T>
    using ConstGetterFuncType = const T& (ScriptType::*)() const;

    template<typename ScriptType, typename T>
    using SetterFuncType = void (ScriptType::*)(const T& value);

    template<typename ScriptType, typename T>
    struct NativeScriptPropertyAccessors
    {
        GetterFuncType<ScriptType, T>       getterFunc{nullptr};
        ConstGetterFuncType<ScriptType, T>  constGetterFunc{nullptr};
        SetterFuncType<ScriptType, T>       setterFunc{nullptr};
    };

    enum class NativeScriptPropertyTypeTag
    {
        FLOAT = 0,
        VEC3
    };
    
    struct NativeScriptPropertyDesc
    {
        std::string name;
        NativeScriptPropertyTypeTag type;
        uint32_t index;
    };
    
    struct PropertiesExposerInterface
    {
        virtual std::vector<std::string> getPropertiesName() const = 0;
        virtual void render(const std::string& propertyName) = 0;
    };

    template<typename S>
    struct PropertiesExposer : public PropertiesExposerInterface
    {
        PropertiesExposer(S& ns) : nativeScript(ns) {}

        virtual std::vector<std::string> getPropertiesName() const override
        {
            std::vector<std::string> names;
            std::transform(m_properties.begin(), m_properties.end(), std::back_inserter(names),
                [](const std::pair<std::string, NativeScriptPropertyDesc>& kv) { return kv.second.name; }
            );

            return names;
        }

        virtual void render(const std::string& propertyName) override
        {
            auto propertyDesc = m_properties.find(propertyName);
            if (propertyDesc != m_properties.end())
            {
                auto getterIndex = propertyDesc->second.index;

                ImGui::PushID(propertyName.c_str());

                switch (propertyDesc->second.type)
                {
                    case NativeScriptPropertyTypeTag::FLOAT:
                    {
                        auto accessors = m_floatAccessors[getterIndex];
                        auto getter = accessors.getterFunc;
                        auto constGetter = accessors.constGetterFunc;

                        if (getter)
                        {
                            auto& value = (nativeScript.*getter)();
                            ImGui::TableNextColumn();
                            ImGui::DragFloat("##getterFloat", &value, 0.0f, 0.0f, 0.0f, "%.5f");
                        }
                        else if (constGetter)
                        {
                            auto setter = accessors.setterFunc;

                            // Read / Write Property
                            if (setter)
                            {
                                auto value = (nativeScript.*constGetter)();
                                ImGui::TableNextColumn();
                                ImGui::DragFloat("##setterFloat", &value, 0.001f, -5.0f, 5.0f, "%.5f");
                                (nativeScript.*setter)(value);
                            }
                            // Read only Property
                            else
                            {
                                auto& value = (nativeScript.*getter)();
                                ImGui::TableNextColumn();
                                ImGui::Text("%.5f", value);                                
                            }
                        }
                        break;
                    }

                    case NativeScriptPropertyTypeTag::VEC3:
                    {
                        auto accessors = m_vec3Accessors[getterIndex];
                        auto getter = accessors.getterFunc;
                        auto constGetter = accessors.constGetterFunc;

                        if (getter)
                        {
                            auto& value = (nativeScript.*getter)();
                            ImGui::TableNextColumn();
                            ImGui::DragFloat3("##getterVec3", &value[0], 0.001f, -5.0f, 5.0f, "%.5f");
                        }
                        else if (constGetter)
                        {
                            auto setter = accessors.setterFunc;

                            // Read / Write Property
                            if (setter)
                            {
                                auto value = (nativeScript.*constGetter)();
                                ImGui::TableNextColumn();
                                ImGui::DragFloat3("##setterVec3", &value[0], 0.001f, -5.0f, 5.0f, "%.5f");
                                (nativeScript.*setter)(value);
                            }
                            // Read only Property
                            else
                            {
                                auto& value = (nativeScript.*getter)();
                                ImGui::TableNextColumn();
                                ImGui::Text("%.5f", value.x); ImGui::SameLine();
                                ImGui::Text("%.5f", value.y); ImGui::SameLine();
                                ImGui::Text("%.5f", value.z);
                            }
                        }
                        break;
                    }
                }
                ImGui::PopID();
            }
        }

        void add(const std::string& name, NativeScriptPropertyAccessors<S, float> accessors)
        {
            m_floatAccessors.push_back(accessors);
            uint32_t index = m_floatAccessors.size() - 1;
            m_properties[name] = {name, NativeScriptPropertyTypeTag::FLOAT, index};
        }

        void add(const std::string& name, NativeScriptPropertyAccessors<S, glm::vec3> accessors)
        {
            m_vec3Accessors.push_back(accessors);
            uint32_t index = m_vec3Accessors.size() - 1;
            m_properties[name] = {name, NativeScriptPropertyTypeTag::VEC3, index};
        }

    private:
        S& nativeScript;
        std::unordered_map<std::string, NativeScriptPropertyDesc> m_properties;
        std::vector<NativeScriptPropertyAccessors<S, float>> m_floatAccessors;
        std::vector<NativeScriptPropertyAccessors<S, glm::vec3>> m_vec3Accessors;
    };

} // namespace comet
