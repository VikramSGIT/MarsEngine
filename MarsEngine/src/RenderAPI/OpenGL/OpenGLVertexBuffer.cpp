#include "OpenGLVertexBuffer.h"

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            OpenGLVertexBuffer::OpenGLVertexBuffer(const ME_DATATYPE* data, unsigned int size, unsigned int mode)
                :Emptybuffer(false), m_RendererID(0)
            {

                ME_PROFILE_TRACE_CALL();

                GLLogCall(glGenBuffers(1, &m_RendererID));
                GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
                GLLogCall(glBufferData(GL_ARRAY_BUFFER, size, data, mode));
            }

            OpenGLVertexBuffer::OpenGLVertexBuffer(const unsigned int& RendererID)
                :Emptybuffer(true), m_RendererID(RendererID)
            {
                GLClearError();
                glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
                if (!GLPrintError("glBindBuffer", __FILE__, __LINE__))
                {
                    ME_CORE_CRITICAL("Can't load buffer with ID");
                    ClearBuffer = false;
                    this->~OpenGLVertexBuffer();
                }
            }

            OpenGLVertexBuffer::OpenGLVertexBuffer(const unsigned int& size, const unsigned int& mode)
                :Emptybuffer(true), m_RendererID(0)
            {

                ME_PROFILE_TRACE_CALL();

                GLLogCall(glGenBuffers(1, &m_RendererID));
                GLLogCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
                GLLogCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, mode));
            }
            OpenGLVertexBuffer::~OpenGLVertexBuffer()
            {

                ME_PROFILE_TRACE_CALL();

                if (ClearBuffer)
                {
                    GLLogCall(glDeleteBuffers(1, &m_RendererID));
                }
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
                GLLogCall(glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(ME_DATATYPE), count * sizeof(ME_DATATYPE), data));
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
}