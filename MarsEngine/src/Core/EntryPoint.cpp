#include "MarsHeader.h"

#include "Application.h"
#include "Entity.h"

#include "Core/Utilites/String.h"

int main(int, char**)
{
    ME_LOGINIT();
    ME_CORE_INFO("MARS ENGINE STARTING....");
    ME_MEMINIT();
    ME_ENTITYINIT();
    {
        ME::String s = "Hi";
        auto app = ME::CreateApp();
        app->Run();
        ME::dealloc(app, sizeof(ME::Application));
    }
    ME_ENTITYCLEAR();
    ME_MEMCLEAR();
    ME_CORE_INFO("MARS ENGINE SHUTDOWN....");
    ME_LOGDEINIT();
    return 0;
}