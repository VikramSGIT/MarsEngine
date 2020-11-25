#include "OpenGLIndexBuffer.h"

namespace Renderer
{
    namespace OpenGL
    {
        OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned int* data, unsigned int count, const unsigned int mode)
            :m_Count(0)
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glGenBuffers(1, &m_RendererID));
            GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
            GLLogCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, mode));
        }
        OpenGLIndexBuffer::~OpenGLIndexBuffer()
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glDeleteBuffers(1, &m_RendererID));
        }
        void OpenGLIndexBuffer::Bind() const
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
        }
        void OpenGLIndexBuffer::unBind() const
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        }
    }
}