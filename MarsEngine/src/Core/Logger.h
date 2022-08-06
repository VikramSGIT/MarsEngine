#ifndef ME_LOGGER
#define ME_LOGGER

#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"
#include "spdlog/fmt/ostr.h"

static void InitLogger() 
{
    spdlog::cfg::load_env_levels();
    spdlog::set_pattern("[%H:%M:%S  %t] [%l] %v");
}
static void DeinitLogger() noexcept { }

#define ME_LOGINIT() InitLogger()
#define ME_LOGDEINIT() DeinitLogger()

#define ME_CORE_FILTER //if(msg[0] == 'R' || msg[0] == 'P')
#define ME_CORE_MSG(...) spdlog::log(spdlog::level::off, __VA_ARGS__)
#define ME_CORE_INFO(...) spdlog::info(__VA_ARGS__)
#define ME_CORE_WARNING(...) spdlog::warn(__VA_ARGS__)
#define ME_CORE_ERROR(CND, ...) if(CND) { spdlog::error(__VA_ARGS__); }
#define ME_CORE_CRITICAL(CND, ...) if(CND) { spdlog::critical(__VA_ARGS__); throw "Critical"; }

#endif