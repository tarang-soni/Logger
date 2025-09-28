#pragma once
#include <string>
#include <chrono>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <fstream>


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
		std::ostringstream log_stream;
		std::ostringstream main_message_stream;

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
		main_message_stream<< fileName 
			<< " => "
			<< funcname << "()"
			<< ":" << line << " => ";

		log_stream << "\033[" << color
			<< "[" << timestamp << "]" 
			<< "[" << type << "] " 
			<< main_message_stream.str();
		log_stream << "\033[0m";


		LogArgs(main_message_stream, args...);
		LogArgs(log_stream, args...);
		std::cout << log_stream.str() << std::endl;
		std::ofstream logFile("log.txt", std::ios::app);
		if(!logFile)
		{
			std::cerr << "Failed to open log file." << std::endl;
			return;
		}
		logFile << timestamp<<"\n" << main_message_stream.str() << std::endl;
		logFile.close(); 
		log_stream.clear();
		main_message_stream.clear();
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

/*
TODO: Logger Library Phases

Phase 1: Core Logging
- [x] Enum for log levels (Info, Warning, Error)
- [x] Basic Log function for console output
- [x] Macros: LOG_INFO, LOG_WARN, LOG_ERROR

Phase 2: File/Line/Function Metadata
- [x] Use __FILE__, __LINE__, __func__ in macros
- [x] Format log output: [timestamp][LEVEL] file => func():line => message

Phase 3: Timestamp Support
- [x] Add current timestamp with milliseconds
- [x] Use std::chrono and std::put_time

Phase 4: Variadic Templates
- [x] Support multi-argument logging
- [x] Recursive LogArgs function with base case

Phase 5: Colored Console Output
- [x] Different colors for Info/Warning/Error
- [x] Reset color after printing

Phase 6: Optional Enhancements
- [x] Add file logging (write logs to file)
- [ ] Thread safety with std::mutex
- [ ] Log level filtering (skip Info in production)

Phase 7: Publishing / Packaging
- [x] GitHub repo setup
- [ ] Build instructions / CMake integration
- [ ] README with usage examples

Phase 8: Advanced Features
- [ ] Multiple logger instances (categories)
- [ ] Custom log formats (JSON, CSV, etc.)
- [ ] Rotating log files (size-based or daily)
- [ ] Callbacks / Observer support for log events
- [ ] Performance optimization (minimal overhead in high-frequency logging)

Phase 9: Testing & Documentation
- [ ] Unit tests for all log levels and multi-argument logs
- [ ] Multi-threaded logging tests
- [ ] Document macros, usage, and example code

*/
