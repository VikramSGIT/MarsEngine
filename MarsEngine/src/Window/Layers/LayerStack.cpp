#include "MarsHeader.h"
#include "Layer.h"

#include <algorithm>
namespace ME
{
    namespace Window
    {
        LayerStack::~LayerStack()
        {

            ME_PROFILE_TRACE_CALL();

            for (Ref<Layer> layer : m_layerstack)
                layer->OnDetach();
        }

        void LayerStack::PushLayer(const Ref<Layer>& layer)
        {

            ME_PROFILE_TRACE_CALL();

            Ref<Layer> ref = layer;
            m_layerstack.emplace_back(layer);
            ref->OnAttach();
        }

        void LayerStack::PushOverlay(const Ref<Layer>& overlay)
        {

            ME_PROFILE_TRACE_CALL();

            Ref<Layer> ref = overlay;
            m_layerstack.emplace(m_layerstack.begin(), ref);
            ref->OnAttach();
        }

        void LayerStack::PopLayer(const Ref<Layer>& layer)
        {

            ME_PROFILE_TRACE_CALL();

            Ref<Layer> ref = layer;
            auto it = std::find(m_layerstack.begin(), m_layerstack.end(), layer);
            if (it != m_layerstack.end())
            {
                ref->OnDetach();
                m_layerstack.erase(it);
            }
        }
    }
}