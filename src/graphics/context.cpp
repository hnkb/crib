
#include "stdafx.h"
#include "context.h"
#include "d3d11/d3d11_context.h"

using crib::graphics::context;


std::unique_ptr<context> context::create(const std::wstring type, const HWND handle)
{
	if (type == L"d3d11") return std::unique_ptr<context>(new d3d11_context(handle));
	throw std::invalid_argument("crib::graphics::context type is invalid.");
}
