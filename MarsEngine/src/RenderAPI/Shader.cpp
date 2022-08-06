#include "Shader.h"
#include "MarsHeader.h"
#include "Shader.h"

#include "Renderer.h"
#include "OpenGL/OpenGLShader.h"
namespace ME
{
	Ref<Shader> ME::Shader::Create(const string& filename)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPItype::ME_RENDERER_OPENGL:
		case RenderAPItype::ME_RENDERER_OPENGL2D:
			return CreateRef<OpenGL::OpenGLShader>(filename);
		default:ME_CORE_CRITICAL(true, "Unknown Render API");
		}
		return nullptr;
	}
}
