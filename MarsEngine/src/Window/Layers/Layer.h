#ifndef ME_LAYER
#define ME_LAYER

#pragma once

#include "Window/Events/Event.h"
#include "Core/Utilites/TimeStep.h"
#include "Vender/MTL/String.h"
#include "Vender/MTL/Vector.h"
#include "Vender/MTL/Ref.h"

namespace ME
{
    namespace Window
    {
        class Layer
        {
        public:
            Layer(const string& name)
                :m_LayerName(name) {}
            virtual ~Layer() = default;

            virtual void OnAttach() {};
            virtual void OnDetach() {};
            virtual void OnUpdate(Timestep) {};
            virtual void OnDraw() {};
            virtual void OnEvent(Event::Event&) {};

            inline string GetName() const { return m_LayerName; }
        private:
            string m_LayerName;
        };

        class IOLayer
        {
        public:
            virtual void OnIOUpdate() {}
        };

        class LayerStack
        {
        private:
            vector<ref<Layer>> m_layerstack;
        public:
            LayerStack() = default;
            ~LayerStack();

            void PushOverlay(const Ref<Layer>& overlay);
            void PushLayer(const Ref<Layer>& layer);
            void PopLayer(const Ref<Layer>& layer);

            void clear() { m_layerstack.clear(); }
            auto begin() { return m_layerstack.begin(); }
            auto end() { return m_layerstack.end(); }

            auto begin() const { return m_layerstack.begin(); }
            auto end() const { return m_layerstack.end(); }

            inline size_t GetTotalLayers() const { return m_layerstack.size(); }
        };
    }
}

#endif