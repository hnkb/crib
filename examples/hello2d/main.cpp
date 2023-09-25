
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
				dragStart = camera.pixelToWorld(toFloat2(ev.pos));
		}
		else if (ev.type == MouseEvent::Type::ButtonUp)
		{
			dragStart = { std::numeric_limits<float>::quiet_NaN(), -1 };
			this->draw();
		}
		else if (ev.type == MouseEvent::Type::Move && !std::isnan(dragStart.x))
		{
			// our mouse pointer is now pointing at a world position, but, we want it to be
			// pointing to the same position as dragStart => so fix the offset to achieve this
			camera.view.offset -= dragStart - camera.pixelToWorld(toFloat2(ev.pos));
			draw();
		}
		else if (ev.type == MouseEvent::Type::Wheel)
		{
			// first, I convert from pixel space to range -1...1
			auto screen = camera.pixelToClip(toFloat2(ev.pos));

			// this is the reverse of the transformation in my shader:
			// screen = (world + offset) * scale

			// I have two values for scale (before and after).
			// I want the screen to point to the same world coordinates, but it does not.
			// So, I have to add the difference between worldBefore and worldAfter to my offset.
			//
			// Therefore:
			//     world_before = screen / scale_before - offset
			//  -  world_after  = screen / scale_after  - offset
			// --------------------------------------------------
			//        diff       = (screen / scale_before) - (screen / scale_after)

			auto worldBefore = camera.pixelToWorld(toFloat2(ev.pos));

			camera.view.scale *= powf(1.25f, ev.wheel);

			auto worldAfter = camera.pixelToWorld(toFloat2(ev.pos));

			camera.view.offset -= worldBefore - worldAfter;

			draw();
		}
	}

private:
	float2 dragStart = { std::numeric_limits<float>::quiet_NaN(), -1 };
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
