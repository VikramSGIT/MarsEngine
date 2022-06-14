#include "MarsHeader.h"
#include "Buffers.h"

#include "Renderer.h"
#include "OpenGL/OpenGLFramebuffer.h"

#include "GL/glew.h"
namespace ME
{
    namespace Renderer
    {
        unsigned int GetElementSize(unsigned int type)
        {

            ME_PROFILE_TRACE_CALL();

            switch (type)
            {
            case GL_FLOAT:         return 4;
            case GL_UNSIGNED_INT:  return 4;
            case GL_UNSIGNED_BYTE: return 1;

            default:               return 0;
            }
        }

        Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
        {
            switch (RenderAPI::GetAPI())
            {
            case RenderAPItype::ME_RENDERER_OPENGL:
            case RenderAPItype::ME_RENDERER_OPENGL2D:
                return CreateRef<OpenGL::OpenGLFramebuffer>(spec);
            default:ME_CORE_CRITICAL(true, "Unknow Render API type");
            }
            return nullptr;
        }

        void VertexbufferLayout::push(unsigned int type, unsigned int count)
        {
            m_Elements.push_back({ type, count, GL_FALSE });
            m_Stride += sizeof(GLfloat) * count;
        }
    }
}
