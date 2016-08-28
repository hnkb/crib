
#pragma once

#include <stdexcept>
#include <Windows.h>


class com_initialize
{
public:
	com_initialize()
	{
		if (FAILED(CoInitialize(nullptr)))
			throw std::runtime_error("CoInitialize() failed.");
	}

	~com_initialize()
	{
		CoUninitialize();
	}
};
