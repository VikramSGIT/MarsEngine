#include "MarsHeader.h"

#include "Application.h"
int main(int , char**)
{

    ME_PROFILE_TRACE_CALL();

    ME_CORE_WARNING("MARS ENGINE STARTING....");
    auto app = new Application;
    app->Run();
    delete app;
    ME_CORE_WARNING("MARS ENGINE SHUTDOWN....");

    return 1;
}
