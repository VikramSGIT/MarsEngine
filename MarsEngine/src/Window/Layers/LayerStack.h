#ifndef ME_LAYERSTACK
#define ME_LAYERSTACK

#include "MarsHeader.h"

#include "Layer.h"
#include "Core/Memory/Vector.h"

#include <algorithm>
#include <functional>
namespace ME
{
    namespace Window
    {
        namespace Layer
        {
            class LayerStack
            {
            private:
                std::vector<Ref<Layer>> m_layerstack;
                size_t m_TotalLayers = 0;
            public:
                LayerStack() = default;
                ~LayerStack();

                void PushLayer(Ref<Layer> layer);
                void PushOverlay(Ref<Layer> overlay);
                void PopLayer(Ref<Layer> layer);
                void PopOverlay(Ref<Layer> overlay);

                auto begin() { return m_layerstack.begin(); }
                auto end() { return m_layerstack.end(); }

                auto begin() const { return m_layerstack.begin(); }
                auto end() const { return m_layerstack.end(); }

                inline size_t GetTotalLayers() const { return m_TotalLayers; }
            };
        }
    }
}

#endif