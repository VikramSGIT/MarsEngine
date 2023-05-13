#ifndef ME_OPENGLSHADER
#define ME_OPENGLSHADER

#pragma once

#include "Vender/glm/glm/gtc/matrix_transform.hpp"
#include "RenderAPI/Shader.h"

#include <unordered_map>
#include <fstream>

namespace ME
{
    namespace OpenGL
    {
        struct ShaderSource
        {
            std::string VertexShader, FragmentShader;
        };

        class OpenGLShader : public Shader
        {
        private:
            unsigned int m_RendererID;
            string m_Filepath;
            std::unordered_map<std::string, unsigned int> m_UniformLocationCache;

            ShaderSource PharseShader(const string& filepath);
            unsigned int CreateShader(const string& vertexShader, const string& fragmentShader);
            unsigned int CompileShader(unsigned int type, const string& program);

        public:
            OpenGLShader(const string& filepath);
            ~OpenGLShader();

            void Bind() override;
            void unBind() override;

            void SetUniforms4f(const string& name, const float& f1, const float& f2, const float& f3, const float& f4) override;
            void SetUniforms3f(const string& name, const float* data) override;
            void SetUniforms1i(const string& name, const int& data) override;
            void SetUniforms1f(const string& name, const float& data) override;
            void SetUniformsMat4f(const string& name, const glm::mat4& matrix) override;
        };
    }
}
#endif