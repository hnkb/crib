
#include <Crib/Application.h>
#include <Crib/Window.h>


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int nCmdShow)
{
	try
	{
		crib::core::window main;
		return crib::core::application::messageLoop();
	}
	catch (std::exception e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
		return -1;
	}
}
