
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

		File::Stream file(path, "rb");

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

	{
		auto file = App::executablePath().parent_path() / "test.dat";

		cout << "File name is " << file << "\n";

		vector<int> vals { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		// File::writeAll(vals, file);
		File::Stream(file, "wb") << vals;

		File::MemoryMapped<int> m(file);

		if (0)
			auto n = std::move(m);

		cout << "Size is " << m.size() << "\n";
		cout << "Data is " << m.data() << "\n";
		cout << "Item 3 is " << m[3] << "\n";
	}

	return 0;
}
