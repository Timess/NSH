
#include "Hooks.h"
#include "lazy_importer.hpp"

#include <locale>

std::optional<Hooks> hooks;
static bool once = false;

extern "C" BOOL WINAPI _CRT_INIT(HMODULE moduleHandle, DWORD reason, LPVOID reserved);
extern "C" UINT64 __readr14();
extern "C" UINT64 __readr13();
__forceinline auto get_nt_headers(const std::uintptr_t image_base) -> IMAGE_NT_HEADERS*
{
	const auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*> (image_base);

	return reinterpret_cast<IMAGE_NT_HEADERS*> (image_base + dos_header->e_lfanew);
}


extern "C" __declspec(dllexport) UINT64 gacbase = 0;
extern "C" UINT64 origcall = 0;
extern "C" UINT64 origqword = 0;

extern "C" UINT64 __fastcall __origcall(UINT64 rcx, UINT64 rdx, UINT64 r8, UINT64 r9);

#define ENABLE_HOOK
//#define SPOOF_CALL
UINT64 __fastcall DllEntryPoint(UINT64 rcx, UINT64 rdx, UINT64 r8, UINT64 r9)
{

	static bool once = false;

#ifdef SPOOF_CALL
	std::add_pointer_t<decltype(DllEntryPoint)> t = (decltype(t))rdx;
#endif

	if (!once) {
		once = true;

		origcall = gacbase + 0x12388CE;
		origqword = gacbase + 0x61857F0;

		HMODULE hModule = (HMODULE)0x7FFAA0000000;
		PRUNTIME_FUNCTION pRuntimeFunc;
		ULONG64 uCnt;

		const auto nt_header = get_nt_headers((UINT64)hModule);
		IMAGE_IMPORT_DESCRIPTOR* import_description = (IMAGE_IMPORT_DESCRIPTOR*)((UINT64)hModule + nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

		if (!nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress || !nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size)
		{
#ifdef ENABLE_HOOK
#ifdef SPOOF_CALL

			return return_spoofer::spoof_call((void*)r8, t, rcx, 0ui64, 0ui64, 0ui64);
#else
			return __origcall(rcx, 0, 0, 0);
#endif
#else
			return 0;
#endif
	}

		LPSTR module_name = NULL;

		while ((module_name = (LPSTR)((UINT64)hModule + import_description->Name)))
		{
			uintptr_t base_image = (std::uintptr_t)LI_FN(GetModuleHandleA)(module_name);
			if (!base_image)
			{
				break;
			}

			auto import_header_data = (IMAGE_THUNK_DATA*)((UINT64)hModule + import_description->FirstThunk);

			while (import_header_data->u1.AddressOfData)
			{
				if (import_header_data->u1.Ordinal & IMAGE_ORDINAL_FLAG)
				{
					import_header_data->u1.Function = (UINT64)LI_FN(GetProcAddress)((HMODULE)base_image, (LPCSTR)(import_header_data->u1.Ordinal & 0xFFFF));
				}
				else {
					IMAGE_IMPORT_BY_NAME* ibn = (IMAGE_IMPORT_BY_NAME*)((UINT64)hModule + import_header_data->u1.AddressOfData);
					import_header_data->u1.Function = (UINT64)LI_FN(GetProcAddress)((HMODULE)base_image, (LPCSTR)ibn->Name);

				}
				import_header_data++;
			}
			import_description++;
		}

		PIMAGE_DATA_DIRECTORY pExceptionDir = &nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION];
		pRuntimeFunc = (PRUNTIME_FUNCTION)(pExceptionDir->VirtualAddress + (ULONG64)hModule);
		uCnt = pExceptionDir->Size / sizeof(RUNTIME_FUNCTION);

		if (!_CRT_INIT(hModule, 1, 0)) {
#ifdef ENABLE_HOOK
#ifdef SPOOF_CALL
			return return_spoofer::spoof_call((void*)r8, t, rcx, 0ui64, 0ui64, 0ui64);
#else
			return __origcall(rcx, 0, 0, 0);
#endif
#else
			return 0;
#endif
}

		//RtlAddFunctionTable(pRuntimeFunc, (DWORD)uCnt, (ULONG64)hModule);

		std::setlocale(LC_CTYPE, ".utf8");
		hooks.emplace(hModule);

	}

#ifdef ENABLE_HOOK
#ifdef SPOOF_CALL
	return return_spoofer::spoof_call((void*)r8, t, rcx, 0ui64, 0ui64, 0ui64);
#else
	return __origcall(rcx, 0, 0, 0);
#endif
#else
	return 0;
#endif
}