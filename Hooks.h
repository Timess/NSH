#pragma once

#define DEFINE_DXGI
#ifndef GUI_CPP
#include "ntdll.h"
#endif
#include <iostream>
#include <string>
#include <memory>
#include <optional>


#include "Fashion.h"
#include "SuperBlockTitle.h"
#include "EventLogger.h"
#include "DoubleLogger.h"
#include "Vehicle.h"

class Hooks
{
public:
	Hooks(HMODULE hModule);
	void install();
	void uninstall();
	static DWORD WINAPI unload(LPVOID);
	void restoreInjectEntryPoint();

	WNDPROC originalWndProc;
	UINT64 originalPresent;
	UINT64 originalReset;
public:
	UINT64 hModule;
	HANDLE injectThread;


	UINT64 GacRunnerNG;

	HWND window;

	UINT64 table;
	UINT64 ngtable;
	UINT64 origngtable;
	
	UINT64 present;
	UINT64 reset;
	UINT64 dxgi;

	std::unique_ptr<UINT64[]> newTable;

	typedef struct _request_data
	{
		uint32_t unique;
		ULONG command;
		ULONG self;
		void* buffer;
		void* rw;
	}request_data, * prequest_data;

	UINT32 currentProcessId;
	std::add_pointer_t<UINT64 __fastcall(UINT32, void*)> callDriver;


	std::add_pointer_t<double __fastcall(UINT64, UINT64)> sub1;
	std::add_pointer_t<UINT64 __fastcall(UINT64, UINT32)> sub2;
	std::add_pointer_t<UINT64 __fastcall(UINT64, UINT64, UINT64)> sub3;


	std::optional<Fashion> fashion;
	std::optional<SuperBlockTitle> superBlockTitle;
	std::optional<EventLogger> eventLogger;
	std::optional<DoubleLogger> doubleLogger;
	std::optional<Vehicle> vehicle;

	std::vector<std::optional<NSHFeatures::Features>*> features;

	static bool _virtualprot(void* address, ULONGLONG size, ULONG protect, PULONG oldprotect);

	static bool _protmem(void* address);

	static void WPM(ULONGLONG address, void* buffer, size_t size);

	static bool RPM(ULONGLONG address, void* buffer, size_t size);

	static void _memwrite(void* dst, void* src, size_t size);

	static bool _memread(void* src, void* dst, size_t size);

	static UINT64 _alloc(size_t size, DWORD protect);

};

extern std::optional<Hooks> hooks;
extern UINT64 g_trampolineAddress;