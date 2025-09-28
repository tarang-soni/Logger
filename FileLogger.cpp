#include "FileLogger.h"
#include <iostream>
#include "LogUtil.h"
namespace Log 
{
	FileLogger::FileLogger(const std::string& filename, LogLevel supportedLevels) :m_filename(filename), m_supportedLevels(supportedLevels)
	{
		m_logFile.open(filename, std::ios::app);
		if (!m_logFile)
		{
			std::cerr << "Failed to open log file." << std::endl;
			return;
		}

	}
	void FileLogger::Log(LogLevel level, const std::string& message)
	{
		if ((level & m_supportedLevels) == LogLevel::None) return;
		std::string timeStamp = Util::CurrentTime();
		if (m_logFile.is_open())
		{
			m_logFile << timeStamp << std::endl << message << std::endl;
		}
	}

	FileLogger::~FileLogger()
	{
		if (m_logFile.is_open())
		{
			m_logFile << "-----------------------------------------------------" << std::endl;
			m_logFile.close();
		}
	}
}