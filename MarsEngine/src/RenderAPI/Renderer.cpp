#include "MarsHeader.h"
#include "Renderer.h"
#include "Core/Logger.h"

#include "OpenGL/OpenGLRenderer.h"
#include "OpenGL/OpenGL2d/OpenGL2DRenderer.h"
namespace ME
{
    RenderAPItype RenderAPI::s_renderapi = RenderAPItype::ME_NONE;

    Ref<RenderAPI> RenderAPI::Create(const RenderAPItype& renderapitype)
    {
        s_renderapi = renderapitype;
        switch (renderapitype)
        {
        case RenderAPItype::ME_RENDERER_OPENGL:
            return CreateRef<OpenGL::OpenGLRendererAPI>();
        case RenderAPItype::ME_RENDERER_OPENGL2D:
            return CreateRef<OpenGL::OpenGL2DRendererAPI>();
        default:ME_CORE_CRITICAL(true, "Unknown Renderer API type");
        }
        return nullptr;
    }
}