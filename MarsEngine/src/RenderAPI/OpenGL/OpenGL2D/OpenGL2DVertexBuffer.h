 #pragma once
 #include "MarsHeader.h"
#include "RenderAPI/Buffers2D.h"
 
 #include "RenderAPI\OpenGL\OpenGLErrorhandler.h"
 #include "GL/glew.h"

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            class OpenGL2DVertexBuffer : public VertexBuffer2D
            {
            private:
                unsigned int m_RendererID = 0, m_Filled = 0;
                bool Emptybuffer, ClearBuffer = true;
            public:
                OpenGL2DVertexBuffer(const ME_DATATYPE* data, unsigned int size, unsigned int mode);
                OpenGL2DVertexBuffer(const unsigned int& RendererID);
                OpenGL2DVertexBuffer(const unsigned int& size, const unsigned int& mode);
                ~OpenGL2DVertexBuffer();

                void BufferPostRenderData(const void* data, const unsigned int& count, const unsigned int& offset) override;
                void ClearBufferOnDestroy(bool mode) override { ClearBuffer = mode; }

                void Bind() const override;
                void unBind() const override;

                inline bool IsEmpty() const override { return Emptybuffer; }
                inline unsigned int GetFilledSize() const override { return m_Filled; }
                inline unsigned int GetID() const override { return m_RendererID; }
            };

        }
    }
}