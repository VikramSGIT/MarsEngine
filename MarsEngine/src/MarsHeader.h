#ifndef ME_MARSHEADER
#define ME_MARSHEADER

//#define ME_IMGUI
#define ME_DATATYPE float
//#define ME_DEBUG_SHOW_EVENT
//#define ME_PROFILE_TRACE_CALLFUNC
//#define ME_TRACE_LVL_2
#define ME_MAX_VERTEX_BUFFER_SIZE 1000 * sizeof(ME_DATATYPE) * 24
#define ME_MAX_INDEX_BUFFER_SIZE 200 * sizeof(unsigned int) * 6

#include "Engine Utilites/Instrumentor.h"

#include "Core/Memory/MemoryManager.h"
#include "Core/Logger.h"

#endif