
#pragma once

#include <Windows.h>


class com_initialize
{
public:
	com_initialize() : hr(CoInitialize(nullptr)) {}
	~com_initialize() { if (SUCCEEDED(hr)) CoUninitialize(); }
	operator HRESULT() const { return hr; }
	operator bool() const { return SUCCEEDED(hr); }

private:
	HRESULT hr;
};

