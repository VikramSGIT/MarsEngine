#include "MarsHeader.h"

#include "Application.h"
int main(int, char**)
{
    ME_CORE_WARNING("MARS ENGINE STARTING....");
    auto app = ME::CreateApp();
    app->Run();
    delete app;
    ME_CORE_WARNING("MARS ENGINE SHUTDOWN....");
    WriteFile("Data.XLS");

    return 0;
}