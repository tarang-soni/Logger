#pragma once
#include<chrono>
#include <ctime>
#include <string>
namespace Log::Util
{
	inline static std::string CurrentTime()
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
}