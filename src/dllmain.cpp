#include <Windows.h>
#include <thread>
#include <iostream>
#include <cstdint>

#include <hack_core.h>

bool beforeInject(HINSTANCE dll) {
	return true;
}

void init(HINSTANCE dll) {
	HackCore->init(dll);
}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID lp_reserved) {
	if (reason == DLL_PROCESS_ATTACH && beforeInject(dll)) {
		std::thread(init, dll).detach();
	}

	return TRUE;
}