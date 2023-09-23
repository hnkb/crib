
#include <crib/Platform/Win>


namespace crib::Platform::Windows
{
	class SetupConsoleUtf8
	{
	public:
		SetupConsoleUtf8()
		{
			SetConsoleCP(CP_UTF8);
			SetConsoleOutputCP(CP_UTF8);
			SetLastError(0);
		}
	};

	SetupConsoleUtf8 globalSetupConsoleUtf8;
}
