#ifndef ME_BUFFERS
#define ME_BUFFERS

#pragma once

#include "Core/Utilites/Ref.h"

#include <vector>

namespace ME
{
    namespace Renderer
    {
        unsigned int GetElementSize(unsigned int type);

        enum class Type
        {
            NONE = 0,
            VERTICES,
            TEXTURECOORD,
            INDEX
        };

        struct FramebufferSpecification
        {
            uint32_t Height = 1280, Width = 720, Samples = 1;
            bool SwapChainTarget = false, IncludeDepthbuffer = false;
        };

        class Framebuffer
        {
        public:

            virtual void Bind() = 0;
            virtual void unBind() = 0;

            virtual void Resize(uint32_t Width, uint32_t Height) = 0;

            virtual unsigned int getColorAttachment() const = 0;
            virtual unsigned int getDepthAttachment() const = 0;

            static Ref<Framebuffer> Create(const FramebufferSpecification&);
        };
        //
        // OpenGL stuffs need to be hidden
        //
        struct VertexbufferElement
        {
            unsigned int type;
            unsigned int count;
            char normalized;
        };

        class VertexbufferLayout
        {
        private:
            std::vector<VertexbufferElement> m_Elements;
            unsigned int m_Stride;
        public:
            VertexbufferLayout()
                : m_Stride(0) {};

            ~VertexbufferLayout() = default;

            void push(unsigned int type, unsigned int count);

            inline const std::vector<VertexbufferElement> GetElements() const { return m_Elements; }

            inline const unsigned int GetStride() const { return m_Stride; }
            inline const unsigned int GetTotalCount() const { return m_Stride / sizeof(ME_DATATYPE); }
        };
    }
}

#endif