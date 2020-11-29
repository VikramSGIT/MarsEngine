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

std::shared_ptr<Logger> GetLogger();
#define ME_CORE_MSG(X) GetLogger()->LogNormal(X);
#define ME_CORE_INFO(X) GetLogger()->LogInfo(X)
#define ME_CORE_WARNING(X) GetLogger()->LogWarning(X)
#define ME_CORE_ERROR(X) GetLogger()->LogError(X)
#define ME_CORE_CRITICAL(X) GetLogger()->LogCritical(X)