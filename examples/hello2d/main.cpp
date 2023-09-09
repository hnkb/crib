
#include <crib/App>
#include <iostream>

using namespace crib;


class MyWindow : public App::Window
{
public:
	void onSizeChanged(int2 dims) override
	{
		Window::onSizeChanged(dims);
		printf("size %d %d\n", dims.x, dims.y);
	}
	void onPosChanged(int2 pos) override
	{
		Window::onPosChanged(pos);
		// printf("move\n");
	}
	void onKeyChar(const std::string& str) override
	{
		if (str[0] == 27)  // Escape
		{
			if (opt.title.empty())
				close();
			else
				opt.title.clear();
		}
		else if (str[0] == 8)  // Backspace
		{
			// remove all UTF-8 continuation bytes
			while (opt.title.size() && (opt.title.back() & 0xc0) == 0x80)
				opt.title.pop_back();
			if (opt.title.size())
				opt.title.pop_back();
		}
		else
		{
			opt.title += str;
		}
		setOptions(opt);
	}

private:
	Options opt;
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
