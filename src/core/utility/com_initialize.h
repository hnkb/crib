
#pragma once

#include "windows_error.h"
#include <Windows.h>


class com_initialize
{
public:
	com_initialize()
	{
		throw_if_failed(CoInitialize(nullptr), "COM initialization");
	}

	~com_initialize()
	{
		CoUninitialize();
	}
};
