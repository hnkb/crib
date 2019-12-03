
#include "DemoWindow.h"
#include <Crib/Application.h>


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int nCmdShow)
{
	try
	{
		demo_window main;
		return crib::core::application::messageLoop();
	}
	catch (std::exception e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
		return -1;
	}
}
