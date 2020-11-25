#pragma once

#define ME_DEBUG_SHOW_EVENTSTR

#include "Utils\Intrumentor.h"

#include <memory>

#ifndef NOMINMAX
#define NOMINMAX
#endif


template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T>
using Scope = std::shared_ptr<T>;
template<typename T, typename... Args>
Ref<T> CreateRef(Args&& ... args) { return std::make_shared<T>(std::forward<Args>(args)...); }
template<typename T, typename ... Args>
Scope<T> CreateScope(Args&& ...args) { return std::make_unique<T>(std::forward<Args>(args)...); }

#include "Core/Logger.h"