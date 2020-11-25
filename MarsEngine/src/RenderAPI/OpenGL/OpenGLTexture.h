#pragma once
#include "MarsHeader.h"

#include "OpenGLErrorHandle.h"
#include "GL/glew.h"

#include <string>

namespace Renderer
{
    namespace OpenGL
    {
        struct Dimension
        {
            int Width = 0;
            int Height = 0;
            int BPP = 0;
        };


        class OpenGLTexture
        {
        private:
            unsigned int m_RendererID;
            const std::string m_FilePath;
            unsigned char* m_LocalBuffer;
            Dimension m_Dimension;
            unsigned int Slot;
        public:
            OpenGLTexture(const std::string filepath);
            ~OpenGLTexture();

            void Bind(unsigned int slot = 0);
            void unBind() const;

            inline Dimension GetDimension() const { return m_Dimension; }
            inline const unsigned int GetSlot() const { return Slot; }
        };
    }
}