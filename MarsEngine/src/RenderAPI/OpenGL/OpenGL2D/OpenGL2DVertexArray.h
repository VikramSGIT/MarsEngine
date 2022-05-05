#ifndef ME_OPENGL2DVERTEXARRAY
#define ME_OPENGL2DVERTEXARRAY

#pragma once

#include "MarsHeader.h"
#include "RenderAPI/Buffers2D.h"

#include "OpenGL2DVertexBuffer.h"
#include "RenderAPI\OpenGL\OpenGLErrorhandler.h"

#include <iostream>
namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            class OpenGL2DVertexArray : public VertexArray2D
            {
            private:
                unsigned m_RendererID;
            public:
                OpenGL2DVertexArray();
                ~OpenGL2DVertexArray();

                void AddBuffer(const VertexBuffer2D& vertexbuffer, const VertexBufferLayout2D& layout) override;

                void Bind() const override;
                void unBind() const override;
            };
        }
    }
}

#endif