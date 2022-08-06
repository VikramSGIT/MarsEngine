#include "OpenGLTexture2D.h"
#include "GL/glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Vender/stb/stb_image.h"

namespace ME
{
    namespace OpenGL
    {
        OpenGLTexture2D::OpenGLTexture2D(const string filename)
            :m_RendererID(0), m_FilePath(filename), m_LocalBuffer(nullptr)
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

        }

        OpenGLTexture2D::~OpenGLTexture2D()
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glDeleteTextures(1, &m_RendererID));
            if (m_LocalBuffer)
                stbi_image_free(m_LocalBuffer);
        }
        void OpenGLTexture2D::Bind(unsigned int slot)
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glActiveTexture(GL_TEXTURE0 + slot));
            GLLogCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
        }

        void OpenGLTexture2D::SetData(void* data, const uint32_t& size)
        {
            if (m_LocalBuffer)
                stbi_image_free(m_LocalBuffer);

            m_LocalBuffer = data;
        }

    }
}

