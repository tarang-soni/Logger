#pragma once
#include "ILogger.h"
#include <fstream>
namespace Log 
{
	class FileLogger :
		public ILogger
	{
	private:
		// Inherited via ILogger
		std::string m_filename;
		LogLevel m_supportedLevels;
		std::ofstream m_logFile;
	public:
		FileLogger(const std::string& filename, LogLevel supportedLevels);
		void Log(LogLevel level, const std::string& message) override;
		virtual ~FileLogger();
	};

}