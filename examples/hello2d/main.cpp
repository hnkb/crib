
#include <crib/App>
#include <iostream>

using namespace crib;


class MyWindow : public App::Window
{
public:
	void onSizeChanged(int2 dims) override
	{
		Window::onSizeChanged(dims);
		printf("size\n");
	}
	void onPosChanged(int2 pos) override
	{
		Window::onPosChanged(pos);
		printf("move\n");
	}
};

int main()
{
	try
	{
		// App::Window win;
		MyWindow win2;
		auto win = std::move(win2);
		return App::run();
	}
	catch (std::exception& ex)
	{
		std::cout << "App finished with error:\n" << ex.what() << "\n";
	}

	return 0;
}
