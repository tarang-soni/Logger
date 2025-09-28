#pragma once
#include "ILogger.h"
#include <ostream>
namespace Log
{
	class ConsoleLogger :
		public ILogger
	{
	private:

		LogLevel m_supportedLevels;
		std::ostream& m_oss;

	public:
		ConsoleLogger(std::ostream& oss, LogLevel supportedLevels);
		void Log(LogLevel level, const std::string& message) override;

	};

}