#include "Logger.h"

std::shared_ptr<Logger> logger = std::make_shared<Logger>();
std::shared_ptr<Logger> GetLogger()
{
    return logger;
}

void Logger::LogError(const std::string& err)
{
    if(m_setlog)
    {
        now = std::time(0);
        m_Time = localtime(&now);
        std::cout << "[" << 5+m_Time->tm_hour << ":" << 30+m_Time->tm_min << ":" << m_Time->tm_sec << "] MARS ENGINE    ERROR: " << err << std::endl;
    }
}

void Logger::LogInfo(const std::string& info)
{
    if(m_setlog)
    {
        now = std::time(0);
        m_Time = localtime(&now);
        std::cout << "[" << 5+m_Time->tm_hour << ":" << 30+m_Time->tm_min << ":" << m_Time->tm_sec << "] MARS ENGINE     INFO: "  << info << std::endl;
    }
}

void Logger::LogWarning(const std::string& war)
{
    if(m_setlog)
    {
        now = std::time(0);
        m_Time = localtime(&now);
        std::cout << "[" << 5+m_Time->tm_hour << ":" << 30+m_Time->tm_min << ":" << m_Time->tm_sec << "] MARS ENGINE  WARNING: "  << war << std::endl;
    }
}

void Logger::LogCritical(const std::string& crt)
{
    if(m_setlog)
    {
        now = std::time(0);
        m_Time = localtime(&now);
        std::cout << "[" << 5+m_Time->tm_hour << ":" << 30+m_Time->tm_min << ":" << m_Time->tm_sec << "] MARS ENGINE CRIRICAL: "  << crt << std::endl;
    }
}

void Logger::LogNormal(const std::string& msg)
{
    if(m_setlog)
    {
        now = std::time(0);
        m_Time = localtime(&now);
        std::cout <<"[" << 5+m_Time->tm_hour << ":" << 30+m_Time->tm_min << ":" << m_Time->tm_sec << "] MARS ENGINE   NORMAL: "  << msg << std::endl;
    }
}
