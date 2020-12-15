#include "OpenGLVertexBuffer.h"

namespace Renderer
{
    namespace OpenGL
    {
        OpenGLVertexBuffer::OpenGLVertexBuffer(const float* data, unsigned int size, unsigned int mode)
            :Emptybuffer(false)
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glGenBuffers(1, &m_RendererID));
            GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
            GLLogCall(glBufferData(GL_ARRAY_BUFFER, size, data, mode));
        }
        OpenGLVertexBuffer::OpenGLVertexBuffer(const unsigned int& size, const unsigned int& mode)
            :Emptybuffer(true)
        {

            ME_PROFILE_TRACE_CALL();

            glewInit();

            GLLogCall(glGenBuffers(1, &m_RendererID));
            GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
            GLLogCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, mode));
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

        void OpenGLVertexBuffer::BufferPostRenderData(const void* data, const unsigned int& count, const unsigned int& offset)
        {

            ME_PROFILE_TRACE_CALL();

            Bind();
            GLLogCall(glBufferSubData(GL_ARRAY_BUFFER, offset, count *sizeof(float), data));
            unBind();
            m_Filled += count;
        }
        void OpenGLVertexBuffer::unBind() const
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }
    }
}