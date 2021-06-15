#include "MarsHeader.h"

#include "Application.h"
int main(int, char**)
{
    ME_LOGINIT();
    ME_CORE_WARNING("MARS ENGINE STARTING....");
    ME_MEMINIT();
    {
        auto app = ME::CreateApp();
        app->Run();
        ME::dealloc(app, sizeof(ME::Application));

        WriteFile("Data.XLS");
    }
    ME_MEMCLEAR();
    ME_CORE_WARNING("MARS ENGINE SHUTDOWN....");
    ME_LOGDEINIT();
    return 0;
}