
#include "DemoWindow.h"
#include <Crib/Application.h>


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int nCmdShow)
{
	try
	{
		CribDemo::Window main;
		return Crib::Application::messageLoop();
	}
	catch (std::exception e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
		return -1;
	}
}
