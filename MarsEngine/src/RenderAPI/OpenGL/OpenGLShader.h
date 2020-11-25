#pragma once
#include "MarsHeader.h"
#include "RenderAPI/Buffers.h"

#include "Core/oglm.h"
#include "GL/glew.h"
#include "OpenGLErrorHandle.h"

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

        class OpenGLShader
        {
        private:
            unsigned int m_RendererID;
            std::string Filepath;
            std::unordered_map<std::string, int> m_UniformLocationCache;

            ShaderSource PharseShader(const std::string& filepath);
            unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
        public:
            OpenGLShader(const std::string& filepath);
            ~OpenGLShader();

            unsigned int CompileShader(unsigned int type, const std::string& program);
            void Bind() const;
            void unBind() const;

            void SetUniforms4f(const std::string& name, float f1, float f2, float f3, float f4);
            void SetUniforms1i(const std::string& name, int i1);
            void SetUniformsMat4f(const std::string& name, const oglm::Matrix4& matrix);
        };
    }
}