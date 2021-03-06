#pragma once

#include <comet/comet.h>

#include <glm/glm.hpp>

#include <vector>

using namespace comet;


class RotateScript : public NativeScript
{
public:
    RotateScript() : m_propExposer(*this)
    {
        CM_CORE_LOG_DEBUG("RotateScript::RotateScript()");
    }

    virtual ~RotateScript()
    {
        CM_CORE_LOG_DEBUG("RotateScript::~RotateScript()");
    }

    virtual void onCreate() override
    {
        CM_LOG_DEBUG("RotateScript::onCreate()");
        setPropertiesExposer(&m_propExposer);
        m_propExposer.add("Speed", {nullptr, &RotateScript::getSpeed, &RotateScript::setSpeed});
    }

    virtual void onDestroy() override
    {
        CM_LOG_DEBUG("RotateScript::onDestroy()");
    }

    virtual void onUpdate(Entity& entity, double deltaTime) override
    {
        auto& transformComponent = getComponent<TransformComponent>(entity);
        transformComponent.rotation += m_rotation * (float)(m_speed * deltaTime);
    }

    const float& getSpeed() const { return m_speed; }
    void setSpeed(const float& speed) { m_speed = speed; }

private:
    float m_speed{0.001f};
    glm::vec3 m_rotation{0.0f, 1.0f, 0.0f};
    PropertiesExposer<RotateScript> m_propExposer;
};

class TranslateScript : public NativeScript
{
public:
    TranslateScript() : m_propExposer(*this)
    {
        CM_CORE_LOG_DEBUG("TranslateScript::TranslateScript()");
    }

    virtual ~TranslateScript()
    {
        CM_CORE_LOG_DEBUG("TranslateScript::~TranslateScript()");
    }

    virtual void onCreate() override
    {
        CM_LOG_DEBUG("TranslateScript::onCreate()");
        setPropertiesExposer(&m_propExposer);
        m_propExposer.add("Speed", {nullptr, &TranslateScript::getSpeed, &TranslateScript::setSpeed});
        m_propExposer.add("Translation", {nullptr, &TranslateScript::getTranslation, &TranslateScript::setTranslation});
    }

    virtual void onDestroy() override
    {
        CM_LOG_DEBUG("TranslateScript::onDestroy()");
    }

    virtual void onUpdate(Entity& entity, double deltaTime) override
    {
        auto& transformComponent = getComponent<TransformComponent>(entity);
        m_delta += m_speed * deltaTime;
        transformComponent.translation += m_translation * (float)(glm::cos(m_delta));
    }

    const float& getSpeed() const { return m_speed; }
    void setSpeed(const float& speed) { m_speed = speed; }

    const glm::vec3& getTranslation() const { return m_translation; }
    void setTranslation(const glm::vec3& translation) { m_translation = translation; }

private:
    float m_delta{0.0f};
    float m_speed{0.005f};
    glm::vec3 m_translation{0.04f, 0.0f, 0.0f};
    PropertiesExposer<TranslateScript> m_propExposer;
};


//
// ==== Expose Factory functions to Comet Editor ====
//
extern "C"
{
    std::vector<const char*> getScripts()
    {
        return {
            "RotateScript",
            "TranslateScript"
        };
    }

    NativeScript* instantiateRotateScript() { return static_cast<NativeScript*>(new RotateScript()); }
    void destroyRotateScript(NativeScriptComponent* scriptComponent)
    {
        delete scriptComponent->instance;
    }

    NativeScript* instantiateTranslateScript() { return static_cast<NativeScript*>(new TranslateScript()); }
    void destroyTranslateScript(NativeScriptComponent* scriptComponent)
    {
        delete scriptComponent->instance;
    }
}
