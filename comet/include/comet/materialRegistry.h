#pragma once

#include <comet/singleton.h>
#include <comet/material.h>
#include <comet/utils.h>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>

namespace comet
{

    class MaterialRegistry : public Singleton<MaterialRegistry>
    {
    public:
        ~MaterialRegistry() = default;

        template<typename T, typename... Args>
        T* createMaterial(Args&&... args)
        {
            auto materialInstancePtr = std::make_unique<T>(std::forward<Args>(args)...);
            auto materialTypeHash = materialInstancePtr->getTypeHash();

            auto& materials = m_materials[materialTypeHash];
            uint32_t instanceId = materials.size();
            materialInstancePtr->m_instanceID = instanceId;
            materials.push_back(std::move(materialInstancePtr));
            
            return static_cast<T*>(materials.back().get());
        }

        bool hasMaterial(uint32_t materialTypeHash, uint32_t materialInstanceId) const noexcept
        {   
            auto it = m_materials.find(materialTypeHash);
            if (it == m_materials.end())
            {
                return false;
            }

            return materialInstanceId < it->second.size();
        }

        const std::vector<std::unique_ptr<Material>>* getMaterialInstances(uint32_t materialTypeHash) const noexcept
        {
            auto it = m_materials.find(materialTypeHash);
            if (it == m_materials.end())
            {
                return nullptr;
            }

            return &(it->second);
        }

        Material* getMaterial(uint32_t materialTypeHash, uint32_t materialInstanceId) const noexcept
        {
            if (!hasMaterial(materialTypeHash, materialInstanceId))
            {
                return nullptr;
            }

            auto it = m_materials.find(materialTypeHash);
            return it->second[materialInstanceId].get();
        }

    private:
        std::unordered_map<uint32_t, std::vector<std::unique_ptr<Material>>> m_materials;
    };
    
} // namespace comet
