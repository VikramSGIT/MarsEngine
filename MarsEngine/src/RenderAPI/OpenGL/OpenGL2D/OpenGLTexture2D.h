#ifndef ME_OPENGLTEXTURE
#define ME_OPENGLTEXTURE

#pragma once

#include "MarsHeader.h"

#include "RenderAPI/OpenGL/OpenGLErrorHandler.h"
#include "RenderAPI/Texture.h"
#include "Core/Utilites/String.h"

namespace ME
{
    namespace OpenGL
    {
        class OpenGLTexture2D : public Texture2D
        {
        public:
            OpenGLTexture2D(const string filepath);
            virtual ~OpenGLTexture2D();
            virtual void Bind(unsigned int slot = 0) override;
            virtual void SetData(void* data, const uint32_t& size) override;
            virtual TextureDimensions GetDimensions() const override { return m_Dimension; }

        private:
            unsigned int m_RendererID;
            void* m_LocalBuffer;
            TextureDimensions m_Dimension;
            string m_FilePath;
        };
    }
}

#endif