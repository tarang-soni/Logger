#pragma once
#include <cstdint>
namespace Log
{
	enum class LogLevel : uint32_t
	{
		None = 0,
		Info = 1 << 0,
		Warning = 1 << 1,
		Error = 1 << 2
	};
	inline LogLevel operator|(LogLevel a, LogLevel b)
	{
		return static_cast<LogLevel>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}
	inline LogLevel operator&(LogLevel a, LogLevel b)
	{
		return static_cast<LogLevel>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
	}
}