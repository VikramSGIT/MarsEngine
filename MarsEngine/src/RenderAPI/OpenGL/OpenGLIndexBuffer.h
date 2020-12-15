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
            unsigned int m_Filled;
            bool Emptyindex = false;
        public:
            OpenGLIndexBuffer(const unsigned int* data, unsigned int count, unsigned int mode);
            OpenGLIndexBuffer(const unsigned int size, const unsigned int& mode);
            ~OpenGLIndexBuffer();

            void BufferPostRenderData(const void* data, const unsigned int& size, const unsigned int& offset) override;

            void Bind() const override;
            void unBind() const override;

            inline unsigned int GetFilledSize() const override {return m_Filled; }
            inline bool IsEmpty() const override { return Emptyindex; }
        };
    }
}