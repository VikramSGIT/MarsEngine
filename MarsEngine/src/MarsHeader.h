#pragma once

#define ME_IMGUI
//#define ME_DEBUG_SHOW_EVENT
//#define ME_PROFILE_TRACE_CALLFUNC
#define ME_MAX_VERTEX_BUFFER_SIZE 1000
#define ME_MAX_INDEX_BUFFER_SIZE 200

#include "Utils\Intrumentor.h"

#include <memory>

#ifndef NOMINMAX
#define NOMINMAX
#endif


template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename... Args>
Ref<T> CreateRef(Args&& ... args) { return std::make_shared<T>(std::forward<Args>(args)...); }
template<typename T, typename ... Args>
Scope<T> CreateScope(Args&& ...args) { return std::make_unique<T>(std::forward<Args>(args)...); }

#include "Core/Logger.h"