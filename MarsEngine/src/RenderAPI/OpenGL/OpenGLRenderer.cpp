#include "OpenGLRenderer.h"
namespace Renderer
{
    namespace OpenGL
    {
        OpenGLRendererAPI::OpenGLRendererAPI()
            :RenderAPI(RenderAPItype::ME_RENDERER_OPENGL)
        {

            ME_PROFILE_TRACE_CALL();

        }

        OpenGLRendererAPI::~OpenGLRendererAPI()
        {

            ME_PROFILE_TRACE_CALL();

        }

        void OpenGLRendererAPI::SetClearColor(const oglm::vec4& color)
        {

            ME_PROFILE_TRACE_CALL();

            m_clearcolor.x = color.x;
            m_clearcolor.y = color.y;
            m_clearcolor.z = color.z;
            m_clearcolor.w = color.w;
            oglm::Normalize<oglm::vec4>(m_clearcolor, 256.0f);
        }

        void OpenGLRendererAPI::Init()
        {

            ME_PROFILE_TRACE_CALL();

            std::stringstream ss;
            if (glewInit() != GLEW_OK)
                ME_CORE_ERROR("Error : Can't Impliment GLEW");
            else
                ss << "Detected OpenGL Vesrion (using) : " << glGetString(GL_VERSION);
            ME_CORE_INFO(ss.str());

            GLLogCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            GLLogCall(glEnable(GL_BLEND));
        }

        void OpenGLRendererAPI::OnUpdate()
        {

            ME_PROFILE_TRACE_CALL();

            Clear();
        }

        void OpenGLRendererAPI::Clear() const
        {

            ME_PROFILE_TRACE_CALL();

            GLLogCall(glClearColor(m_clearcolor.x, m_clearcolor.y, m_clearcolor.z, m_clearcolor.w));
            GLLogCall(glClear(GL_COLOR_BUFFER_BIT));
        }

        void OpenGLRendererAPI::Draw(const VertexArray& va, const IndexBuffer& ib, const OpenGLShader& shader) const
        {

            ME_PROFILE_TRACE_CALL();

            shader.Bind();
            va.Bind();
            ib.Bind();
            GLLogCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        }

        bool OpenGLRendererAPI::SwitchAPI(const RenderAPItype api)
        {

            ME_PROFILE_TRACE_CALL();

            return true;
        }

        Layer::BasicLayer* OpenGLRendererAPI::GetLayer()
        {

            ME_PROFILE_TRACE_CALL();

            Layer::BasicLayer* layer = new Layer::BasicLayer();
            layer -> SetOnUpdate(std::bind(&OpenGLRendererAPI::OnUpdate , this));
            layer -> SetOnEvent(std::bind(&OpenGLRendererAPI::OnEvent,this, std::placeholders::_1));
            return layer;
        }

        void OpenGLRendererAPI::OnEvent(Event::Event& e)
        {

            ME_PROFILE_TRACE_CALL();

            #ifdef ME_DEBUG_SHOW_EVENTSTR
            ME_CORE_INFO(e.ToString());
            #endif
        }
    }
}