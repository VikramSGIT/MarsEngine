#ifndef ME_OPENGL2DINDEXBUFFER
#define ME_OPENGL2DINDEXBUFFER

#pragma  once

#include "MarsHeader.h"
#include "RenderAPI/Buffers2D.h"

#include "GL/glew.h"
#include "RenderAPI/OpenGL/OpenGLErrorhandler.h"

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            class OpenGL2DIndexBuffer : public IndexBuffer2D
            {
            private:
                unsigned int m_RendererID;
                unsigned int m_Filled;
                bool Emptyindex, ClearBuffer = true;
            public:
                OpenGL2DIndexBuffer(const unsigned int* data, unsigned int count, unsigned int mode);
                OpenGL2DIndexBuffer(const unsigned int& RendererID);
                OpenGL2DIndexBuffer(const unsigned int size, const unsigned int& mode);
                ~OpenGL2DIndexBuffer();

                void BufferPostRenderData(const void* data, const unsigned int& size, const unsigned int& offset) override;
                void ClearBufferOnDestroy(bool mode) override { ClearBuffer = mode; }

                void Bind() const override;
                void unBind() const override;

                inline unsigned int GetFilledSize() const override { return m_Filled; }
                inline bool IsEmpty() const override { return Emptyindex; }
                inline unsigned int GetID() const override { return m_RendererID; }
            };
        }
    }
}

#endif