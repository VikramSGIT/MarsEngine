#include "MarsHeader.h"
#include "OpenGLShader.h"

#include "OpenGLErrorHandler.h"
#include "GL/glew.h"

#include <sstream>

namespace ME
{
    namespace OpenGL
    {
        OpenGLShader::OpenGLShader(const string& filepath)
            :m_RendererID(0), m_Filepath(filepath)
        {
            ME_PROFILE_TRACE_CALL();

            ShaderSource src = PharseShader(filepath);
            m_RendererID = CreateShader(src.VertexShader.c_str(), src.FragmentShader.c_str());
        }

        OpenGLShader::~OpenGLShader()
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glDeleteProgram(m_RendererID));
        }

        void OpenGLShader::SetUniforms1i(const string& name, const int& data)
        {

            ME_PROFILE_TRACE_CALL();

            int location;
            if (m_UniformLocationCache.find(name.c_str()) != m_UniformLocationCache.end())
                location = m_UniformLocationCache[name.c_str()];
            else
            {
                location = GLValidUniformLocation(m_RendererID, name.c_str());
                m_UniformLocationCache[name.c_str()] = location;
            }

            ME_CORE_ERROR(location == -1, "Cant pass the Uniforms!!")

            {
                Bind();
                GLLogCall(glUniform1i(location, data));
                unBind();
            }
        }

        void OpenGLShader::SetUniforms1f(const string& name, const float& data)
        {
            ME_PROFILE_TRACE_CALL();

            int location;
            if (m_UniformLocationCache.find(name.c_str()) != m_UniformLocationCache.end())
                location = m_UniformLocationCache[name.c_str()];
            else
            {
                location = GLValidUniformLocation(m_RendererID, name.c_str());
                m_UniformLocationCache[name.c_str()] = location;
            }

            ME_CORE_ERROR(location != -1, "Cant pass the Uniforms!!")

            {
                Bind();
                GLLogCall(glUniform1f(location, data));
                unBind();
            }
        }

        void OpenGLShader::SetUniformsMat4f(const string& name, const glm::mat4& matrix)
        {

            ME_PROFILE_TRACE_CALL();

            int location;
            if (m_UniformLocationCache.find(name.c_str()) != m_UniformLocationCache.end())
                location = m_UniformLocationCache[name.c_str()];
            else
            {
                location = GLValidUniformLocation(m_RendererID, name.c_str());
                m_UniformLocationCache[name.c_str()] = location;
            }
            if (location != -1)
            {
                Bind();
                GLLogCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
                unBind();
            }
            else
                std::cout << "Cant pass the Uniforms!!" << std::endl;
        }

        void OpenGLShader::SetUniforms4f(const string& name, const float& f1, const float& f2, const float& f3, const float& f4)
        {

            ME_PROFILE_TRACE_CALL();

            int location;
            if (m_UniformLocationCache.find(name.c_str()) != m_UniformLocationCache.end())
                location = m_UniformLocationCache[name.c_str()];
            else
            {
                location = GLValidUniformLocation(m_RendererID, name.c_str());
                m_UniformLocationCache[name.c_str()] = location;
            }
            if (location != -1)
            {
                Bind();
                GLLogCall(glUniform4f(location, f1, f2, f3, f4));
                unBind();
            }
            else
                std::cout << "Cant pass the Uniforms!!" << std::endl;
        }

        void OpenGLShader::SetUniforms3f(const string& name, const float* data)
        {

            ME_PROFILE_TRACE_CALL();

            int location;
            if (m_UniformLocationCache.find(name.c_str()) != m_UniformLocationCache.end())
                location = m_UniformLocationCache[name.c_str()];
            else
            {
                location = GLValidUniformLocation(m_RendererID, name.c_str());
                m_UniformLocationCache[name.c_str()] = location;
            }
            if (location != -1)
            {
                Bind();
                GLLogCall(glUniform1fv(location, 3, data));
                unBind();
            }
            else
                std::cout << "Cant pass the Uniforms!!" << std::endl;
        }

        void OpenGLShader::Bind()
        {

            ME_PROFILE_TRACE_CALL();
            GLLogCall(glUseProgram(m_RendererID));
        }

        void OpenGLShader::unBind()
        {

            ME_PROFILE_TRACE_CALL();
            GLLogCall(glUseProgram(0));
        }

        ShaderSource OpenGLShader::PharseShader(const string& filepath)
        {

            ME_PROFILE_TRACE_CALL();

            std::ifstream stream(filepath.c_str());

            ME_CORE_ERROR(!stream.good(), "Can't open Shader File!!");

            enum class ShaderType
            {
                NONE = -1, VERTEX = 0, FRAGMENT = 1
            };

            ShaderType Mode = ShaderType::NONE;


            std::string line;
            std::stringstream ss[2];
            while (getline(stream, line))
            {
                if (line.find("#shader") != std::string::npos)
                {
                    if (line.find("vertex") != std::string::npos)
                        Mode = ShaderType::VERTEX;
                    else if (line.find("fragment") != std::string::npos)
                        Mode = ShaderType::FRAGMENT;
                }
                else
                    ss[(int)Mode] << line << '\n';
            }
            return { ss[0].str().c_str(), ss[1].str().c_str() };
        }

        unsigned int OpenGLShader::CreateShader(const string& vertexShader, const string& fragmentShader)
        {

            ME_PROFILE_TRACE_CALL();

            unsigned int program = glCreateProgram();
            unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
            unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

            glAttachShader(program, vs);
            glAttachShader(program, fs);
            glLinkProgram(program);
            glValidateProgram(program);

            glDeleteShader(vs);
            glDeleteShader(fs);

            return program;
        }

        unsigned int OpenGLShader::CompileShader(unsigned int type, const string& program)
        {

            ME_PROFILE_TRACE_CALL();

            unsigned int id = glCreateShader(type);
            const char* c = program.c_str();

            glShaderSource(id, 1, &c, nullptr);
            glCompileShader(id);

            if (!GLShaderError(type, id))
            {
                glDeleteShader(id);
                return 0;
            }

            return id;
        }
        }
}
