#include "Gui.h"
#include "Helper.h"

#ifndef GUI_CPP
#define GUI_CPP
#endif

#include "Hooks.h"


void Gui::new_console() {
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
}

void Gui::close_console() {
	fclose(stdout);
	fclose(stderr);
	FreeConsole();
}


Gui::Gui() {

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	ImFontConfig cfg;
	cfg.SizePixels = 15.0f;

	Helper::addFontFromVFONT("D:\\1\\notosanssc-regular.vfont", 17.0f, io.Fonts->GetGlyphRangesChineseFull(), false);

	ImGui::StyleColorsSpectrum();
	ImGuiStyle& style = ImGui::GetStyle();

	style.ScrollbarSize = 9.0f;

	io.SetPlatformImeDataFn = nullptr; // F**king bug take 4 hours of my life

}

ImVec2 operator/(ImVec2& a, float b) {
	return{ a.x / b, a.y / b };
}

void Gui::render() {


	ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize / 2.f, ImGuiCond_FirstUseEver);
	ImGui::Begin("persuader NSH " __DATE__ " " __TIME__, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::Checkbox((char*)u8"日志", &config->logger.enable);
	if (hooks->eventLogger) {
		ImGui::SameLine();
		ImGui::Checkbox((char*)u8"事件日志", &config->logger.enableLogEvent);
	}
	if (hooks->doubleLogger) {
		ImGui::SameLine();
		ImGui::Checkbox((char*)u8"数据日志", &config->logger.enableDoubleEvent);
	}
	ImGui::Checkbox((char*)u8"时装称号修改", &config->fashion.enable);
	ImGui::SameLine();
	ImGui::Checkbox((char*)u8"强力屏蔽其他人称号", &config->enableSuperBlockTitle);

	ImGui::Checkbox((char*)u8"坐骑修改", &config->vehicle.enable);

	ImVec4 color2(255, 0, 0, 255);
	ImGui::TextColored(color2, (char*)u8"惜时");

	if (ImGui::Button((char*)u8"安全退出")) {
		config->shutDown = true;
	}

	ImGui::End();

}

void Gui::handleToggle()
{
	if (this->openKey.isPressed()) {
		open = !open;
	}
}