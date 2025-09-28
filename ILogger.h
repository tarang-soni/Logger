#pragma once
#include <string>
#include "LogLevel.h"
namespace Log 
{
	class ILogger
	{
	public:
		virtual ~ILogger() = default;
		virtual void Log(LogLevel level, const std::string& message) = 0;
	};
}