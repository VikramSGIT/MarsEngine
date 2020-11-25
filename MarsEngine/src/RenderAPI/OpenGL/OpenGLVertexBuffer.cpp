#include "OpenGLVertexBuffer.h"

namespace Renderer
{
    namespace OpenGL
    {
        OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, unsigned int size, unsigned int mode)
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glGenBuffers(1, &m_RendererID));
            GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
            GLLogCall(glBufferData(GL_ARRAY_BUFFER, size, data, mode));
        }
        OpenGLVertexBuffer::~OpenGLVertexBuffer()
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glDeleteBuffers(1, &m_RendererID));
        }
        void OpenGLVertexBuffer::Bind() const
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        }
        void OpenGLVertexBuffer::unBind() const
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }
    }
}