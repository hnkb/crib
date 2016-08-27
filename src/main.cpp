
#include "stdafx.h"
#include "core/application.h"


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int nCmdShow)
{
	application app;
	return app.run();
}
