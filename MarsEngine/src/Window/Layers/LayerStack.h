#pragma once
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
                Vector<Ref<Layer>> m_layerstack;
                size_t m_TotalLayers = 0;
            public:
                LayerStack() = default;
                ~LayerStack();

                void PushLayer(Ref<Layer> layer);
                void PushOverlay(Ref<Layer> overlay);
                void PopLayer(Ref<Layer> layer);
                void PopOverlay(Ref<Layer> overlay);

                Ref<Layer>* begin() { return m_layerstack.begin(); }
                Ref<Layer>* end() { return m_layerstack.end(); }
                Ref<Layer>* rbegin() { return m_layerstack.rbegin(); }
                Ref<Layer>* rend() { return m_layerstack.rend(); }

                Ref<Layer>* begin() const { return m_layerstack.begin(); }
                Ref<Layer>* end() const { return m_layerstack.end(); }
                Ref<Layer>* rbegin() const { return m_layerstack.rbegin(); }
                Ref<Layer>* rend() const { return m_layerstack.rend(); }

                inline size_t GetTotalLayers() const { return m_TotalLayers; }
            };
        }
    }
}