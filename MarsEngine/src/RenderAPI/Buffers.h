#pragma once
#include <vector>

#include "MarsHeader.h"
#include "GL/glew.h"

namespace Renderer
{
    class VertexBuffer
    {
    public:
        VertexBuffer() = default;
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void unBind() const = 0;
    };

    class IndexBuffer
    {
    public:
        IndexBuffer() = default;
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void unBind() const = 0;
    };

    //OpenGL stuffs need to be hidden
    struct VertexBufferElement
    {
        unsigned int type;
        unsigned int count;
        char normalized;
    };

    class VertexBufferLayout
    {
    private:
        std::vector<VertexBufferElement>  m_Elements;
        unsigned int m_Stride;
    public:
        VertexBufferLayout()
            : m_Stride(0) {};

        ~VertexBufferLayout() {}

        void push(unsigned int type, unsigned int count)
        {
            m_Elements.push_back({ type, count, GL_FALSE });
            m_Stride += sizeof(GLfloat) * count;
        }

        inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }

        inline const unsigned int GetStride() const { return m_Stride; }
    };

    class VertexArray
    {
    public:
        VertexArray() = default;
        virtual ~VertexArray() = default;

        virtual void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) = 0;

        virtual void Bind() const = 0;
        virtual void unBind() const = 0;
    };

    inline unsigned int GetElementSize(unsigned int type)
    {

        ME_PROFILE_TRACE_CALL();

        switch (type)
        {
        case GL_FLOAT:         return 4;
        case GL_UNSIGNED_INT:  return 4;
        case GL_UNSIGNED_BYTE: return 1;

        default:               return 0;
        }
    }
}