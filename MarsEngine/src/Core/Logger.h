#ifndef ME_LOGGER
#define ME_LOGGER

#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"
#include "spdlog/fmt/ostr.h"

class Logger
{
private:
    time_t now = std::time(0);
    tm* m_Time = localtime(&now);

    bool m_setlog = true;
public:

    Logger() { spdlog::cfg::load_env_levels(); }

    void LogNormal(const std::string& msg) { spdlog::log(spdlog::level::off, msg); }
    void LogInfo(const std::string& msg) { spdlog::info(msg); }
    void LogWarning(const std::string& msg) { spdlog::warn(msg); }
    void LogError(const std::string& msg) { spdlog::error(msg); }
    void LogCritical(const std::string& msg) { spdlog::critical(msg); }

    void inline SetLogging(bool setlog) {m_setlog = setlog;}

    static Logger* GlobalLogger;
};

static void InitLogger() { Logger::GlobalLogger = new Logger; }
static void DeinitLogger() noexcept { delete Logger::GlobalLogger; }

#define ME_LOGINIT() InitLogger()
#define ME_LOGDEINIT() DeinitLogger()

#define ME_CORE_MSG(X) Logger::GlobalLogger->LogNormal(X)
#define ME_CORE_INFO(X) Logger::GlobalLogger->LogInfo(X)
#define ME_CORE_WARNING(X) Logger::GlobalLogger->LogWarning(X)
#define ME_CORE_ERROR(CND, X) if(CND) { Logger::GlobalLogger->LogError(X); }
#define ME_CORE_CRITICAL(CND, X) if(CND) { Logger::GlobalLogger->LogCritical(X); throw X; }

#endif