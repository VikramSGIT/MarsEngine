#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Vender/stb/stb_image.h"

namespace Renderer
{
    namespace OpenGL
    {
        OpenGLTexture::OpenGLTexture(const std::string filename)
            :m_RendererID(0), m_FilePath(filename), m_LocalBuffer(nullptr), Slot(NULL)
        {

            ME_PROFILE_TRACE_CALL();

            stbi_set_flip_vertically_on_load(1);
            m_LocalBuffer = stbi_load(filename.c_str(), &m_Dimension.Width, &m_Dimension.Height, &m_Dimension.BPP, 4);

            GLLogCall(glGenTextures(1, &m_RendererID));
            GLLogCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

            GLLogCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            GLLogCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            GLLogCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
            GLLogCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));

            GLLogCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Dimension.Width, m_Dimension.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
            GLLogCall(glBindTexture(GL_TEXTURE_2D, 0));

            if (m_LocalBuffer)
                stbi_image_free(m_LocalBuffer);
        }

        OpenGLTexture::~OpenGLTexture()
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glDeleteTextures(1, &m_RendererID));
        }

        void OpenGLTexture::Bind(unsigned int slot)
        {

            ME_PROFILE_TRACE_CALL();

            Slot = slot;
            GLLogCall(glActiveTexture(GL_TEXTURE0 + Slot));
            GLLogCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
        }

        void OpenGLTexture::unBind() const
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glBindTexture(GL_TEXTURE_2D, 0));
        }
    }
}

