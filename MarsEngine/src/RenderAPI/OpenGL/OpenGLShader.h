#pragma once
#include "MarsHeader.h"
#include "RenderAPI/Buffers.h"
#include "Vender/OGLM/oglm.h"
#include "GL/glew.h"
#include "OpenGLErrorHandle.h"
#include "RenderAPI\Shader.h"

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

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

            ShaderSource PharseShader(const std::string& filepath);
            unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
            unsigned int CompileShader(unsigned int type, const std::string& program);

        public:
            OpenGLShader(const std::string& filepath);
            ~OpenGLShader();

            void Bind() const override;
            void unBind() const override;

            void SetUniforms4f(const std::string& name, float f1, float f2, float f3, float f4) override;
            void SetUniforms1i(const std::string& name, int i1) override;
            void SetUniformsMat4f(const std::string& name, const oglm::Matrix4<float>& matrix) override;
        };
    }
}