
#include "stdafx.h"
#include "crib.h"


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int nCmdShow)
{
	try
	{
		crib::core::application app;
		return app.run();
	}
	catch (std::exception e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_ICONERROR);
		return -1;
	}
}
