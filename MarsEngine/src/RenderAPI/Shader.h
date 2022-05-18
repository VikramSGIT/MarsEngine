#ifndef ME_SHADER
#define ME_SHADER

#include "MarsHeader.h"
#include "Vender/glm/glm/gtc/matrix_transform.hpp"

#include <string>

namespace ME
{
	namespace Renderer
	{
		class Shader
		{
		public:
			virtual ~Shader() = default;

			virtual void Bind() = 0;
			virtual void unBind() = 0;

			virtual void SetUniforms4f(const std::string& name, const float& f1, const float& f2, const float& f3, const float& f4) = 0;
			virtual void SetUniforms3f(const std::string& name, const float* data) = 0;
			virtual void SetUniforms1i(const std::string& name, const int& data) = 0;
			virtual void SetUniforms1f(const  std::string& name, const float& data) = 0;
			virtual void SetUniformsMat4f(const  std::string& name, const glm::mat4& matrix) = 0;
		};
	}
}

#endif