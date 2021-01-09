#pragma once

#include <comet/vertex.h>
#include <comet/nativeScript.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <dlfcn.h>
#include <sstream>

namespace comet
{
    struct TransformComponent
    {
        TransformComponent() = default;
        TransformComponent(const glm::vec3& pos)
        {
            translation = pos;
        }
        
        glm::mat4 getTransform() const
        {
            glm::mat4 identity(1.0f);
            auto rot = glm::rotate(identity, rotation.x, glm::vec3(1, 0, 0))
                * glm::rotate(identity, rotation.y, glm::vec3(0, 1, 0))
                * glm::rotate(identity, rotation.z, glm::vec3(0, 0, 1));

            return glm::translate(identity, translation)
                * rot
                * glm::scale(identity, scale);
        }

        glm::vec3 translation{0.0f};
        glm::vec3 scale{1.0f};
        glm::vec3 rotation{0.0f};
    };
    
    struct NameComponent
    {
        NameComponent() = default;
        NameComponent(const char* name) : name(name) {};
        NameComponent(const std::string& name) : name(name) {};

        std::string name{"Unnamed"};
    };
    
    struct TagComponent
    {
        TagComponent() = default;
    };
    
    struct PlayerTagComponent : public TagComponent
    {
        PlayerTagComponent() = default;
    };
    
    struct EnemyTagComponent : public TagComponent
    {
        EnemyTagComponent() = default;
    };
    
    struct MeshComponent
    {
        MeshComponent() = default;
        MeshComponent(uint32_t meshTypeId) : meshTypeId(meshTypeId) {};

        uint32_t meshTypeId{0};
    };
    
    struct MaterialComponent
    {
        MaterialComponent() = default;
        MaterialComponent(uint32_t materialInstanceId)
            : materialInstanceId(materialInstanceId) {};

        uint32_t materialInstanceId{0};
    };

    struct NativeScriptComponent
    {
        using InstantiateFuncType = NativeScript* (*)();
        using DestroyFuncType = void (*)(NativeScriptComponent*);
        using GetScriptsFuncType = std::vector<const char*> (*)();

        // Data Members
        NativeScript* instance{nullptr};
        //TODO: remove this once we have an Asset Manager UI
        std::string scriptLibFilepath = "../testScriptsLib/libtestScriptsLib.so";
        std::string selectedScriptName;
        std::vector<const char*> scriptsName;
        InstantiateFuncType instantiateScript{nullptr};
        DestroyFuncType destroyScript{nullptr};
        GetScriptsFuncType getScripts{nullptr};

        // Functions Members
        NativeScriptComponent(const NativeScriptComponent&) = delete;
        void operator=(const NativeScriptComponent&) = delete;

        NativeScriptComponent(NativeScriptComponent&& other)
            : instance(std::move(other.instance)),
            scriptLibFilepath(std::move(other.scriptLibFilepath)),
            selectedScriptName(std::move(selectedScriptName)),
            scriptsName(std::move(other.scriptsName)),
            instantiateScript(std::move(other.instantiateScript)),
            destroyScript(std::move(other.destroyScript)),
            getScripts(std::move(other.getScripts)),
            m_dllHandle(std::move(other.m_dllHandle)) 
        {
            other.instance = nullptr;
            other.instantiateScript = nullptr;
            other.destroyScript = nullptr;
            other.getScripts = nullptr;
            other.m_dllHandle = nullptr;
        }

        NativeScriptComponent& operator=(NativeScriptComponent&& other)
        {
            if (&other == this)
            {
                return *this;
            }
            if (instance)
            {
                delete instance;
                instance = nullptr;
            }
            if (m_dllHandle)
            {
                dlclose(m_dllHandle);
            }

            instance = std::move(other.instance);
            scriptLibFilepath = std::move(other.scriptLibFilepath);
            selectedScriptName = std::move(other.selectedScriptName);
            scriptsName = std::move(other.scriptsName);
            instantiateScript = std::move(other.instantiateScript);
            destroyScript = std::move(other.destroyScript);
            getScripts = std::move(other.getScripts);
            m_dllHandle = std::move(other.m_dllHandle);

            other.instance = nullptr;
            other.instantiateScript = nullptr;
            other.destroyScript = nullptr;
            other.getScripts = nullptr;
            other.m_dllHandle = nullptr;

            return *this;
        }

        NativeScriptComponent() = default;
        ~NativeScriptComponent()
        {
            if (instance)
            {
                instance->onDestroy();
                destroyScript(this);
            }

            if (m_dllHandle)
            {
                dlclose(m_dllHandle);
            }
        }

        template<typename T>
        void bind()
        {  
            instantiateScript = []() { return static_cast<NativeScript*>(new T()); };
            destroyScript = [](NativeScriptComponent* comp)
            {
                if (comp->instance)
                {
                    delete comp->instance;
                    comp->instance = nullptr;
                }
            };
        }

        bool runtimeBindGetScripts(const char* scriptLibFilepath)
        {
            if (!m_dllHandle)
            {
                m_dllHandle = dlopen(scriptLibFilepath, RTLD_LAZY);
            }

            if (!m_dllHandle)
            {
                CM_CORE_LOG_WARN("Unable to load dynamic library '{}'", scriptLibFilepath);
                return false;
            }
            else
            {
                // Bind the getScripts function (if it exists)
                getScripts = (GetScriptsFuncType)dlsym(m_dllHandle, "getScripts");
                if (getScripts == nullptr)
                {
                    CM_CORE_LOG_DEBUG("The function 'getScripts' has not been exposed by the library '{}'", scriptLibFilepath);
                    return false;
                }
            }

            return true;
        }

        bool runtimeBind(const char* scriptLibFilepath, const char* scriptClassName)
        {
            if (!m_dllHandle)
            {
                m_dllHandle = dlopen(scriptLibFilepath, RTLD_LAZY);
            }

            if (!m_dllHandle)
            {
                CM_CORE_LOG_WARN("Unable to load dynamic library '{}'", scriptLibFilepath);
                return false;
            }
            else
            {
                scriptLibFilepath = scriptLibFilepath;

                // Bind the instantiate function
                {
                    std::stringstream ss;
                    ss << "instantiate" << scriptClassName;
                    instantiateScript = (InstantiateFuncType)dlsym(m_dllHandle, ss.str().c_str());
                    if (instantiateScript == nullptr)
                    {
                        CM_CORE_LOG_ERROR("The function '{}' for the script '{}' has not been exposed", ss.str().c_str(), selectedScriptName);
                        dlclose(m_dllHandle);
                        return false;
                    }
                }

                // Bind the destroy function
                {
                    std::stringstream ss;
                    ss << "destroy" << scriptClassName;
                    destroyScript = (DestroyFuncType)dlsym(m_dllHandle, ss.str().c_str());
                    if (destroyScript == nullptr)
                    {
                        CM_CORE_LOG_ERROR("The function '{}' for the script '{}' has not been exposed", ss.str().c_str(), selectedScriptName);
                        dlclose(m_dllHandle);

                        // Do not allow to instantiate a Script if we don't know out to destroy it
                        instantiateScript = nullptr;

                        return false;
                    }
                }
            }

            selectedScriptName = scriptClassName;
            return true;
        }
    
    private:
        void* m_dllHandle{nullptr};
    };

} // namespace comet
