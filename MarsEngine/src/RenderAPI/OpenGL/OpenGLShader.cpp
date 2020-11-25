#include "OpenGLShader.h"

namespace Renderer
{
    namespace OpenGL
    {
        OpenGLShader::OpenGLShader(const std::string& filepath)
            :Filepath(filepath), m_RendererID(0)
        {

            ME_PROFILE_TRACE_CALL();

            ShaderSource src = PharseShader(filepath);
            m_RendererID = CreateShader(src.VertexShader, src.FragmentShader);

        }

        OpenGLShader::~OpenGLShader()
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glDeleteProgram(m_RendererID));
        }

        void OpenGLShader::SetUniforms1i(const std::string& name, int i1)
        {

            ME_PROFILE_TRACE_CALL();

            int location;
            if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
                location = m_UniformLocationCache[name];
            else
            {
                location = GLValidUniformLocation(m_RendererID, name.c_str());
                m_UniformLocationCache[name] = location;
            }
            if (location != -1)
            {
                GLLogCall(glUniform1i(location, i1));
            }
            else
                std::cout << "Cant pass the Uniforms!!" << std::endl;
        }

        void OpenGLShader::SetUniformsMat4f(const std::string& name, const oglm::Matrix4& matrix)
        {

            ME_PROFILE_TRACE_CALL();

            int location;
            if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
                location = m_UniformLocationCache[name];
            else
            {
                location = GLValidUniformLocation(m_RendererID, name.c_str());
                m_UniformLocationCache[name] = location;
            }
            if (location != -1)
            {
                GLLogCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix.mat[0][0]));
            }
            else
                std::cout << "Cant pass the Uniforms!!" << std::endl;
        }

        void OpenGLShader::SetUniforms4f(const std::string& name, float f1, float f2, float f3, float f4)
        {

            ME_PROFILE_TRACE_CALL();

            int location;
            if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
                location = m_UniformLocationCache[name];
            else
            {
                location = GLValidUniformLocation(m_RendererID, name.c_str());
                m_UniformLocationCache[name] = location;
            }
            if (location != -1)
            {
                GLLogCall(glUniform4f(location, f1, f2, f3, f4));
            }
            else
                std::cout << "Cant pass the Uniforms!!" << std::endl;
        }

        void OpenGLShader::Bind() const
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glUseProgram(m_RendererID));
        }

        void OpenGLShader::unBind() const
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glUseProgram(0));
        }

        ShaderSource OpenGLShader::PharseShader(const std::string& filepath)
        {

            ME_PROFILE_TRACE_CALL();

            std::ifstream stream(filepath);

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
                    ss[(int)Mode] << line << "\n";
            }
            return { ss[0].str(), ss[1].str() };
        }

        unsigned int OpenGLShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

        unsigned int OpenGLShader::CompileShader(unsigned int type, const std::string& program)
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
