#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <memory>
#include <vector>

class Logger
{
private:
    time_t now = std::time(0);
    tm* m_Time = localtime(&now);

    bool m_setlog = true;
public:

    void LogError(const std::string& err);
    void LogInfo(const std::string& info);
    void LogWarning(const std::string& war);
    void LogCritical(const std::string& crt);
    void LogNormal(const std::string& msg);

    void SetLogging(bool setlog) {m_setlog = setlog;}
};

static std::unique_ptr<Logger> logger = std::make_unique<Logger>();
#define ME_CORE_MSG(X)  logger->LogNormal(X)
#define ME_CORE_INFO(X) logger->LogInfo(X)
#define ME_CORE_WARNING(X) logger->LogWarning(X)
#define ME_CORE_ERROR(X) logger->LogError(X)
#define ME_CORE_CRITICAL(X) logger->LogCritical(X)