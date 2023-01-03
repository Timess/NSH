#pragma once
#include <d3d11.h>
#include <optional>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "KeyBind.h"
#include "Config.h"

class Gui
{
public:
	void new_console();
	void close_console();
	Gui();
	void render();
	bool isOpen() { return open; }
	void handleToggle();


private:
	bool open = true;
	KeyBind openKey{ KeyBind::INSERT,KeyMode::Toggle,open };


};



extern std::optional<Gui> gui;
