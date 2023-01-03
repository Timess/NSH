
#include "Hooks.h"
#include "Helper.h"
#include "Logger.h"

#include "Config.h"
#include "Gui.h"

std::optional<Config> config;
std::optional<Gui> gui;

UINT64 g_trampolineAddress;


bool Hooks::_virtualprot(void* address, ULONGLONG size, ULONG protect, PULONG oldprotect) {
	typedef struct _memprot {
		ULONG pid;
		ULONGLONG address;
		ULONGLONG size;
		ULONG protect;
	}memprot;

	sizeof(memprot);

	memprot m = { 0 };
	m.pid = hooks->currentProcessId;
	m.address = (ULONGLONG)address;
	m.size = size;
	m.protect = protect;

	request_data r = { 0 };
	r.self = hooks->currentProcessId;
	r.command = 27;
	r.buffer = &m;
	r.unique = 0x114514;
	r.rw = oldprotect;

	return hooks->callDriver(0x114514, &r) == 1;
}

bool Hooks::_protmem(void* address) {
	typedef struct _memprot {
		ULONG pid;
		ULONGLONG address;
	}memprot;

	memprot m = { 0 };
	m.pid = hooks->currentProcessId;
	m.address = (ULONGLONG)address;

	request_data r = { 0 };
	r.self = hooks->currentProcessId;
	r.command = 28;
	r.buffer = &m;
	r.unique = 0x114514;
	r.rw = 0;

	return hooks->callDriver(0x114514, &r) == 1;
}

void Hooks::WPM(ULONGLONG address, void* buffer, size_t size) {
start:
	typedef struct _memWrite {
		ULONG pid;
		ULONGLONG address;
		ULONGLONG size;
	}memWrite;

	request_data r = { 0 };
	memWrite m = { 0 };

	m.pid = hooks->currentProcessId;
	m.address = address;
	m.size = size;

	r.self = hooks->currentProcessId;
	r.command = 2;
	r.buffer = &m;
	r.unique = 0x114514;
	r.rw = buffer;
	const auto result = hooks->callDriver(0x114514, &r);
	if (result == -1) {
		logger->log("WPM: %llX, Write failed, Retry\n", address);
		goto start;
	}
}

bool Hooks::RPM(ULONGLONG address, void* buffer, size_t size) {

	memset(buffer, 0, size);
	typedef struct _memRead {
		ULONG pid;
		ULONGLONG address;
		ULONGLONG size;
	}memRead;

	request_data r = { 0 };
	memRead m = { 0 };


	m.address = address;
	m.pid = hooks->currentProcessId;
	m.size = size;

	r.self = hooks->currentProcessId;
	r.command = 1;
	r.buffer = &m;
	r.unique = 0x114514;
	r.rw = buffer;
	const auto result = hooks->callDriver(0x114514, &r);
	if (result == -1) {
		logger->log("WPM: %llX, Read failed\n", address);
		memset(buffer, 0, size);
		return false;
	}

	return true;
}

void Hooks::_memwrite(void* dst, void* src, size_t size) {
	size_t totalsize = size;
	size_t offset = 0;
	while (1) {
		if (totalsize > 0x1000) {
			WPM((ULONGLONG)((uintptr_t)dst + offset), (void*)((uintptr_t)src + offset), 0x1000);
			totalsize -= 0x1000;
			offset += 0x1000;
		}
		else {
			WPM((ULONGLONG)((uintptr_t)dst + offset), (void*)((uintptr_t)src + offset), totalsize);
			break;
		}
	}
}

bool Hooks::_memread(void* src, void* dst, size_t size) {
	size_t totalsize = size;
	size_t offset = 0;
	while (1) {
		if (totalsize > 0x1000) {
			if (RPM((ULONGLONG)((uintptr_t)src + offset), (void*)((uintptr_t)dst + offset), 0x1000)) {
				totalsize -= 0x1000;
				offset += 0x1000;
			}
			else return false;
		}
		else {
			if (RPM((ULONGLONG)((uintptr_t)src + offset), (void*)((uintptr_t)dst + offset), totalsize)) {
				break;
			}
			else return false;
		}
	}

	return true;
}

UINT64 Hooks::_alloc(size_t size, DWORD protect) {
	typedef struct _memAlloc {
		ULONG pid;
		ULONGLONG size;
		ULONG protect;
	}memAlloc;

	request_data r = { 0 };
	memAlloc m = { 0 };

	m.size = size;
	m.protect = protect;
	m.pid = hooks->currentProcessId;

	r.self = hooks->currentProcessId;
	r.unique = 0x114514;
	r.command = 7;
	r.buffer = &m;
	r.rw = 0;

	return hooks->callDriver(0x114514, &r);

}

typedef HRESULT(__stdcall* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

static IDXGISwapChainPresent fnIDXGISwapChainPresent = nullptr;
static ID3D11Device* pDevice = nullptr;
static ID3D11DeviceContext* pContext = nullptr;
static ID3D11RenderTargetView* mainRenderTargetView = nullptr;

static LRESULT __stdcall wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static BOOL g_bInitialised = false;
	if (!g_bInitialised)
	{
		//HANDLE thisThread = GetCurrentThread();
		//if (hooks->injectThread != thisThread) {

			//SuspendThread(hooks->injectThread);
		hooks->restoreInjectEntryPoint();
		hooks->install();
		//ResumeThread(hooks->injectThread);
		g_bInitialised = true;

		logger->log("WndProc Initialized!\n");
		//}
	};


	ImGuiIO& io = ImGui::GetIO();
	POINT mPos;
	GetCursorPos(&mPos);
	ScreenToClient(window, &mPos);
	ImGui::GetIO().MousePos.x = static_cast<float>(mPos.x);
	ImGui::GetIO().MousePos.y = static_cast<float>(mPos.y);


	LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam);


	return CallWindowProcW(hooks->originalWndProc, window, msg, wParam, lParam);
	//return ReturnSpoofer::DoSpoofCall<LRESULT>(OrigwndProc, hooks->jmpRBX, window, msg, wParam, lParam);
}

static HRESULT __stdcall Present_Hook(IDXGISwapChain* pChain, const UINT SyncInterval, const UINT Flags)
{
	static BOOL g_bInitialised = false;


	if (!g_bInitialised)
	{
		auto result = (HRESULT)pChain->GetDevice(__uuidof(pDevice), reinterpret_cast<void**>(&pDevice));

		if (SUCCEEDED(result))
		{
			pDevice->GetImmediateContext(&pContext);

			DXGI_SWAP_CHAIN_DESC sd;
			pChain->GetDesc(&sd);

			ImGui::CreateContext();


			config.emplace();


			gui.emplace();

			hooks->window = sd.OutputWindow;
			hooks->originalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hooks->window, GWLP_WNDPROC,
				reinterpret_cast<LONG_PTR>(wndProc)));

			ImGui_ImplWin32_Init(hooks->window);
			ImGui_ImplDX11_Init(pDevice, pContext);

			ID3D11Texture2D* pBackBuffer;
			pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
			if (pBackBuffer) {
				pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mainRenderTargetView);
				pBackBuffer->Release();
			}

			g_bInitialised = true;

			logger->log("Loaded! GacRunnerNG: 0x%p, dxgi: 0x%p, ngtable: 0x%p, newTable: 0x%p, printf: 0x%p\n", hooks->GacRunnerNG, hooks->dxgi, hooks->ngtable, hooks->newTable.get(), printf);

			logger->log("D3D11 Initialized! window: 0x%p, hooks->originalWndProc: 0x%p, wndProc: 0x%p\n", hooks->window, hooks->originalWndProc, wndProc);
		}
	}

	if (g_bInitialised) {
		if (!config->shutDown) {
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();

			ImGuiIO& io = ImGui::GetIO(); (void)io;

			ImGui::NewFrame();

			gui->handleToggle();

			if (gui->isOpen()) {
				logger->render();

				for (auto& f : hooks->features) {
					(*f)->render();
				}

				gui->render();
			}

			ImGui::EndFrame();
			ImGui::Render();

			pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
		else {
			if (!config->shutDownBegin) {
				config->shutDownBegin = true;
				hooks->uninstall();
			}
		}
	}

	return ((decltype(Present_Hook)*)hooks->originalPresent)(pChain, SyncInterval, Flags);
}

static HRESULT __stdcall Reset_Hook(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {

	if (mainRenderTargetView) {
		pContext->OMSetRenderTargets(0, 0, 0);
		mainRenderTargetView->Release();
	}

	HRESULT hr = ((decltype(Reset_Hook)*)hooks->originalReset)(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	ID3D11Texture2D* pBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
	// Perform error handling here!

	pDevice->CreateRenderTargetView(pBuffer, NULL, &mainRenderTargetView);
	// Perform error handling here!
	pBuffer->Release();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	// Set up the viewport.
	D3D11_VIEWPORT vp;
	vp.Width = Width;
	vp.Height = Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);
	return hr;
}

Hooks::Hooks(HMODULE hModule) {


?/?????????????????????????????????????????????????????????

	logger->log("%s\n", __FUNCTION__);
}


void Hooks::install() {
#define FEAT_INST(x) \
x.emplace(); \
features.push_back(reinterpret_cast<std::optional<NSHFeatures::Features>*>(&x))

	FEAT_INST(fashion);
	FEAT_INST(superBlockTitle);
	FEAT_INST(eventLogger);
	//FEAT_INST(doubleLogger);
	FEAT_INST(vehicle);


	for (auto& f : features) {
		(*f)->hook();
	}

}

extern "C" BOOL WINAPI _CRT_INIT(HMODULE moduleHandle, DWORD reason, LPVOID reserved);

DWORD WINAPI Hooks::unload(LPVOID p) {

	Sleep(200);

	HMODULE moduleHandle = (HMODULE)hooks->hModule;

	gui.reset();
	logger.reset();
	config.reset();
	hooks.reset();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	_CRT_INIT(moduleHandle, 0, 0);

	return 0;
}

void Hooks::uninstall() {

	for (auto& f : hooks->features) {
		(*f).reset();
	}


	SetWindowLongPtrW(window, GWLP_WNDPROC, LONG_PTR(originalWndProc));
	*(UINT64*)this->ngtable = this->origngtable;

	this->newTable.reset();

	if (HANDLE thread = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(Hooks::unload), GetCurrentThread(), 0, nullptr))
		CloseHandle(thread);

}

extern "C" UINT64 origcall;

void Hooks::restoreInjectEntryPoint() {

	static unsigned char restoreData[]{
		/????????????????
	};



	UINT64 origcallheader = origcall - ??????????????;

	if (ULONG oldProtect; Hooks::_virtualprot((void*)origcallheader, sizeof(restoreData), PAGE_EXECUTE_READWRITE, &oldProtect)) {

		memcpy((void*)origcallheader, restoreData, sizeof(restoreData));
		Hooks::_virtualprot((void*)origcallheader, sizeof(restoreData), oldProtect, &oldProtect);
	}



	logger->log("restoreInjectEntryPoint complete\n");
}