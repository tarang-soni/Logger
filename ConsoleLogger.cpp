#include "ConsoleLogger.h"
#include <iostream>
#include "LogUtil.h"
namespace Log
{
	ConsoleLogger::ConsoleLogger(std::ostream& oss, LogLevel supportedLevels) : m_supportedLevels(supportedLevels),m_oss(oss)
	{

	}
	inline const char* GetColor(LogLevel level)
	{
		switch (level)
		{
			case LogLevel::Info:return "36m";
			case LogLevel::Warning: return "33m";
			case LogLevel::Error:return "31m";
			default: return "0m";
		}
	}
	void ConsoleLogger::Log(LogLevel level, const std::string& message)
	{
		LogLevel currentLevel = (level & m_supportedLevels);
		if (currentLevel == LogLevel::None) return;

		std::string timeStamp = Util::CurrentTime();
		std::string color = GetColor(currentLevel);
	
		m_oss <<"\033["<<color << "[" << timeStamp << "]" << message << "\033[0m" << std::endl;
	}

}