 #pragma once
 #include "MarsHeader.h"
#include "RenderAPI/Buffers.h"
 
 #include "OpenGLErrorHandle.h"
 #include "GL/glew.h"

namespace Renderer
{
    namespace OpenGL
    {
        class OpenGLVertexBuffer : public VertexBuffer
        {
        private:
            unsigned int m_RendererID;
        public:
            OpenGLVertexBuffer(const void* data, unsigned int size, unsigned int mode);
            ~OpenGLVertexBuffer();

            void Bind() const override;
            void unBind() const override;
        };

    }
}