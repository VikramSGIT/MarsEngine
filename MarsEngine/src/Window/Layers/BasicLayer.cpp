#include "BasicLayer.h"
namespace ME
{
    namespace Layer
    {
        void BasicLayer::SetOnAttach(std::function<void()> func)
        {

            ME_PROFILE_TRACE_CALL();

            onattach = std::bind(func);
        }

        void BasicLayer::SetOnDetach(std::function<void()> func)
        {

            ME_PROFILE_TRACE_CALL();

            ondetach = std::bind(func);
        }

        void BasicLayer::SetOnUpdate(std::function<void()> func)
        {

            ME_PROFILE_TRACE_CALL();

            onupdate = std::bind(func);
        }

        void BasicLayer::SetOnEvent(std::function<void(Event::Event&)> func)
        {

            ME_PROFILE_TRACE_CALL();

            onevent = std::bind(func, std::placeholders::_1);
        }

        void BasicLayer::OnAttach()
        {

            ME_PROFILE_TRACE_CALL();

            onattach;
        }

        void BasicLayer::OnDetach()
        {

            ME_PROFILE_TRACE_CALL();

            ondetach;
        }

        void BasicLayer::OnUpdate()
        {

            ME_PROFILE_TRACE_CALL();

            onupdate;
        }

        void BasicLayer::OnEvent(Event::Event& e)
        {

            ME_PROFILE_TRACE_CALL();

            onevent(e);
        }
    }
}
