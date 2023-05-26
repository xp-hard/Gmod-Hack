#include <Windows.h>
#include <thread>
#include <iostream>

bool beforeInject(HINSTANCE dll) {
	return true;
}

void init(HINSTANCE dll) {
	AllocConsole();
	SetConsoleTitleA("Gmod Hack");

	freopen("conout$", "w", stdout);
	freopen("contout$", "w", stderr);

	std::cout << "[L] Initialized!";

	FreeConsole();
	FreeLibraryAndExitThread(dll, 0);

}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID lp_reserved) {
	DisableThreadLibraryCalls(dll);

	if (reason == DLL_PROCESS_ATTACH && beforeInject(dll)) {
		std::thread(init, dll).detach();
	}
}