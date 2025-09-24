#pragma once
#include <string>
#include <chrono>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>


#define LOG_INFO(...) Logger::Log(Logger::LogLevel::Info,__FILE__,__func__,__LINE__,__VA_ARGS__)
#define LOG_WARN(...) Logger::Log(Logger::LogLevel::Warning,__FILE__,__func__,__LINE__,__VA_ARGS__)
#define LOG_ERROR(...) Logger::Log(Logger::LogLevel::Error,__FILE__,__func__,__LINE__,__VA_ARGS__)
class Logger
{
public:
	enum class LogLevel
	{
		Info,
		Warning,
		Error
	};
public:
	template<typename... Args>
	static void Log(LogLevel level, const char* filename, const char* funcname, int line, Args... args)
	{
		std::ostringstream oss;


		std::string fileName = filename;
		size_t pos = fileName.find_last_of("\\/");
		if (pos != std::string::npos) fileName = fileName.substr(pos + 1);
		std::string color = "0m";
		std::string type = "";
		std::string timestamp = CurrentTime();
		switch (level)
		{
		case LogLevel::Info:
			color = "36m";
			type = "INFO";
			break;
		case LogLevel::Warning:
			color = "33m";
			type = "WARN";

			break;
		case LogLevel::Error:
			color = "31m";
			type = "ERROR";
			break;
		default:
			break;
		}
		oss << "\033[" << color
			<< "[" << timestamp << "]" 
			<< "[" << type << "] " 
			<< fileName << " => " 
			<< funcname << "()" 
			<< ":" << line << " => ";
		LogArgs(oss, args...);
		oss << "\033[0m";
		std::cout << oss.str() << std::endl;
	}
private:
	
	template<typename T,typename... Args>
	static void LogArgs(std::ostringstream& oss,T first, Args... args)
	{
		oss << first;
		LogArgs(oss, args...);
	}
	static void LogArgs(std::ostringstream& oss) { /* base case, do nothing */ }

	static std::string CurrentTime()
	{
		auto now = std::chrono::system_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			now.time_since_epoch()) % 1000;
		std::time_t now_time = std::chrono::system_clock::to_time_t(now);

		std::tm localTime;
#ifdef _WIN32
		localtime_s(&localTime, &now_time);
#else
		localtime_r(&now_time, &localTime);
#endif
		std::ostringstream oss;
		oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S")
			<< '.' << std::setfill('0') << std::setw(3) << ms.count();
		return oss.str();
	}

};

