
#include "App.h"
#include "../../Graphics/OpenGL/Context.h"
#include <stdexcept>
#include <cstring>

using namespace crib;
using namespace crib::Platform;


X11::Window::Window(crib::App::Window* owner, const crib::App::Window::Options& opt)
	: owner(owner)
{
	App::open();
	auto& disp = App::display;

	pixelFormat = GLX::choosePixelFormat(disp);
	auto vi = glXGetVisualFromFBConfig(disp, pixelFormat);

	// create color map
	colorMap = XCreateColormap(disp, RootWindow(disp, vi->screen), vi->visual, AllocNone);

	XSetWindowAttributes swa;
	swa.colormap = colorMap;
	swa.background_pixmap = None;
	swa.border_pixel = 0;
	swa.event_mask = StructureNotifyMask;

	wnd = XCreateWindow(
		disp,
		RootWindow(disp, vi->screen),
		opt.pos.x,
		opt.pos.y,
		opt.size.x,
		opt.size.y,
		0,
		vi->depth,
		InputOutput,
		vi->visual,
		CWBorderPixel | CWColormap | CWEventMask,
		&swa);

	XFree(vi);

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

	XMapRaised(disp, wnd);
}

X11::Window::~Window()
{
	close();
	owner->impl = nullptr;
}

void X11::Window::close()
{
	if (owner->context)
	{
		delete owner->context;
		owner->context = nullptr;
	}

	if (alreadyDeleted)
		return;
	alreadyDeleted = true;

	XDestroyWindow(App::display, wnd);
	XFreeColormap(App::display, colorMap);

	App::close();
}

void X11::Window::proc(XEvent& event)
{
	auto& disp = App::display;

	switch (event.type)
	{
		case Expose:
			owner->draw();
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

	impl = new X11::Window(this, opt);

	createGraphicsContext(opt);

	if (context)
	{
		XWindowAttributes gwa;
		XGetWindowAttributes(X11::App::display, ((X11::Window*)impl)->wnd, &gwa);
		context->onResize({ gwa.width, gwa.height });
	}
}

App::Window::~Window()
{
	if (context)
	{
		delete context;
		context = nullptr;
	}
	if (impl)
		delete (X11::Window*)impl;
}

App::Window& App::Window::operator=(Window&& other)
{
	if (this != &other)
	{
		impl = other.impl;
		context = other.context;
		other.impl = nullptr;
		other.context = nullptr;

		if (impl)
			((Platform::X11::Window*)impl)->owner = this;
	}
	return *this;
}


void App::Window::close()
{
	XEvent ev;
	memset(&ev, 0, sizeof(ev));
	ev.xclient.type = ClientMessage;
	ev.xclient.window = ((X11::Window*)impl)->wnd;
	ev.xclient.message_type = XInternAtom(X11::App::display, "WM_PROTOCOLS", true);
	ev.xclient.format = 32;
	ev.xclient.data.l[0] = X11::App::windowClosed;
	ev.xclient.data.l[1] = CurrentTime;
	XSendEvent(X11::App::display, ev.xclient.window, False, NoEventMask, &ev);
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
