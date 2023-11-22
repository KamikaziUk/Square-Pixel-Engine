#include "test.h"

#include <windows.h>
#pragma comment(lib, "winmm.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
	SquareEngine::PrintHelloWorld();
	system("pause");
	return 0;
}