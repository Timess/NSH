#pragma once
#ifndef GUI_CPP
#include "ntdll.h"
#endif
#include <asmjit/asmjit.h>
#include <iostream>
#include <vector>
#include <optional>

#include "Logger.h"


#define trampolineSize 0x10000
#define HOOKCALL [](HookClass* _this, Assembler& a1, Assembler& a2)

#define DOHOOK(x) this->hookClass##x##->doHook(__FUNCTION__, this->originalFunction##x##, HOOKCALL

#define HOOKFUNC(x) hookedFunction##x

class HookClass;

using CustomHook = void (*)(HookClass*);
using PreHook = UINT64(*)(HookClass*, asmjit::x86::Assembler&, asmjit::x86::Assembler&);

class HookClass
{
public:
	HookClass(UINT64& trampolineAddress, UINT64 hookAddress, CustomHook customHook = 0, CustomHook customUnHook = 0);
	~HookClass();

	void doHook();
	template<typename T>
	void doHook(const char* functionName, T& originalFunction, PreHook p) {
		logger->log("%s Start hooking: 0x%p, trampoline: 0x%p\n", functionName, this->hookAddress, this->trampolineAddress);

		asmjit::x86::Assembler a1(&this->hookcode);
		asmjit::x86::Assembler a2(&this->trampolinecode);

		originalFunction = reinterpret_cast<T>(p(this, a1, a2));

		this->doHook();

		logger->log("%s Hooked\n", functionName);
	}
	void unHook();
public:
	UINT64 trampolineAddress;
	UINT64 hookAddress;

	asmjit::Environment env;

	asmjit::StringLogger hooklogger;
	asmjit::CodeHolder hookcode;

	asmjit::StringLogger trampolinelogger;
	asmjit::CodeHolder trampolinecode;

	std::vector<UINT8> originalBytes;

	CustomHook customHook;
	CustomHook customUnHook;
};

