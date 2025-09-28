#pragma once
#include <memory>
#include <vector>
#include <sstream>
#include "ILogger.h"
#include "LogLevel.h"
#include "LogUtil.h"
#include<chrono>
#include <ctime>
#include <iomanip>


namespace Log 
{
	class LogManager
	{
	private:
		std::vector<std::unique_ptr<ILogger>> m_sinks;
		LogManager() {}
	public:
		LogManager(const LogManager&) = delete;
		LogManager& operator= (const LogManager&) = delete;
		inline static LogManager& Get()
		{
			static LogManager instance;
			return instance;
		}
		void AddSink(std::unique_ptr<ILogger> sink)
		{
			m_sinks.push_back(std::move(sink));
		}
		template<typename ...Args>
		void Log(LogLevel level, const char* filename, const char* funcname, int line, Args... args)
		{
			std::ostringstream oss;
			SetLogData(oss, level, filename, funcname, line);
			(oss << ... << args);
			std::string message = oss.str();
			for (const auto& sink : m_sinks)
			{
				sink->Log(level, message);
			}
		}



		void SetLogData(std::ostringstream& oss, LogLevel level, const char* filename, const char* funcname, int line)
		{
			std::string fileName = filename;
			size_t pos = fileName.find_last_of("\\/");
			if (pos != std::string::npos) fileName = fileName.substr(pos + 1);

			std::string type = GetLevelInString(level);
			std::string timestamp = Util::CurrentTime();

			oss << fileName
				<< " => "
				<< funcname << "()"
				<< ":" << line << " => "
				<< type<<" ";

		}
		inline const char* GetLevelInString(LogLevel level)
		{
			switch (level)
			{
			case LogLevel::Info:return "[INFO]";
			case LogLevel::Warning: return "[WARN]";
			case LogLevel::Error:return "[ERROR]";
			default: return "[NONE]";
			}
		}
	};

}
#define LOG_INFO(...) Log::LogManager::Get().Log(Log::LogLevel::Info,__FILE__,__func__,__LINE__,__VA_ARGS__)
#define LOG_WARN(...) Log::LogManager::Get().Log(Log::LogLevel::Warning,__FILE__,__func__,__LINE__,__VA_ARGS__)
#define LOG_ERROR(...) Log::LogManager::Get().Log(Log::LogLevel::Error,__FILE__,__func__,__LINE__,__VA_ARGS__)


//TODO: Logger Library Phases
//
//Phase 1: Core Logging
//- [x] Enum for log levels (Info, Warning, Error)
//- [x] Basic Log function for console output
//- [x] Macros: LOG_INFO, LOG_WARN, LOG_ERROR
//
//Phase 2: File/Line/Function Metadata
//- [x] Use __FILE__, __LINE__, __func__ in macros
//- [x] Format log output: [timestamp][LEVEL] file => func():line => message
//
//Phase 3: Timestamp Support
//- [x] Add current timestamp with milliseconds
//- [x] Use std::chrono and std::put_time
//
//Phase 4: Variadic Templates
//- [x] Support multi-argument logging
//- [x] Recursive LogArgs function with base case
//
//Phase 5: Colored Console Output
//- [x] Different colors for Info/Warning/Error
//- [x] Reset color after printing
//
//Phase 6: Optional Enhancements
//- [x] Add file logging (write logs to file) 
//- [ ] Thread safety with std::mutex
//- [ ] Log level filtering (skip Info in production)
//
//Phase 7: Publishing / Packaging
//- [x] GitHub repo setup
//- [ ] Build instructions / CMake integration
//- [ ] README with usage examples
//
//Phase 8: Advanced Features
//- [x] Multiple logger instances (categories)
//- [ ] Custom log formats (JSON, CSV, etc.)
//- [ ] Rotating log files (size-based or daily)
//- [ ] Callbacks / Observer support for log events
//- [ ] Performance optimization (minimal overhead in high-frequency logging)
//
//Phase 9: Testing & Documentation
//- [ ] Unit tests for all log levels and multi-argument logs
//- [ ] Multi-threaded logging tests
//- [ ] Document macros, usage, and example code
//
//*/
