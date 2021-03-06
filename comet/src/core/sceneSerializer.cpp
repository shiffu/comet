#include "sceneSerializer.h"
#include <comet/components.h>
#include <comet/resourceManager.h>
#include <comet/materialRegistry.h>
#include <comet/staticMesh.h>
#include <comet/propertiesExposer.h>

#include <ryml.hpp>
#include <c4/yml/std/string.hpp>
#include <c4/format.hpp>

#include <fstream>

namespace c4
{
    // glm::vec3 serialization
    size_t to_chars(c4::substr buf, glm::vec3 v)
    {
        return c4::format(buf, "({},{},{})", v.x, v.y, v.z);
    }

    // glm::vec3 deserialization
    bool from_chars(c4::csubstr buf, glm::vec3 *v)
    {
        size_t ret = c4::unformat(buf, "({},{},{})", v->x, v->y, v->z);
        return ret != c4::csubstr::npos;
    }
}

namespace comet
{
    
    bool SceneSerializer::serialize(Scene& scene)
    {
        CM_PROFILE_FUNC();

        std::ofstream sceneFile("EditorScene.scn", std::ios::out | std::ios::trunc);
        ryml::Tree sceneTree;
        auto root = sceneTree.rootref();

        root |= ryml::MAP;
        auto sceneInfo = root["CometScene"];
        sceneInfo |= ryml::MAP;
        sceneInfo.append_child() << ryml::key("name") << scene.getName();
        sceneInfo.append_child() << ryml::key("apiVersion") << "1.0.0";

        auto sceneEntities = sceneInfo["entities"];
        sceneEntities |= ryml::SEQ;

        auto& registry = scene.getRegistry();
        registry.each([&] (auto entityId)
        {
            auto entity = Entity{entityId, &scene};
            auto entityNode = sceneEntities.append_child();
            entityNode |= ryml::MAP;
            entityNode.append_child() << ryml::key("id") << (uint32_t)entityId;

            // NameComponent (Mandatory)
            auto nameComponent = entity.getComponent<NameComponent>();
            entityNode.append_child() << ryml::key("NameComponent") << nameComponent.name;

            // TransformComponent (Mandatory)
            auto& transformComponent = entity.getComponent<TransformComponent>();
            auto transformNode = entityNode["TransformComponent"];
            transformNode |= ryml::MAP;
            transformNode.append_child() << ryml::key("scale") << transformComponent.scale;
            transformNode.append_child() << ryml::key("translation") << transformComponent.translation;
            transformNode.append_child() << ryml::key("rotation") << transformComponent.rotation;

            // TagComponent (Optional)
            if (entity.hasComponent<TagComponent>())
            {
                entityNode.append_child() << "TagComponent";
            }

            // PlayerTagComponent (Optional)
            if (entity.hasComponent<PlayerTagComponent>())
            {
                entityNode.append_child() << "PlayerTagComponent";
            }

            // EnemyTagComponent (Optional)
            if (entity.hasComponent<EnemyTagComponent>())
            {
                entityNode.append_child() << "EnemyTagComponent";
            }

            // MeshComponent (Optional)
            if (entity.hasComponent<MeshComponent>())
            {
                auto& component = entity.getComponent<MeshComponent>();
                auto node = entityNode["MeshComponent"];
                node |= ryml::MAP;
                auto staticMesh = ResourceManager::getInstance().getStaticMesh(component.meshTypeId);
                node.append_child() << ryml::key("name") << staticMesh.resource->getName();
            }

            // MaterialComponent (Optional)
            if (entity.hasComponent<MaterialComponent>())
            {
                auto& component = entity.getComponent<MaterialComponent>();
                auto node = entityNode["MaterialComponent"];
                node |= ryml::MAP;
                auto material = MaterialRegistry::getInstance().getMaterialInstance(component.materialInstanceId);
                node.append_child() << ryml::key("name") << material->getName();
                node.append_child() << ryml::key("shaderName") << material->getShaderName();
                node.append_child() << ryml::key("diffuse") << material->getDiffuse();
                node.append_child() << ryml::key("specular") << material->getSpecular();
                node.append_child() << ryml::key("shininess") << material->getShininess();
                node.append_child() << ryml::key("albedoTextureFilename") << material->getAlbedoTextureFilename();
            }

            // CameraComponent (Optional)
            if (entity.hasComponent<CameraComponent>())
            {
                auto& component = entity.getComponent<CameraComponent>();
                auto node = entityNode["CameraComponent"];
                node |= ryml::MAP;
                node.append_child() << ryml::key("isPrimary") << component.isPrimary;

                auto& camera = component.camera;
                auto projectionType = camera.getProjectionType();
                node.append_child() << ryml::key("projectionType") << (uint32_t)projectionType;

                if (projectionType == Camera::ProjectionType::PERSPECTIVE)
                {
                    node.append_child() << ryml::key("fov") << camera.getFOV();
                }

                auto zoom = camera.getZoom();
                node.append_child() << ryml::key("zoom") << zoom;

                auto ar = camera.getAspectRatio();
                node.append_child() << ryml::key("aspectRatio") << ar;

                auto near = camera.getNear();
                node.append_child() << ryml::key("near") << near;

                auto far = camera.getFar();
                node.append_child() << ryml::key("far") << far;
            }

            // NativeScriptComponent (Optional)
            if (entity.hasComponent<NativeScriptComponent>())
            {
                auto& component = entity.getComponent<NativeScriptComponent>();

                // We only serialize if the NativeScript Component has been bound
                // at runtime, thus having m_dllHandle != nullptr
                if (component.m_dllHandle != nullptr)
                {
                    auto node = entityNode["NativeScriptComponent"];
                    node |= ryml::MAP;
                    node.append_child() << ryml::key("scriptLibFilepath") << component.scriptLibFilepath;
                    node.append_child() << ryml::key("selectedScriptName") << component.selectedScriptName;

                    auto propertiesNode = node["properties"];
                    propertiesNode |= ryml::SEQ;

                    auto propertiesExposer = component.instance->getPropertiesExposer();
                    if (propertiesExposer)
                    {
                        for(auto& propertyName : propertiesExposer->getPropertiesName())
                        {
                            auto propertyNode = propertiesNode.append_child();
                            propertyNode |= ryml::MAP;

                            propertyNode.append_child() << ryml::key("name") << propertyName;

                            auto valueContainer = propertiesExposer->getPropertyValue(propertyName);
                            if (valueContainer)
                            {
                                switch (valueContainer->typeTag)
                                {
                                case NativeScriptPropertyTypeTag::FLOAT:
                                    propertyNode.append_child() << ryml::key("type") << NativeScriptPropertyTypeTag::FLOAT;
                                    propertyNode.append_child() << ryml::key("value") << std::get<NativeScriptPropertyTypeTag::FLOAT>(valueContainer->value);
                                    break;
                                
                                case NativeScriptPropertyTypeTag::VEC3:
                                    propertyNode.append_child() << ryml::key("type") << NativeScriptPropertyTypeTag::VEC3;
                                    propertyNode.append_child() << ryml::key("value") << std::get<NativeScriptPropertyTypeTag::VEC3>(valueContainer->value);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        });

        sceneFile << sceneTree;
        sceneFile.flush();
        sceneFile.close();
    }

    bool SceneSerializer::deserialize(Scene& scene, const char* sceneFilename)
    {
        CM_PROFILE_FUNC();

        std::ifstream sceneFile(sceneFilename, std::ios::in);
        bool success{true};

        if (!sceneFile.is_open())
        {
            CM_LOG_ERROR("Cannot open Scene file '{}'", sceneFilename);
            success = false;
        }
        else
        {
            CM_LOG_DEBUG("Opened Scene file '{}'", sceneFilename);
            std::stringstream ss;
            ss << sceneFile.rdbuf();
            auto sceneData = ss.str();

            auto sceneTree = ryml::parse(c4::to_substr(sceneData));
            if (sceneTree.size() <= 1)
            {
                CM_LOG_ERROR("Invalid scene, no data");
                return false;
            }

            auto sceneRoot = sceneTree["CometScene"];

            if (!sceneRoot.valid() || sceneRoot.is_seed() || !sceneRoot.is_map())
            {
                CM_LOG_ERROR("Scene '{}' is not a valid Comet Scene", sceneFilename);
                success = false;
            }
            else
            {
                auto& resManager = ResourceManager::getInstance();
                auto& materialRegistry = MaterialRegistry::getInstance();

                scene.clear();

                // Scene Name
                std::string sceneName;
                sceneRoot["name"] >> sceneName;
                scene.setName(sceneName);

                // Scene Version
                std::string sceneVersionText;
                sceneRoot["apiVersion"] >> sceneVersionText;

                // Scene Entities
                auto sceneEntities = sceneRoot["entities"];

                for (auto entityNode : sceneEntities)
                {
                    auto entity = scene.createEntity(false);

                    // NameComponent (Mandatory)
                    std::string name;
                    entityNode["NameComponent"] >> name;
                    entity.addComponent<NameComponent>(name);

                    // TransformComponent (Mandatory)
                    {
                        auto compNode = entityNode["TransformComponent"];
                        auto& component = entity.addComponent<TransformComponent>();
                        compNode["scale"] >> component.scale;
                        compNode["translation"] >> component.translation;
                        compNode["rotation"] >> component.rotation;
                    }

                    // TagComponent (Optional)
                    if (entityNode["TagComponent"] != nullptr)
                    {
                        entity.addComponent<TagComponent>();
                    }

                    // PlayerTagComponent (Optional)
                    if (entityNode["PlayerTagComponent"] != nullptr)
                    {
                        entity.addComponent<PlayerTagComponent>();
                    }

                    // EnemyTagComponent (Optional)
                    if (entityNode["EnemyTagComponent"] != nullptr)
                    {
                        entity.addComponent<EnemyTagComponent>();
                    }

                    // MeshComponent (Optional)
                    if (entityNode["MeshComponent"] != nullptr)
                    {
                        auto compNode = entityNode["MeshComponent"];
                        std::string filename;
                        compNode["name"] >> filename;
                        auto resourceHandle = resManager.loadStaticMesh(filename.c_str());
                        
                        auto& component = entity.addComponent<MeshComponent>();
                        component.meshTypeId = resourceHandle.resourceId;
                    }

                    // MaterialComponent (Optional)
                    if (entityNode["MaterialComponent"] != nullptr)
                    {
                        auto compNode = entityNode["MaterialComponent"];
                        std::string materialName;
                        compNode["name"] >> materialName;
                        std::string shaderName;
                        compNode["shaderName"] >> shaderName;

                        auto material = materialRegistry.createMaterialInstance(materialName.c_str(), shaderName.c_str());
                        glm::vec3 diffuse;
                        compNode["diffuse"] >> diffuse;
                        material->setDiffuse(diffuse);

                        glm::vec3 specular;
                        compNode["specular"] >> specular;
                        material->setSpecular(specular);

                        float shininess;
                        compNode["shininess"] >> shininess;
                        material->setShininess(shininess);                        

                        std::string albedoTextureFilename;
                        compNode["albedoTextureFilename"] >> albedoTextureFilename;
                        material->setAlbedoTexture(albedoTextureFilename);                        
                        
                        auto& component = entity.addComponent<MaterialComponent>();
                        component.materialInstanceId = material->getInstanceId();
                    }

                    // CameraComponent (Optional)
                    if (entityNode["CameraComponent"] != nullptr)
                    {
                        auto compNode = entityNode["CameraComponent"];
                        bool isPrimary;
                        compNode["isPrimary"] >> isPrimary;

                        uint32_t projectionType;
                        compNode["projectionType"] >> projectionType;
                        Camera camera((Camera::ProjectionType)projectionType);

                        if (projectionType == Camera::ProjectionType::PERSPECTIVE)
                        {
                            float fov;
                            compNode["fov"] >> fov;
                            camera.setFOV(fov);
                        }

                        float zoom;
                        compNode["zoom"] >> zoom;
                        camera.setZoom(zoom);

                        float ar;
                        compNode["aspectRatio"] >> ar;
                        camera.setAspectRatio(ar);

                        float near;
                        compNode["near"] >> near;
                        camera.setNear(near);

                        float far;
                        compNode["far"] >> far;
                        camera.setFar(far);

                        auto& component = entity.addComponent<CameraComponent>();
                        component.isPrimary = isPrimary;
                        component.camera = camera;
                    }

                    // NativeScriptComponent (Optional)
                    if (entityNode["NativeScriptComponent"] != nullptr)
                    {
                        auto compNode = entityNode["NativeScriptComponent"];
                        auto& component = entity.addComponent<NativeScriptComponent>();

                        std::string scriptLibFilepath;
                        compNode["scriptLibFilepath"] >> scriptLibFilepath;
                        std::string scriptName;
                        compNode["selectedScriptName"] >> scriptName;

                        if (!component.runtimeBind(scriptLibFilepath.c_str(), scriptName.c_str()))
                        {
                            CM_LOG_ERROR("Unable to bind NativeScript '{}' from Lib '{}'", scriptName, scriptLibFilepath);
                        }
                        else
                        {
                            scene.instantiateNativeScriptComponent(component);
                            
                            auto propertiesExposer = component.instance->getPropertiesExposer();
                            if (propertiesExposer)
                            {
                                if (compNode["properties"] != nullptr)
                                {
                                    auto propertiesNode = compNode["properties"];
                                    for (auto propertyNode : propertiesNode)
                                    {
                                        std::string propertyName;
                                        propertyNode["name"] >> propertyName;
                                        uint32_t propertyType;
                                        propertyNode["type"] >> propertyType;

                                        switch ((NativeScriptPropertyTypeTag)propertyType)
                                        {
                                        case NativeScriptPropertyTypeTag::FLOAT:
                                        {                                            
                                            float propertyValue;
                                            propertyNode["value"] >> propertyValue;
                                            propertiesExposer->setPropertyValue(propertyName, {(NativeScriptPropertyTypeTag)propertyType, propertyValue});
                                            break;
                                        }
                                        
                                        case NativeScriptPropertyTypeTag::VEC3:
                                        {
                                            glm::vec3 propertyValue;
                                            propertyNode["value"] >> propertyValue;
                                            propertiesExposer->setPropertyValue(propertyName, {(NativeScriptPropertyTypeTag)propertyType, propertyValue});
                                            break;
                                        }
                                        
                                        default:
                                            CM_LOG_ERROR("Unmanaged property type {}", propertyType);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return success;
    }

} // namespace comet
