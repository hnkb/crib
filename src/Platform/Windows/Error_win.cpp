
#include <Crib/Platform/Windows.h>

using crib::core::windows_error;

std::string windows_error::get_message(const HRESULT hr, const char* operation_name)
{
	std::string retval = std::string(operation_name) + " failed.";
	char* msg;

	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msg, 0, nullptr);

	if (msg)
	{
		retval = retval + "\n" + msg;
		LocalFree(msg);
	}
	else
	{
		retval += " Unknown error " + std::to_string(hr) + ".";
	}

	return retval;
}
