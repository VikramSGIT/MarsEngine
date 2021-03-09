#include "OpenGLIndexBuffer.h"

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned int* data, unsigned int count, const unsigned int mode)
                :m_Filled(0), m_RendererID(0), Emptyindex(false)
            {

                ME_PROFILE_TRACE_CALL();

                GLLogCall(glGenBuffers(1, &m_RendererID));
                GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
                GLLogCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, mode));
            }
            OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned int& RendererID)
                :m_Filled(NULL), m_RendererID(RendererID), Emptyindex(true)
            {
                GLClearError();
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
                if (!GLPrintError("glBindBuffer", __FILE__, __LINE__))
                {
                    ME_CORE_CRITICAL("Can't load IndexBuffer with ID");
                    ClearBuffer = false;
                    this->~OpenGLIndexBuffer();
                }
            }
            OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned int size, const unsigned int& mode)
                :m_Filled(0), m_RendererID(0), Emptyindex(true)
            {
                ME_PROFILE_TRACE_CALL();

                GLLogCall(glGenBuffers(1, &m_RendererID));
                GLLogCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
                GLLogCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, mode));

            }
            OpenGLIndexBuffer::~OpenGLIndexBuffer()
            {

                ME_PROFILE_TRACE_CALL();
                if (ClearBuffer)
                {
                    GLLogCall(glDeleteBuffers(1, &m_RendererID));
                }
            }
            void OpenGLIndexBuffer::BufferPostRenderData(const void* data, const unsigned int& count, const unsigned int& offset)
            {

                ME_PROFILE_TRACE_CALL();

                Bind();
                GLLogCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(unsigned int), count * sizeof(unsigned int), data));
                unBind();
                m_Filled += count;
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
}