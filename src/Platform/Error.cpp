
#include <Crib/Application.h>

using namespace Crib;
using namespace std;


// Unfortunately, MinGW std::system_category() does not correctly report Windows error
// messages, so we have to do it ourselves.
#if defined(__MINGW32__)
#include <Crib/Platform/Windows.h>

namespace
{
	string getWindowsErrorMessage(DWORD code)
	{
		char* msg;

		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&msg, 0, nullptr);

		string output;

		if (msg)
		{
			output = msg;
			LocalFree(msg);
		}
		else
		{
			output = "Unknown error " + to_string(code);
		}

		return output;
	}
}
#endif


Error::Error(error_code _code, CallSite callSite)
	: system_error(move(_code))
{
#if defined(__MINGW32__)
	message = getWindowsErrorMessage(code().value());
#else
	message = code().message();
#endif

	if (callSite.description && *callSite.description)
		message = "Error "s + callSite.description + ": " + message;

	while (!message.empty() &&
		(message.back() == '\r' || message.back() == '\n' || message.back() == ' '))
		message.pop_back();

	if (callSite.syscall && *callSite.syscall)
		message += "\nWhile calling "s + callSite.syscall + " from:";

	message +=
		"\n  "s + callSite.function +
		"\n  "s + callSite.file + ":" + to_string(callSite.line);
}
