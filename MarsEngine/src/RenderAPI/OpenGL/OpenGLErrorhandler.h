#pragma once
#include "MarsHeader.h"

#include "GL/glew.h"

#include <fstream>
#include <string>
#include <sstream>

#define GLLogCall(x)\
        {\
            GLClearError();\
            x;\
            GLPrintError(#x, __FILE__, __LINE__);\
        }
namespace ME
{
    namespace Renderer
    {
        namespace OpenGL
        {
            bool GLPrintError(const char* function, const char* srcfile, int codeline);
            void GLClearError();
            bool GLShaderError(const unsigned int& type, const unsigned int& shaderid);
            int GLValidUniformLocation(unsigned int& shader, const char* name);
        }
    }
}