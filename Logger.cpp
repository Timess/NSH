#include "ntdll.h"
#include "Logger.h"
#include <iostream>
#include <string>

#include "Gui.h"
#include "Config.h"

std::optional<NSHLogger::Logger> logger;
namespace NSHLogger {
	void Logger::Clear() {
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}

	Logger::Logger() {
		Clear();
	}

	void Logger::render() {

		if (config->logger.enable) {

			ImGui::SetNextWindowSize(ImVec2(1024, 768), ImGuiCond_FirstUseEver);
			ImGui::Begin((char*)u8"日志窗口", nullptr);

			if (!config->logger.enableConsole) {
				if (ImGui::Button((char*)u8"开启Console")) {
					config->logger.enableConsole = true;
					gui->new_console();
				}
			}
			else {
				if (ImGui::Button((char*)u8"关闭Console")) {
					config->logger.enableConsole = false;
					gui->close_console();
				}
			}


			ImGui::Text((char*)u8"日志");
			ImGui::SameLine();
			ImGui::Checkbox((char*)u8"自动滚动", &config->logger.enableAutoScroll);

			if (ImGui::Button((char*)u8"清空")) {
				Clear();
			}

			int maxPopLine = 100;
			int popCount = 0;
			std::unique_lock lock(_mutex);

			while (!_queue.empty())
			{
				if (popCount >= maxPopLine) break;

				std::string s = "[" + Timestamp::localtime() + "]" + _queue.front();// << std::endl;
				_queue.pop();

				if (config->logger.enableConsole) {
					std::cout << s.c_str();
				}

				int old_size = Buf.size();
				Buf.append(s.c_str());
				for (int new_size = Buf.size(); old_size < new_size; old_size++)
					if (Buf[old_size] == '\n')
						LineOffsets.push_back(old_size + 1);

				popCount++;
			}

			lock.unlock();



			ImGui::Separator();
			ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

			const char* buf = Buf.begin();
			const char* buf_end = Buf.end();

			ImGuiListClipper clipper;
			clipper.Begin(LineOffsets.Size);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
			clipper.End();

			if (config->logger.enableAutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::EndChild();

			ImGui::End();
		}
		else {



		}

	}

}

std::string Timestamp::localtime()
{
	std::ostringstream stream;
	auto now = std::chrono::system_clock::now();
	time_t tt = std::chrono::system_clock::to_time_t(now);
	struct tm tm;
	localtime_s(&tm, &tt);
	stream << std::put_time(&tm, "%F %T");
	return stream.str();
}
