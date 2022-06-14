#ifndef ME_IMGUILAYER
#define ME_IMGUILAYER

#pragma

#include "Window/Layers/Layer.h"

#include <functional>
namespace ME
{
    namespace Window
    {
        class imguiLayer : public Layer
        {
        private:
            bool m_BlockEvents = true;
            bool m_Show = true;
            std::function<void()> draw;

        public:
            imguiLayer();
            ~imguiLayer();

            void OnAttach() override;
            void OnDetach() override;
            void OnUpdate(Timestep ts) override;
            void OnEvent(Event::Event& e) override;
            void OnDraw() override;
            void SetDrawData(const std::function<void()>& func);

            void SetBlockEvents(bool block) { m_BlockEvents = !block; }
        };

    }
}
        
#endif