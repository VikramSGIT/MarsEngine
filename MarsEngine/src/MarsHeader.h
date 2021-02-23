#pragma once

//#define ME_IMGUI
#define ME_DATATYPE float
//#define ME_DEBUG_SHOW_EVENT
//#define ME_PROFILE_TRACE_CALLFUNC
#define ME_MAX_VERTEX_BUFFER_SIZE 1000 * sizeof(ME_DATATYPE) * 24
#define ME_MAX_INDEX_BUFFER_SIZE 200 * sizeof(unsigned int) * 6

#include "Utils/Intrumentor.h"
#include "Vender/glm/glm/glm.hpp"
#include "Vender/glm/glm/mat4x4.hpp"
#include "Vender/glm/glm/vec4.hpp"
#include "Vender/glm/glm/vec3.hpp"
#include "Vender/glm/glm/vec2.hpp"

#include <memory>

#ifndef NOMINMAX
#define NOMINMAX
#endif
namespace ME
{
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename... Args>
	Ref<T> CreateRef(Args&& ... args) { return std::make_shared<T>(std::forward<Args>(args)...); }
	template<typename T, typename ... Args>
	Scope<T> CreateScope(Args&& ...args) { return std::make_unique<T>(std::forward<Args>(args)...); }
}


#include "Core/Logger.h"