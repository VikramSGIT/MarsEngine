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
            unsigned int m_RendererID = 0, m_Filled = 0;
            bool Emptybuffer, ClearBuffer = true;
        public:
            OpenGLVertexBuffer(const float* data, unsigned int size, unsigned int mode);
            OpenGLVertexBuffer(const unsigned int& RendererID);
            OpenGLVertexBuffer(const unsigned int& size, const unsigned int& mode);
            ~OpenGLVertexBuffer();

            void BufferPostRenderData(const void* data, const unsigned int& size, const unsigned int& offset) override;
            void ClearBufferOnDestroy(bool mode) override { ClearBuffer = mode; }

            void Bind() const override;
            void unBind() const override;

            inline bool IsEmpty() const override { return Emptybuffer; }
            inline unsigned int GetFilledSize() const override { return m_Filled; }
            inline unsigned int GetID() const override { return m_RendererID; }
        };

    }
}