
// #include <crib/App>
#include <crib/File>
#include <iostream>

using namespace crib;
using namespace std;


int main()
{
	cout << "Hello crib!\n";

	{
		auto file = App::executablePath().parent_path() / "test.dat";

		cout << "File name is " << file << "\n";

		vector<int> vals { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		crib::File::writeAll(vals, file);

		File::MemoryMapped<int> m(file);

		if (0)
			auto n = std::move(m);

		cout << "Size is " << m.size() << "\n";
		cout << "Data is " << m.data() << "\n";
		cout << "Item 3 is " << m[3] << "\n";
	}

	return 0;
}
