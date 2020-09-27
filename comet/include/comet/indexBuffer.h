#pragma once

namespace comet
{

    class IndexBuffer
    {
    public:
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void bind() const;
        void unbind() const;

        inline unsigned int getCount() const {return m_count; }
    private:
        unsigned int m_ibo{0};
        unsigned int m_count;
    };
}