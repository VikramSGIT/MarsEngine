#include "MarsHeader.h"
#include "Event.h"

#include "Core/Application.h"
namespace ME
{
	namespace Event
	{
		void throwEvent(Event& e)
		{
			Application::GetInstance().OnEvent(e);
		}
	}
}
