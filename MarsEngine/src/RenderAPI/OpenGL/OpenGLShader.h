#pragma once
#include "MarsHeader.h"
#include "RenderAPI/Buffers.h"
#include "Vender/glm/glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include "OpenGLErrorHandle.h"
#include "RenderAPI\Shader.h"

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            struct ShaderSource
            {
                std::string VertexShader;
                std::string FragmentShader;
            };

            class OpenGLShader : public Shader
            {
            private:
                unsigned int m_RendererID;
                std::unordered_map<std::string, int> m_UniformLocationCache;
                std::string m_Filepath;
                bool Bound = false;

                ShaderSource PharseShader(const std::string& filepath);
                unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
                unsigned int CompileShader(unsigned int type, const std::string& program);

            public:
                OpenGLShader(const std::string& filepath);
                ~OpenGLShader();

                void Bind() override;
                void unBind() override;

                void SetUniforms4f(const std::string& name, const float& f1, const float& f2, const float& f3, const float& f4) override;
                void SetUniforms3f(const std::string& name, const float* data) override;
                void SetUniforms1i(const  std::string& name, const int& data) override;
                void SetUniforms1f(const  std::string& name, const float& data) override;
                void SetUniformsMat4f(const std::string& name, const glm::mat4& matrix) override;
            };
        }
    }
}