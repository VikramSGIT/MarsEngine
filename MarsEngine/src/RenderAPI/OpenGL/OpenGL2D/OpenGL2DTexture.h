#ifndef ME_OPENGL2DTEXTURE
#define ME_OPENGL2DTEXTURE

#pragma once

#include "MarsHeader.h"

#include "RenderAPI\OpenGL\OpenGLErrorhandler.h"
#include "GL/glew.h"

#include <string>

namespace ME
{
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


            class OpenGL2DTexture
            {
            private:
                unsigned int m_RendererID;
                const std::string m_FilePath;
                unsigned char* m_LocalBuffer;
                Dimension m_Dimension;
                unsigned int Slot;
            public:
                OpenGL2DTexture(const std::string filepath);
                ~OpenGL2DTexture();

                void Bind(unsigned int slot = 0);
                void unBind() const;

                inline Dimension GetDimension() const { return m_Dimension; }
                inline const unsigned int GetSlot() const { return Slot; }
            };
        }
    }
}

#endif