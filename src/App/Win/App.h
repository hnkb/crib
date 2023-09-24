
#pragma once

#include <crib/Platform/Win>


namespace crib::Platform::Win
{

	enum class Message : UINT { closed = (WM_APP + 1), quit = (WM_APP + 2) };

	constexpr auto windowClass = L"crib";

}
