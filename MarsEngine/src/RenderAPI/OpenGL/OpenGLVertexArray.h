#pragma once
#include "MarsHeader.h"
#include "RenderAPI/Buffers.h"

#include "OpenGLVertexBuffer.h"
#include "OpenGLErrorHandler.h"

#include <iostream>
namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            class OpenGLVertexArray
            {
            private:
                unsigned m_RendererID;
            public:
                OpenGLVertexArray();
                ~OpenGLVertexArray();

                void AddBuffer(const VertexBuffer& vertexbuffer, const VertexBufferLayout& layout);

                void Bind() const;
                void unBind() const;
            };
        }
    }
}

