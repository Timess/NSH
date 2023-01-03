#pragma once
#include <queue>
#include <mutex>
#include <atomic>
#include <chrono>
#include <optional>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"

namespace NSHLogger {
	enum Priority {
		LOG_DEBUG, LOG_STATE, LOG_INFO, LOG_WARNING, LOG_ERROR,
	};

	class Logger
	{
	public:
		Logger& operator=(const Logger&) = delete;
		Logger(const Logger&) = delete;
		Logger();
		~Logger() = default;

		void Clear();

		void render();

		template<Priority priority = LOG_INFO, typename... Args>
		void log(const char* fmt, Args... args) {
			char buf[4096] = { 0 };

			sprintf(buf, "[%s] [%d] ", Priority_To_String[priority], GetCurrentThreadId());

			sprintf(buf + strlen(buf), fmt, args...);

			std::string entry(buf);
			std::unique_lock lock(_mutex); //lock
			//while (_queue.size() >= 1000) _cond.wait(lock); //×Ô¶¯unlock
			_queue.push(std::move(entry));
		}

		bool open = false;
	private:
		std::queue<std::string> _queue;
		std::mutex _mutex;

		ImGuiTextBuffer     Buf;
		ImVector<int>       LineOffsets;

		const char* Priority_To_String[5]
		{
			"DEBUG",
			"CONFIG",
			"INFO",
			"WARNING",
			"ERROR"
		};
	};
}

extern std::optional<NSHLogger::Logger> logger;

class Timestamp
{
public:
	Timestamp()
		: _beginTimePoint(std::chrono::high_resolution_clock::now())
	{ }

	void reset()
	{
		_beginTimePoint = std::chrono::high_resolution_clock::now();
	}

	int64_t elapsed()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - _beginTimePoint).count();
	}

	static std::string localtime();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> _beginTimePoint;
};