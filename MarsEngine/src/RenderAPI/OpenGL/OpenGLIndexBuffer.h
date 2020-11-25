#pragma  once
#include "MarsHeader.h"
#include "RenderAPI/Buffers.h"

#include "GL/glew.h"
#include "OpenGLErrorhandle.h"
namespace Renderer
{
    namespace OpenGL
    {
        class OpenGLIndexBuffer : public IndexBuffer
        {
        private:
            unsigned int m_RendererID;
            unsigned int m_Count;
        public:
            OpenGLIndexBuffer(const unsigned int* data, unsigned int count, unsigned int mode);
            ~OpenGLIndexBuffer();

            void Bind() const override;
            void unBind() const override;

            inline unsigned int GetCount() const {return m_Count; }
        };
    }
}