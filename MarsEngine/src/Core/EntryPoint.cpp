#include "MarsHeader.h"

#include "Application.h"

#include "Core/Utilites/String.h"

int main(int, char**)
{
    ME_LOGINIT();
    ME_CORE_INFO("MARS ENGINE STARTING....");
    ME_MEM_INIT();
    {
        auto app = ME::Application::CreateApp();
        app->Run();
        ME::dealloc(app);
    }
    ME_MEM_CLEAR();
    ME_CORE_INFO("MARS ENGINE SHUTDOWN....");
    ME_LOGDEINIT();
    return 0;
}