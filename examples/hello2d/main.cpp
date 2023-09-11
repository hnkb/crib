
#include <crib/App>
#include <iostream>

using namespace crib;


float2 offset = { 0.f, 0.f };


class MyWindow : public App::Window
{
public:
	MyWindow() : Window(Options { "HelloTriangle", { 400, 300 }, { 800, 800 } }) {}
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
	void onMouseEvent(const App::MouseEvent& ev) override
	{
		using App::MouseEvent;

		if (ev.type == MouseEvent::Type::ButtonDown)
		{
			if (dragStartPos.x == -1)
				dragStartPos = ev.pos;
		}
		else if (ev.type == MouseEvent::Type::ButtonUp)
		{
			dragStartPos = { -1 };
			this->draw();
		}
		else if (ev.type == MouseEvent::Type::Move && dragStartPos.x != -1)
		{
			offset = toFloat2(ev.pos - dragStartPos) / 400.f;
			offset.y *= -1.f;
			draw();
		}
	}

private:
	Options opt;
	int2 dragStartPos = { -1 };
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
