#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>

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

    enum NativeScriptPropertyTypeTag
    {
        FLOAT = 0,
        VEC3
    };
 
    using PropertyValueType = std::variant<float, glm::vec3>;
    
    struct NativeScriptPropertyDesc
    {
        std::string name;
        NativeScriptPropertyTypeTag type;
        uint32_t index;
    };

    struct PropertyValueContainer
    {
        NativeScriptPropertyTypeTag typeTag;
        PropertyValueType value;
    };

    struct PropertiesExposerInterface
    {
        virtual std::vector<std::string> getPropertiesName() const = 0;

        virtual std::optional<PropertyValueContainer> getPropertyValue(const std::string& propertyName) = 0;
        virtual void setPropertyValue(const std::string& propertyName, PropertyValueContainer valueContainer) = 0;
        virtual bool hasSetter(const std::string& propertyName) = 0;
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

        std::optional<PropertyValueContainer> getPropertyValue(const std::string& propertyName)
        {
            auto propertyDesc = m_properties.find(propertyName);
            if (propertyDesc != m_properties.end())
            {
                auto accessorsIndex = propertyDesc->second.index;

                switch (propertyDesc->second.type)
                {
                    case NativeScriptPropertyTypeTag::FLOAT:
                    {
                        auto accessors = m_floatAccessors[accessorsIndex];
                        auto constGetter = accessors.constGetterFunc;

                        if (constGetter)
                        {
                            PropertyValueContainer valueContainer{NativeScriptPropertyTypeTag::FLOAT};
                            valueContainer.value = (nativeScript.*constGetter)();
                            return valueContainer;
                        }
                    }

                    case NativeScriptPropertyTypeTag::VEC3:
                    {
                        auto accessors = m_vec3Accessors[accessorsIndex];
                        auto constGetter = accessors.constGetterFunc;

                        if (constGetter)
                        {
                            PropertyValueContainer valueContainer{NativeScriptPropertyTypeTag::VEC3};
                            valueContainer.value = (nativeScript.*constGetter)();
                            return valueContainer;
                        }
                    }
                }
            }

            CM_CORE_LOG_ERROR("Property name '{}' does not exist in script", propertyName);
            return {};
        }

        bool hasSetter(const std::string& propertyName)
        {
            auto propertyDesc = m_properties.find(propertyName);
            if (propertyDesc != m_properties.end())
            {
                auto accessorsIndex = propertyDesc->second.index;

                switch (propertyDesc->second.type)
                {
                    case NativeScriptPropertyTypeTag::FLOAT:
                    {
                        auto accessors = m_floatAccessors[accessorsIndex];
                        auto setter = accessors.setterFunc;

                        if (setter)
                        {
                            return true;
                        }
                        break;
                    }

                    case NativeScriptPropertyTypeTag::VEC3:
                    {
                        auto accessors = m_vec3Accessors[accessorsIndex];
                        auto setter = accessors.setterFunc;

                        if (setter)
                        {
                            return true;
                        }
                        break;
                    }
                }
            }

            return false;
        }
        
        void setPropertyValue(const std::string& propertyName, PropertyValueContainer valueContainer)
        {
            auto propertyDesc = m_properties.find(propertyName);
            if (propertyDesc != m_properties.end())
            {
                auto accessorsIndex = propertyDesc->second.index;

                switch (propertyDesc->second.type)
                {
                    case NativeScriptPropertyTypeTag::FLOAT:
                    {
                        auto accessors = m_floatAccessors[accessorsIndex];
                        auto setter = accessors.setterFunc;

                        if (setter)
                        {
                            auto value = std::get<NativeScriptPropertyTypeTag::FLOAT>(valueContainer.value);
                            (nativeScript.*setter)(value);
                        }
                        break;
                    }

                    case NativeScriptPropertyTypeTag::VEC3:
                    {
                        auto accessors = m_vec3Accessors[accessorsIndex];
                        auto setter = accessors.setterFunc;

                        if (setter)
                        {
                            auto value = std::get<NativeScriptPropertyTypeTag::VEC3>(valueContainer.value);
                            (nativeScript.*setter)(value);
                        }
                        break;
                    }
                }
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
