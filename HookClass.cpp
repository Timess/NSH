#include "HookClass.h"
#include "Logger.h"
#include "Hooks.h"

using namespace asmjit;
using namespace asmjit::x86;

HookClass::HookClass(UINT64& trampolineAddress, UINT64 hookAddress, CustomHook customHook, CustomHook customUnHook) :
	trampolineAddress(trampolineAddress), hookAddress(hookAddress), customHook(customHook), customUnHook(customUnHook)
{
	env.setArch(asmjit::Environment::kArchX64);


	hooklogger.addFlags(asmjit::FormatOptions::kFlagHexImms);
	hooklogger.addFlags(asmjit::FormatOptions::kFlagHexOffsets);

	hookcode.init(env, (UINT64)(hookAddress));
	hookcode.setLogger(&hooklogger);



	trampolinelogger.addFlags(asmjit::FormatOptions::kFlagHexImms);
	trampolinelogger.addFlags(asmjit::FormatOptions::kFlagHexOffsets);

	trampolinecode.init(env, (UINT64)(trampolineAddress));
	trampolinecode.setLogger(&trampolinelogger);

	trampolineAddress += trampolineSize;
}


void HookClass::doHook() {


	if (customHook) return customHook(this);


	CodeBuffer& hookbuffer = hookcode.textSection()->buffer();
	std::stringstream sshooklog_writebytes;

	for (size_t i = 0; i < hookbuffer.size(); i++) {
		char temp[10]{};
		sprintf_s(temp, "%02X ", hookbuffer.data()[i]);
		sshooklog_writebytes << temp;
	}

	logger->log("hookAddress writing bytes(0x%llX): \n%s\n", hookbuffer.size(), sshooklog_writebytes.str().c_str());
	logger->log("hookAddress writing opcodes: \n%s\n", hooklogger.data());


	CodeBuffer& trampolinebuffer = trampolinecode.textSection()->buffer();
	std::stringstream sstrampolinelog_writebytes;

	for (size_t i = 0; i < trampolinebuffer.size(); i++) {
		char temp[10]{};
		sprintf_s(temp, "%02X ", trampolinebuffer.data()[i]);
		sstrampolinelog_writebytes << temp;
	}

	logger->log("trampolineAddress writing bytes(0x%llX): \n%s\n", trampolinebuffer.size(), sstrampolinelog_writebytes.str().c_str());
	logger->log("trampolineAddress writing opcodes: \n%s\n", trampolinelogger.data());

	memcpy((void*)trampolineAddress, trampolinebuffer.data(), trampolinebuffer.size());

	if (ULONG oldProtect; Hooks::_virtualprot((void*)hookAddress, hookbuffer.size(), PAGE_EXECUTE_READWRITE, &oldProtect)) {

		std::copy((char*)hookAddress, (char*)hookAddress + hookbuffer.size(), std::back_inserter(originalBytes));

		std::stringstream ssoriginalBytes;
		for (size_t i = 0; i < originalBytes.size(); i++) {
			char temp[10]{};
			sprintf_s(temp, "%02X ", originalBytes.data()[i]);
			ssoriginalBytes << temp;
		}

		logger->log("originalBytes(0x%llX): \n%s\n", originalBytes.size(), ssoriginalBytes.str().c_str());

		memcpy((void*)hookAddress, hookbuffer.data(), hookbuffer.size());

		Hooks::_virtualprot((void*)hookAddress, hookbuffer.size(), oldProtect, &oldProtect);
	}
}


void HookClass::unHook() {

	if (customUnHook) return customUnHook(this);

	CodeBuffer& hookbuffer = hookcode.textSection()->buffer();
	CodeBuffer& trampolinebuffer = trampolinecode.textSection()->buffer();

	if (ULONG oldProtect; Hooks::_virtualprot((void*)hookAddress, hookbuffer.size(), PAGE_EXECUTE_READWRITE, &oldProtect)) {

		memcpy((void*)hookAddress, originalBytes.data(), originalBytes.size());
		memset((void*)trampolineAddress, 0, trampolinebuffer.size());

		Hooks::_virtualprot((void*)hookAddress, hookbuffer.size(), oldProtect, &oldProtect);
	}


}

HookClass::~HookClass() {

	this->unHook();
}