#pragma once

namespace comet
{

    class VertexBuffer
    {
    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void bind() const;
        void unbind() const;

        void loadData(const void* data, unsigned int size);

    private:
        unsigned int m_vbo{0};
    };
}