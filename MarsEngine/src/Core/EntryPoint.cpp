#include "MarsHeader.h"

#include "Application.h"
#include "Entity.h"

#include "Core/Utilites/String.h"

int main(int, char**)
{
<<<<<<< HEAD
    ME_CORE_WARNING("MARS ENGINE STARTING....");
    ME_CORE_INFO("MADE WITH MARSENGINE");
    auto app = new Application;
    app->Run();
    delete app;
    ME_CORE_WARNING("MARS ENGINE SHUTDOWN....");
    WriteFile("Data.XLS");

=======
    ME_LOGINIT();
    ME_CORE_INFO("MARS ENGINE STARTING....");
    ME_MEM_INIT();
    ME_ENTITYINIT();
    {
        auto app = ME::Application::CreateApp();
        app->Run();
        ME::dealloc(app);
    }
    ME_ENTITYCLEAR();
    ME_MEM_CLEAR();
    ME_CORE_INFO("MARS ENGINE SHUTDOWN....");
    ME_LOGDEINIT();
>>>>>>> origin/master
    return 0;
}