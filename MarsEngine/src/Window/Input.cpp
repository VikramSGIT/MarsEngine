#include "MarsHeader.h"
#include "Input.h"

#include "Windows/WindowsInput.h"

namespace ME
{
	namespace Window
	{
		Input* Input::s_Instance = nullptr;

		Input* Input::Create()
		{
			switch (ME_PLATFORM)
			{
			case ME_WINDOWS:
				s_Instance = new WindowsInput();
				return s_Instance;
			default: ME_CORE_CRITICAL(true, "Unknow Platform");
			}
			return nullptr;
		}

	}
}
