
#include <iostream>
#include "Logger.h"
#include "LogManager.h"
#include "FileLogger.h"
#include "ConsoleLogger.h"
#include <memory>

int main()
{
	Log::LogManager::Get().AddSink(std::make_unique<Log::FileLogger>("log.txt", Log::LogLevel::Info | Log::LogLevel::Warning | Log::LogLevel::Error));
	Log::LogManager::Get().AddSink(std::make_unique<Log::ConsoleLogger>(std::cout, Log::LogLevel::Info | Log::LogLevel::Warning | Log::LogLevel::Error));


	LOG_INFO("This is an info message. ", 12, " LOLOOLOL");
	LOG_WARN("This is an warning message.");
	LOG_ERROR("This is an error message.");

	return 0;
}
