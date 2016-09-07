
#include "stdafx.h"
#include "context.h"
#include "d3d11/d3d11_context.h"
#include "d3d12/d3d12_context.h"

using crib::graphics::base::context;


std::unique_ptr<context> context::create(crib::core::settings& setting, const HWND handle)
{
	auto type = setting.get(L"graphics", L"d3d11");

	if (type == L"d3d11")
		return std::unique_ptr<context>(new dx11::context(handle, setting));

	if (type == L"d3d12")
		return std::unique_ptr<context>(new dx12::context(handle));

	throw std::invalid_argument("crib::graphics::context type is invalid.");
}

void context::attach_renderer(crib::graphics::base::renderer* rndr)
{
	renderer.reset(rndr);
}
