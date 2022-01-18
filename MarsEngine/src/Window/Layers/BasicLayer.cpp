#include "BasicLayer.h"
namespace ME
{
    namespace Layer
    {
        void EmptyLayer::SetOnAttach(std::function<void()> func)
        {

            ME_PROFILE_TRACE_CALL();

            onattach = std::bind(func);
        }

        void EmptyLayer::SetOnDetach(std::function<void()> func)
        {

            ME_PROFILE_TRACE_CALL();

            ondetach = std::bind(func);
        }

        void EmptyLayer::SetOnUpdate(std::function<void()> func)
        {

            ME_PROFILE_TRACE_CALL();

            onupdate = std::bind(func);
        }

        void EmptyLayer::SetOnEvent(std::function<void(Event::Event&)> func)
        {

            ME_PROFILE_TRACE_CALL();

            onevent = std::bind(func, std::placeholders::_1);
        }

        void EmptyLayer::OnAttach()
        {

            ME_PROFILE_TRACE_CALL();

            onattach;
        }

        void EmptyLayer::OnDetach()
        {

            ME_PROFILE_TRACE_CALL();

            ondetach;
        }

        void EmptyLayer::OnUpdate(Timestep ts)
        {

            ME_PROFILE_TRACE_CALL();

            onupdate;
        }

        void EmptyLayer::OnEvent(Event::Event& e)
        {

            ME_PROFILE_TRACE_CALL();

            onevent(e);
        }
    }
}
