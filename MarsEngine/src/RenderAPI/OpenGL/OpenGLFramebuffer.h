#ifndef OPENGLFRAMEBUFFER
#define OPENGLFRAMEBUFFER

#pragma once

#include "RenderAPI/Buffers.h"

namespace ME
{
	namespace Renderer
	{
		namespace OpenGL
		{
			class OpenGLFramebuffer : public Framebuffer
			{
			public:
				OpenGLFramebuffer(const FramebufferSpecification& spec);
				~OpenGLFramebuffer();

				virtual void Bind();
				virtual void unBind();

				virtual void Resize(uint32_t Width, uint32_t Height) override;

				virtual unsigned int getColorAttachment() const override { return m_ColorAttachment; }
				virtual unsigned int getDepthAttachment() const override { return m_DepthAttachment; }

				void Invalidate();
			private:
				unsigned int m_RendererID, m_ColorAttachment, m_DepthAttachment;
				FramebufferSpecification m_Specification;
			};
		}
	}
}
#endif