#pragma once

#include <iostream>
#include <ctime>
#include <string>

class Logger
{
private:
    time_t now = std::time(0);
    tm* m_Time = localtime(&now);

    bool m_setlog = true;
public:

    void LogError(const std::string& err)
    {
        if (m_setlog)
        {
            now = std::time(0);
            m_Time = localtime(&now);
            std::cout << "[" << 5 + m_Time->tm_hour << ":" << 30 + m_Time->tm_min << ":" << m_Time->tm_sec << "] MARS ENGINE    ERROR: " << err << std::endl;
        }
    }
    void LogInfo(const std::string& info)
    {
        if (m_setlog)
        {
            now = std::time(0);
            m_Time = localtime(&now);
            std::cout << "[" << 5 + m_Time->tm_hour << ":" << 30 + m_Time->tm_min << ":" << m_Time->tm_sec << "] MARS ENGINE     INFO: " << info << std::endl;
        }
    }
    void LogWarning(const std::string& war)
    {
        if (m_setlog)
        {
            now = std::time(0);
            m_Time = localtime(&now);
            std::cout << "[" << 5 + m_Time->tm_hour << ":" << 30 + m_Time->tm_min << ":" << m_Time->tm_sec << "] MARS ENGINE  WARNING: " << war << std::endl;
        }
    }
    void LogCritical(const std::string& crt)
    {
        if (m_setlog)
        {
            now = std::time(0);
            m_Time = localtime(&now);
            std::cout << "[" << 5 + m_Time->tm_hour << ":" << 30 + m_Time->tm_min << ":" << m_Time->tm_sec << "] MARS ENGINE CRIRICAL: " << crt << std::endl;
        }
    }
    void LogNormal(const std::string& msg)
    {
        if (m_setlog)
        {
            now = std::time(0);
            m_Time = localtime(&now);
            std::cout << "[" << 5 + m_Time->tm_hour << ":" << 30 + m_Time->tm_min << ":" << m_Time->tm_sec << "] MARS ENGINE   NORMAL: " << msg << std::endl;
        }
    }

    void SetLogging(bool setlog) {m_setlog = setlog;}

    static Logger* GlobalLogger;
};

void InitLogger();
void DeinitLogger() noexcept;

#define ME_LOGINIT() InitLogger()
#define ME_LOGDEINIT() DeinitLogger()

#define ME_CORE_MSG(X) Logger::GlobalLogger->LogNormal(X)
#define ME_CORE_INFO(X) Logger::GlobalLogger->LogInfo(X)
#define ME_CORE_WARNING(X) Logger::GlobalLogger->LogWarning(X)
#define ME_CORE_ERROR(X) Logger::GlobalLogger->LogError(X)
#define ME_CORE_CRITICAL(X) Logger::GlobalLogger->LogCritical(X)