
#include <crib/App>
#include <iostream>


int main()
{
	try
	{
		crib::App::Window win;
		return crib::App::run();
	}
	catch (std::exception& ex)
	{
		std::cout << "App finished with error:\n" << ex.what() << "\n";
	}

	return 0;
}
