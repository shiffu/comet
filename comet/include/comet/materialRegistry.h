#pragma once

#include <comet/singleton.h>
#include <comet/material.h>

#include <cstdint>
#include <vector>
#include <memory>

namespace comet
{

    class MaterialRegistry : public Singleton<MaterialRegistry>
    {
    public:
        ~MaterialRegistry() = default;

        template<typename... Args>
        Material* createMaterialInstance(Args&&... args)
        {
            auto materialInstancePtr = std::make_unique<Material>(std::forward<Args>(args)...);

            uint32_t instanceId = m_materials.size();
            materialInstancePtr->m_instanceID = instanceId;
            m_materials.push_back(std::move(materialInstancePtr));
            
            return m_materials.back().get();
        }

        bool hasMaterial(uint32_t materialInstanceId) const noexcept
        {   
            return materialInstanceId < m_materials.size();
        }

        std::vector<std::unique_ptr<Material>>& getMaterialInstances() noexcept
        {
            return m_materials;
        }

        uint32_t getMaterialInstanceCount() const { return m_materials.size(); }

        Material* getMaterialInstance(uint32_t instanceId) noexcept
        {
            Material* material = nullptr;

            if (instanceId < m_materials.size())
            {
                material = m_materials[instanceId].get();
            }

            return material;
        }

    private:
        std::vector<std::unique_ptr<Material>> m_materials;
    };
    
} // namespace comet
