#pragma once
#include "MarsHeader.h"
#include "RenderAPI/Buffers.h"

#include "OpenGLVertexBuffer.h"
#include "OpenGLErrorHandle.h"

#include <iostream>
namespace Renderer
{
    namespace OpenGL
    {
        class OpenGLVertexArray : public VertexArray
        {
        private:
            unsigned m_RendererID;
        public:
            OpenGLVertexArray();
            ~OpenGLVertexArray();

            void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) override;

            void Bind() const override;
            void unBind() const override;
        };
    }
}

