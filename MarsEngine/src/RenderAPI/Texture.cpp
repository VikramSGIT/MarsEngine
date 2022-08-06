#include "Texture.h"
#include "OpenGL/OpenGL2D/OpenGLTexture2D.h"
#include "Renderer.h"

namespace ME
{
	Ref<Texture> ME::Texture2D::Create(const string& path)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPItype::ME_RENDERER_OPENGL2D:
			return CreateRef<OpenGL::OpenGLTexture2D>(path);
		default:
			ME_CORE_CRITICAL(true, "Only OpenGL2D is supported for now");
		}
	}
}
