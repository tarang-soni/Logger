//#pragma once
//#include <string>
//#include <chrono>
//#include <iostream>
//#include <sstream>
//#include <ctime>
//#include <iomanip>
//#include <fstream>
//
//#include "LogLevel.h"
//
//#define LOG_INFO(...) Logger::Log(LogLevel::Info,__FILE__,__func__,__LINE__,__VA_ARGS__)
//#define LOG_WARN(...) Logger::Log(LogLevel::Warning,__FILE__,__func__,__LINE__,__VA_ARGS__)
//#define LOG_ERROR(...) Logger::Log(LogLevel::Error,__FILE__,__func__,__LINE__,__VA_ARGS__)
//
//class Logger
//{
//private:
//	LogLevel m_supported_levels;
//	std::string m_filename;
//	std::ofstream m_logFile;
//public:
//	Logger(const std::string& filename, LogLevel supportedLevels) :
//		m_supported_levels(supportedLevels),
//		m_filename(filename)
//	{
//		m_logFile.open(this->m_filename, std::ios::app);
//		if (!m_logFile)
//		{
//			std::cerr << "Failed to open log file." << std::endl;
//			return;
//		}
//		//TODO: Add separator in the log file
//	}
//	virtual ~Logger()
//	{
//		//TODO: Add separator in the log file
//		if (m_logFile.is_open()) {
//			m_logFile.close();
//		}
//	}
//	template<typename... Args>
//	void Log(LogLevel level, const char* filename, const char* funcname, int line, Args... args)
//	{
//		if (!(level & m_supported_levels) == LogLevel::None) return;
//		std::ostringstream log_stream;
//		std::ostringstream main_message_stream;
//
//		std::string fileName = filename;
//		size_t pos = fileName.find_last_of("\\/");
//		if (pos != std::string::npos) fileName = fileName.substr(pos + 1);
//		std::string color = "0m";
//		std::string type = "";
//		std::string timestamp = CurrentTime();
//		switch (level)
//		{
//		case LogLevel::Info:
//			color = "36m";
//			type = "INFO";
//			break;
//		case LogLevel::Warning:
//			color = "33m";
//			type = "WARN";
//
//			break;
//		case LogLevel::Error:
//			color = "31m";
//			type = "ERROR";
//			break;
//		default:
//			break;
//		}
//		main_message_stream<< fileName 
//			<< " => "
//			<< funcname << "()"
//			<< ":" << line << " => ";
//
//		log_stream << "\033[" << color
//			<< "[" << timestamp << "]" 
//			<< "[" << type << "] " 
//			<< main_message_stream.str();
//		log_stream << "\033[0m";
//
//		std::stringstream message;
//		LogArgs(message, args...);
//		log_stream << message.str();
//		main_message_stream << message.str();
//		std::cout << log_stream.str() << std::endl;
//		if (m_logFile.is_open())
//		{
//			logFile << timestamp << "\n" << main_message_stream.str() << std::endl;
//		}
//		log_stream.clear();
//		main_message_stream.clear();
//	}
//private:
//	
//	template<typename... Args>
//	static void LogArgs(std::ostringstream& oss, Args... args) {
//		(oss << ... << args);
//	}
//	static std::string CurrentTime()
//	{
//		auto now = std::chrono::system_clock::now();
//		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
//			now.time_since_epoch()) % 1000;
//		std::time_t now_time = std::chrono::system_clock::to_time_t(now);
//
//		std::tm localTime;
//#ifdef _WIN32
//		localtime_s(&localTime, &now_time);
//#else
//		localtime_r(&now_time, &localTime);
//#endif
//		std::ostringstream oss;
//		oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S")
//			<< '.' << std::setfill('0') << std::setw(3) << ms.count();
//		return oss.str();
//	}
//
//};
//
///*
