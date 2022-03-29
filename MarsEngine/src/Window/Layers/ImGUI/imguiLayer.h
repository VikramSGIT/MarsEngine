#ifndef ME_IMGUILAYER
#define ME_IMGUILAYER

#include "MarsHeader.h"

#include "Window/Layers/Layer.h"

#include <memory>
#include <functional>
namespace ME
{
    namespace Window
    {
        namespace Layer
        {
            class imguiLayer : public Layer
            {
            private:
                ME_DATATYPE m_Time;
                bool m_BlockEvents = true;
                bool m_Show = true;
                std::function<void()> draw;

            public:
                imguiLayer();
                ~imguiLayer();

                void OnAttach() override;
                void OnDetach() override;
                void OnUpdate(Timestep ts) override;
                void OnEvent(Event::Event& e);
                void SetDrawData(const std::function<void()>& func);

                void SetBlockEvents(bool block) { m_BlockEvents = !block; }
            };

        }
    }
}
        
#endif