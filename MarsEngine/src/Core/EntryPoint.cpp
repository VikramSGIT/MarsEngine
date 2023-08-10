#include "MarsHeader.h"

#include "Application.h"

#include "Vender/MTL/String.h"

int main(int, char**)
{
    ME_LOGINIT();
    ME_CORE_INFO("MARS ENGINE STARTING....");
    InitAllocator();
    {
        auto app = ME::Application::CreateApp();
        app->Run();
        ME::dealloc(app);
    }
    DeInitAllocator();
    ME_CORE_INFO("MARS ENGINE SHUTDOWN....");
    ME_LOGDEINIT();
    return 0;
}