#include <comet/flatColorMaterial.h>
#include <rendering/vertexBufferLayout.h>
#include <comet/materialRegistry.h>
#include <comet/utils.h>

namespace comet
{
    const char* FlatColorMaterial::MATERIAL_NAME = "cometFlatColorMaterial";
    const char* FlatColorMaterial::SHADER_NAME = "cometFlatColor";

    constexpr uint32_t getTypeHashImpl()
    {
        constexpr uint32_t typeHash = utils::hashStr("FlatColorMaterial");
        return typeHash;
    }

    FlatColorMaterial::FlatColorMaterial()
        : Material(MATERIAL_NAME, SHADER_NAME)
    {
    }

    FlatColorMaterial::FlatColorMaterial(const glm::vec4& color)
        : Material(MATERIAL_NAME, SHADER_NAME), m_color(color)
    {
    }

    uint32_t FlatColorMaterial::getTypeHash() const
    {
        return getTypeHashImpl();
    }

    // Vertex and Instance Buffers Layout
    void FlatColorMaterial::updateVboDataLayout(VertexBufferLayout& layout) const
    {
        layout.addFloat(3, false, 0); // position
        layout.addFloat(3, false, 1); // normal
        layout.addFloat(2, false, 2); // texture coordinate
    }

    void FlatColorMaterial::updateInstanceDataLayout(VertexBufferLayout& layout) const
    {
        layout.addFloat(4, false, 10, 1); //
        layout.addFloat(4, false, 11, 1); //
        layout.addFloat(4, false, 12, 1); // Instance Model to Word Matrix transform
        layout.addFloat(4, false, 13, 1); //
        layout.addUInt(1, false, 14, 1);  // Material ID (or index)
    }

    void FlatColorMaterial::loadUniforms()
    {
        getShader();
        m_shader->bind();

        auto materialInstancesPtr = MaterialRegistry::getInstance().getMaterialInstances(getTypeHash());
        if (materialInstancesPtr)
        {
            auto& materialInstances = *materialInstancesPtr;
            auto materialInstanceCount = materialInstances.size();
            std::vector<glm::vec4> colors{};
            colors.reserve(materialInstanceCount);

            for (auto& materialInstance : materialInstances)
            {
                auto material = static_cast<FlatColorMaterial*>(materialInstance.get());
                colors.push_back(material->getColor());
            }
            m_shader->setUniform("u_flatColor", materialInstanceCount, (glm::vec4*)colors.data());
        }
    }

} // namespace comet