#include "Logger.h"

Logger* Logger::GlobalLogger = nullptr;

void InitLogger()
{
	Logger::GlobalLogger = new Logger;
}

void DeinitLogger() noexcept
{
	delete Logger::GlobalLogger;
}