#include "MarsHeader.h"

#include "Application.h"
int main(int, char**)
{
    ME_CORE_WARNING("MARS ENGINE STARTING....");
    ME::ME_MEMINIT();
    {
        auto app = ME::CreateApp();
        app->Run();
        ME::dealloc(app, sizeof(ME::Application));

        ME_CORE_WARNING("MARS ENGINE SHUTDOWN....");
        WriteFile("Data.XLS");
    }
    ME::ME_MEMCLEAR();
    return 0;
}