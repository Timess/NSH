#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <tuple>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

namespace Helper
{
	std::tuple<UINT64, UINT64> GetModuleByName(const wchar_t* name);
	ImFont* addFontFromVFONT(const std::string& path, float size, const ImWchar* glyphRanges, bool merge);
	ImWchar* getFontGlyphRanges();
};

