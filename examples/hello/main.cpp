
// #include <crib/App>
#include <crib/File>
#include <iostream>

using namespace crib;
using namespace std;


int main()
{
	cout << "Hello crib!\n";

	{
		auto path = filesystem::current_path() / "cmake_install.cmake";

		File file(path, "rb");

		cout << "File name is: " << path.string() << "\n";
		cout << "File size is: " << file.size() << "\n";

		char buffer[100];
		fread(buffer, 1, 99, file);
		cout << "First 99 bytes are:\n\n" << buffer << "\n\n";
	}

	{
		cout << "Executable: " << App::executablePath() << "\n";

		File::MemoryMapped exec(App::executablePath());
		cout << "First bytes: " << exec[0] << exec[1] << exec[2] << "\n";
	}

	return 0;
}
