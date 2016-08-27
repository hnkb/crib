
#pragma once

#include <string>
#include <Windows.h>


class window
{
public:
	window() : window(L"crib") {}
	window(const std::wstring title) : window(L"crib", title) {}
	window(const std::wstring className, const std::wstring title);

	window(const window& other) { throw "Copy constructor not supported."; }
	window(window&& other) { throw "Move constructor not supported."; }
	window& operator=(const window& other) { throw "Copy assignment not supported."; }
	window& operator=(window&& other) { throw "Move assignment not supported."; }

	virtual ~window();

	void set_title(const std::wstring title);
	std::wstring get_title() const;
	HWND get_handle() const { return handle; }

protected:
	virtual LRESULT proc(const UINT message, const WPARAM wParam, const LPARAM lParam);
	static LRESULT CALLBACK proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND handle = nullptr;
};


#define WM_APP_WINDOWCOUNT (WM_APP + 1)
