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
#include <mutex>
#include <thread>
#include <queue>
#include <utility>

namespace Log 
{
	class LogManager
	{
		struct LogData
		{
			LogLevel level;
			const char* filename;
			const char* funcname;
			int line;
			std::string message;

		};
	private:
		std::vector<std::unique_ptr<ILogger>> m_sinks;
		std::mutex m_mutex;
		std::thread m_logThread;
		std::condition_variable cv;
		bool m_exit;
		std::queue<std::unique_ptr<LogData>> m_logQueue;
		LogManager():m_exit(false) 
		{
			m_logThread = std::thread(&LogManager::LogTask, this);
		}
	public:
		LogManager(const LogManager&) = delete;
		LogManager& operator= (const LogManager&) = delete;

		virtual ~LogManager()
		{
			//std::lock_guard<std::mutex> lock(m_mu
			{
				std::lock_guard<std::mutex> lock(m_mutex);	//thread safety
				m_exit = true;
			}
			cv.notify_one();
			m_logThread.join();
		}
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
			(oss << ... << args);
			auto logData = std::make_unique<LogData>(level, filename, funcname, line, oss.str());
			{
				std::lock_guard<std::mutex> lock(m_mutex);	//thread safety
				m_logQueue.push(std::move(logData));
			}
			cv.notify_one();
		}

	private:


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
		void LogTask()
		{
			while(true)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				cv.wait(lock, [this] {
					return !m_logQueue.empty() || m_exit;
					});
				if (m_exit && m_logQueue.empty())
					break;

				std::unique_ptr<LogData> data = std::move(m_logQueue.front());
				m_logQueue.pop();
				std::ostringstream oss;

				SetLogData(oss, data->level, data->filename, data->funcname, data->line);
				for (const auto& sink : m_sinks)
				{
					sink->Log(data->level, data->message);
				}
			}
		}
	};

}
#define LOG_INFO(...) Log::LogManager::Get().Log(Log::LogLevel::Info,__FILE__,__func__,__LINE__,__VA_ARGS__)
#define LOG_WARN(...) Log::LogManager::Get().Log(Log::LogLevel::Warning,__FILE__,__func__,__LINE__,__VA_ARGS__)
#define LOG_ERROR(...) Log::LogManager::Get().Log(Log::LogLevel::Error,__FILE__,__func__,__LINE__,__VA_ARGS__)


