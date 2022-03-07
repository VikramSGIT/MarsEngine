#include "LayerStack.h"
namespace ME
{
    namespace Window
    {
        namespace Layer
        {
            LayerStack::~LayerStack()
            {

                ME_PROFILE_TRACE_CALL();

                for (Ref<Layer> layer : m_layerstack)
                {
                    layer->OnDetach();
                    layer.reset();
                }
            }

            void LayerStack::PushLayer(Ref<Layer> layer)
            {

                ME_PROFILE_TRACE_CALL();

                m_layerstack.EmplaceBack(layer);
                layer->OnAttach();
                m_TotalLayers++;
            }

            void LayerStack::PushOverlay(Ref<Layer> overlay)
            {

                ME_PROFILE_TRACE_CALL();

                m_layerstack.Emplace(m_layerstack.begin(), overlay);
                overlay->OnAttach();
            }

            void LayerStack::PopLayer(Ref<Layer> layer)
            {

                ME_PROFILE_TRACE_CALL();

                auto it = std::find(m_layerstack.begin(), m_layerstack.begin() + m_TotalLayers, layer);
                if (it != m_layerstack.begin() + m_TotalLayers)
                    layer->OnDetach();
                    m_layerstack.Erase(it);
                    m_TotalLayers--;
            }
        }
    }
}