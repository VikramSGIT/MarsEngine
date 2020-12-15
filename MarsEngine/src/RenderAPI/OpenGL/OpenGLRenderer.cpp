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

        void OpenGLRendererAPI::SetViewPortSize(const unsigned int& X, const unsigned int& Y)
        {
            glViewport(0, 0, X, Y);
        }

        void OpenGLRendererAPI::SetClearColor(const oglm::vec4<float>& color)
        {

            ME_PROFILE_TRACE_CALL();

            m_clearcolor.x = color.x;
            m_clearcolor.y = color.y;
            m_clearcolor.z = color.z;
            m_clearcolor.w = color.w;
            oglm::Normalize<oglm::vec4<float>>(m_clearcolor, 256.0f);
        }

        void OpenGLRendererAPI::Init()
        {

            ME_PROFILE_TRACE_CALL();
//
// Initiating graphics libraries
// Need to add graphics drivers identification
            std::stringstream ss;
            if (glewInit() != GLEW_OK)
                ME_CORE_ERROR("Can't Impliment GLEW");
            else
                ss << "Detected OpenGL Vesrion (using) : " << glGetString(GL_VERSION);
            ME_CORE_INFO(ss.str());
//
// Enabling blending, typically transparencies
//
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

        void OpenGLRendererAPI::AddRenderSubmition(const MeshQueue& meshqueue)
        {
            m_RenderQueue.push_back(meshqueue);
        }

        void OpenGLRendererAPI::Draw(const Shader& shader)
        {

            ME_PROFILE_TRACE_CALL();
//
// Will Soon Sort out the shaders when Materials Are Implemented
//
            shader.Bind();
//
// Constructing buffers for each meshqueue
//
            for (MeshQueue mq : m_RenderQueue)
            {
                if (!Ready)
                {
                    if (LiveStreamData)
                    {
//
// Heap allocating buffers using (known sizes)
// 
                        Scope<float[]> vertexbuffer = CreateScope<float[]>(mq.GetTotalVertices());
                        Scope<unsigned int[]> indexbuffer = CreateScope<unsigned int[]>(mq.GetTotalIndices());

                        unsigned int voffset = 0, ioffset = 0;
                        unsigned int indexoffset = 0;
                        for (Mesh ms : mq)
                        {
//
// Filling up of vertexbuffer with datas
//
                            for (int j = 0; j < ms.GetVertices().size() * 3; j += 3)
                            {
                                vertexbuffer[j + voffset] = ms.GetVertices()[j / 3].x;
                                vertexbuffer[j + 1 + voffset] = ms.GetVertices()[j / 3].y;
                                vertexbuffer[j + 2 + voffset] = ms.GetVertices()[j / 3].z;
                            }
                            voffset += ms.GetVertices().size() * 3;
//                  
// Filling up indexbuffer with data with maintaining offsets of indices
//
                            for (int j = 0; j < ms.GetIndices().size(); j++)
                            {
                                indexbuffer[j + ioffset] = ms.GetIndices()[j] + indexoffset;
                            }
                            ioffset += ms.GetIndices().size();
                            indexoffset += *std::max_element(ms.GetIndices().begin(), ms.GetIndices().end()) + 1;
                            Ready = true;
                        }
                        vertex->BufferPostRenderData(vertexbuffer.get(), mq.GetTotalVertices(), 0);
                        index->BufferPostRenderData(indexbuffer.get(), mq.GetTotalIndices(), 0);
                    }
//
// Constructing buffers and drawing them
// 
                    Ref<VertexArray> array = CreateRef<OpenGLVertexArray>();
                    vertex->Bind();
                    array->AddBuffer(*vertex, *mq.GetLayout());
                    index->Bind();

                    GLLogCall(glDrawElements(GL_TRIANGLES, mq.GetTotalIndices(), GL_UNSIGNED_INT, nullptr));

                    vertex->unBind();
                    index->unBind();
                }
            }
        }

        bool OpenGLRendererAPI::SwitchAPI(const RenderAPItype api)
        {

            ME_PROFILE_TRACE_CALL();

            return true;
        }

        Ref<Layer::BasicLayer> OpenGLRendererAPI::GetLayer()
        {

            ME_PROFILE_TRACE_CALL();

            Ref<Layer::BasicLayer> layer = CreateRef<Layer::BasicLayer>();
            layer -> SetOnUpdate(std::bind(&OpenGLRendererAPI::OnUpdate , this));
            layer -> SetOnEvent(std::bind(&OpenGLRendererAPI::OnEvent,this, std::placeholders::_1));
            return layer;
        }

        void OpenGLRendererAPI::OnEvent(Event::Event& e)
        {

            ME_PROFILE_TRACE_CALL();
//
// Logs out missed event handles
//
            #ifdef ME_DEBUG_SHOW_EVENT
            ME_CORE_INFO(e.ToString());
            #endif
        }
    }
}