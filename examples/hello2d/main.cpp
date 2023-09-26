
#include <crib/App>
#include <crib/Graphics>
#include <iostream>
#include <cmath>

using namespace crib;


Graphics::Camera camera;
std::string text;


class MyWindow : public App::Window
{
public:
	MyWindow() : Window(Options { "HelloTriangle", { 400, 300 }, { 800, 800 } })
	{
		camera.view.scale = .25f;
		camera.setViewport(getOptions().size);
	}
	void onSizeChanged(int2 dims) override
	{
		Window::onSizeChanged(dims);
		printf("size %d %d\n", dims.x, dims.y);
		camera.setViewport(dims);

		// TODO: update scale so that objects are in similar size as before
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
			if (text.empty())
				close();
			else
				text.clear();
		}
		else if (str[0] == 8)  // Backspace
		{
			// remove all UTF-8 continuation bytes
			while (text.size() && (text.back() & 0xc0) == 0x80)
				text.pop_back();
			if (text.size())
				text.pop_back();
		}
		else
		{
			text += str;
		}
		draw();
	}
	void onMouseEvent(const App::MouseEvent& ev) override
	{
		using App::MouseEvent;

		if (ev.type == MouseEvent::Type::ButtonDown)
		{
			if (std::isnan(dragStart.x))
				dragStart = camera.pixelToWorld((float2)ev.pos);
		}
		else if (ev.type == MouseEvent::Type::ButtonUp)
		{
			dragStart = float2::nan();
			this->draw();
		}
		else if (ev.type == MouseEvent::Type::Move && !std::isnan(dragStart.x))
		{
			// our mouse pointer is now pointing at a world position, but, we want it to be
			// pointing to the same position as dragStart => so fix the offset to achieve this
			camera.view.offset -= dragStart - camera.pixelToWorld((float2)ev.pos);
			draw();
		}
		else if (ev.type == MouseEvent::Type::Wheel)
		{
			// I want the cursor position on screen to point to the same world coordinates as
			// before the scaling. So, I subtract the difference between the new world position
			// and the original one from the view offset.

			auto before = camera.pixelToWorld((float2)ev.pos);

			camera.view.scale *= powf(1.25f, ev.wheel);

			auto after = camera.pixelToWorld((float2)ev.pos);

			camera.view.offset -= before - after;

			draw();
		}
	}

private:
	float2 dragStart = float2::nan();
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
