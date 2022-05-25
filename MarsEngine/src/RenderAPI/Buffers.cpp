#include "MarsHeader.h"
#include "Buffers.h"

#include "OpenGL/OpenGLRenderer.h"
#include "OpenGL/OpenGL2D/OpenGL2DRenderer.h"
#include "OpenGL/OpenGLVertexBuffer.h"
#include "OpenGL/OpenGLIndexBuffer.h"
#include "OpenGL/OpenGLFramebuffer.h"

namespace ME
{
    namespace Renderer
    {

        Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
        {
            switch (RenderAPI::GetAPI())
            {
            case RenderAPItype::ME_RENDERER_OPENGL:
            case RenderAPItype::ME_RENDERER_OPENGL2D:
                return CreateRef<OpenGL::OpenGLFramebuffer>();
            default:ME_CORE_CRITICAL(true, "Unknow Render API type");
            }
            return nullptr;
        }
    }
}
