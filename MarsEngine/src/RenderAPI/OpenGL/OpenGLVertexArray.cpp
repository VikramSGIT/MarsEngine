#include "OpenGLVertexArray.h"

namespace Renderer
{
    namespace OpenGL
    {
        OpenGLVertexArray::OpenGLVertexArray()
            :m_RendererID(1)
        {
            
            ME_PROFILE_TRACE_CALL();

            //GLLogCall(glGenVertexArrays(1, &m_RendererID));
        }

        OpenGLVertexArray::~OpenGLVertexArray()
        {

            ME_PROFILE_TRACE_CALL();

            //GLLogCall(glDeleteVertexArrays(1, &m_RendererID));
        }

        void OpenGLVertexArray::AddBuffer(const VertexBuffer& vertexbuffer, const VertexBufferLayout& layout)
        {

            ME_PROFILE_TRACE_CALL();

            Bind();
            vertexbuffer.Bind();
            const std::vector<VertexBufferElement>& elements = layout.GetElements();
            unsigned int offset = 0;

            for (unsigned int i = 0; i < elements.size(); i++)
            {
                const VertexBufferElement element = elements[i];
                GLLogCall(glEnableVertexAttribArray(i));
                GLLogCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)(offset)));
                offset += GetElementSize(element.type) * element.count;
            }
            vertexbuffer.unBind();
        }

        void OpenGLVertexArray::Bind() const
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glBindVertexArray(m_RendererID));
        }

        void OpenGLVertexArray::unBind() const
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glBindVertexArray(0));
        }
    }
}
