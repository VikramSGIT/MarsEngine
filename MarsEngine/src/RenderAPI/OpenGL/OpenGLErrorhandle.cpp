#include "OpenGLErrorHandle.h"

namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            static std::fstream file("include/Gl/glew.h");
            static const char HEX[16] =
            {
                '0',
                '1',
                '2',
                '3',
                '4',
                '5',
                '6',
                '7',
                '8',
                '9',
                'A',
                'B',
                'C',
                'D',
                'E',
            };


            static std::string IntToHex(const int& input)
            {
                std::string out;

                //count is been added to convert i to 4-digit hexadecimal

                int i = input, count = 4;

                while (i)
                {
                    out += HEX[i % 16];
                    i = i / 16;
                    count--;
                }

                for (int zero = 0; zero < count; zero++)
                    out += '0';

                reverse(out.begin(), out.end());
                return out;
            }

            bool GLPrintError(const char* function, const char* srcfile, int codeline)
            {
                std::string line, hex;
                GLenum error;
                std::stringstream ss;

                while ((error = glGetError()) != GL_NO_ERROR)
                {
                    hex = IntToHex((int)error);
                    while (getline(file, line))
                    {
                        if (line.find(hex) != std::string::npos)
                        {
                            ss << "[OpenGl ERROR] :"
                                << "\n Function : " << function
                                << "\n at line  : " << codeline
                                << "\n in file  : " << srcfile
                                << "\n Error    : " << line.erase(0, 8)
                                << "\n";
                            ME_CORE_CRITICAL(ss.str());
                            return false;
                        }
                    }
                }
                return true;
            }

            void GLClearError()
            {
                while (glGetError() != GL_NO_ERROR);
            }

            bool GLShaderError(const unsigned int& type, const unsigned int& shaderid)
            {
                int result, len;
                glGetShaderiv(shaderid, GL_COMPILE_STATUS, &result);
                if (result == GL_FALSE)
                {
                    glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &len);

                    char* msg = (char*)malloc(len);
                    glGetShaderInfoLog(shaderid, len, &len, msg);

                    //Print Shader Error

                    std::cout << "[OpenGl ERROR] : \n Error in Compiling Shader\n " <<
                        (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
                        << " Shader : " << msg << std::endl;
                    return false;
                }
                return true;
            }

            int GLValidUniformLocation(unsigned int& shader, const char* name)
            {
                int location;
                GLLogCall(location = glGetUniformLocation(shader, name));
                if (location != -1)
                    return location;
                else
                {
                    std::cout << "[OpenGL ERROR] : "
                        << "\n Warning : Invalid Uniform Location " << location
                        << "\n Uniform variable : " << name
                        << std::endl;
                    return -1;
                }
            }
        }
    }
}