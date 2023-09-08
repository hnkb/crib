
#include "App.h"
#include "../../Graphics/OpenGL/Context.h"
#include <stdexcept>

using namespace crib;
using namespace crib::Platform;


X11::Window::Window(const crib::App::Window::Options& opt)
{
	App::open();
	auto& disp = App::display;

	screen = DefaultScreen(disp);

	const auto black = BlackPixel(disp, screen);
	const auto white = WhitePixel(disp, screen);

	wnd = XCreateSimpleWindow(
		disp,
		DefaultRootWindow(disp),
		opt.pos.x,
		opt.pos.y,
		opt.size.x,
		opt.size.y,
		5,
		white,
		white);

	if (!wnd)
	{
		close();
		throw std::runtime_error("failed to create window");
	}


	XStoreName(disp, wnd, opt.title.c_str());

	XSaveContext(disp, wnd, App::windowClass, (XPointer)this);

	XSelectInput(
		disp,
		wnd,
		StructureNotifyMask | ExposureMask | ButtonPressMask | KeyPressMask);

	XSetWMProtocols(disp, wnd, &App::windowClosed, 1);

	gc = XCreateGC(disp, wnd, 0, nullptr);
	XSetBackground(disp, gc, white);
	XSetForeground(disp, gc, black);

	XClearWindow(disp, wnd);
	XMapRaised(disp, wnd);
}

X11::Window::~Window()
{
	close();
}

void X11::Window::close()
{
	if (alreadyDeleted)
		return;
	alreadyDeleted = true;

	XFreeGC(App::display, gc);
	XDestroyWindow(App::display, wnd);

	App::close();
}

void X11::Window::proc(XEvent& event)
{
	auto& disp = App::display;

	switch (event.type)
	{
		case Expose:
			break;

		case KeyPress:
			break;

		case ButtonPress:
			break;

		default:
			break;
	}
}

void X11::Window::setTitle(const std::string& title)
{
	XStoreName(App::display, wnd, title.c_str());
}


App::Window::Window() : Window(Options {})
{}

App::Window::Window(Options opt)
{
	if (opt.size.x <= 0)
	{
		opt.size.x = 640;
		opt.size.y = 480;
	}
	opt.pos.x = std::max(0, opt.pos.x);
	opt.pos.y = std::max(0, opt.pos.y);
	if (opt.title.empty())
		opt.title = "crib";

	impl = new X11::Window(opt);
}

App::Window::~Window()
{
	if (impl)
		delete (X11::Window*)impl;
}

App::Window::Options App::Window::getOptions() const
{
	return {};
}

App::Window& App::Window::setOptions(const Options& opt)
{
	if (impl)
		((X11::Window*)impl)->setTitle(opt.title);
	return *this;
}
