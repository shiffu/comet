#pragma once

#include <rendering/vertexArray.h>

namespace comet
{

    class OpenglVertexArray : public VertexArray
    {
    public:
        OpenglVertexArray();
        virtual ~OpenglVertexArray();

        OpenglVertexArray(OpenglVertexArray&& other);
        OpenglVertexArray& operator=(OpenglVertexArray&& other) noexcept;

        virtual void bind() const override;
        virtual void unbind() const override;
        virtual void addLayout(const VertexBufferLayout& vbl, const VertexBuffer& vbo, const IndexBuffer* ibo = nullptr) override;

    private:
        unsigned int m_vao{0};
    };
    
} // namespace comet
