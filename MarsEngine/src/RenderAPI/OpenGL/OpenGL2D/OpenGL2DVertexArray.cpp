#include "OpenGL2DVertexArray.h"

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            OpenGL2DVertexArray::OpenGL2DVertexArray()
                :m_RendererID(1)
            {

                ME_PROFILE_TRACE_CALL();

                //GLLogCall(glGenVertexArrays(1, &m_RendererID));
            }

            OpenGL2DVertexArray::~OpenGL2DVertexArray()
            {

                ME_PROFILE_TRACE_CALL();

                //GLLogCall(glDeleteVertexArrays(1, &m_RendererID));
            }

            void OpenGL2DVertexArray::AddBuffer(const VertexBuffer2D& vertexbuffer, const VertexBufferLayout2D& layout)
            {

                ME_PROFILE_TRACE_CALL();

                Bind();
                vertexbuffer.Bind();
                const std::vector<VertexBufferElement> elements = layout.GetElements();
                unsigned int offset = 0;

                for (unsigned int i = 0; i < elements.size(); i++)
                {
                    const VertexBufferElement element = elements[i];
                    GLLogCall(glEnableVertexAttribArray(i));
                    GLLogCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
                    offset += GetElementSize(element.type) * element.count;
                }
                vertexbuffer.unBind();
            }

            void OpenGL2DVertexArray::Bind() const
            {

                ME_PROFILE_TRACE_CALL();

                GLLogCall(glBindVertexArray(m_RendererID));
            }

            void OpenGL2DVertexArray::unBind() const
            {

                ME_PROFILE_TRACE_CALL();

                GLLogCall(glBindVertexArray(0));
            }
        }
    }
}
