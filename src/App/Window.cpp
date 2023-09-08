
#include <crib/App>
#include "../Graphics/OpenGL/Context.h"

using crib::App::Window;


Window::Window(Window&& other)
{
	*this = std::move(other);
}

Window& Window::operator=(Window&& other)
{
	if (this != &other)
	{
		impl = other.impl;
		context = other.context;
		other.impl = nullptr;
		other.context = nullptr;
	}
	return *this;
}


void Window::createGraphicsContext(Options options)
{
	if (options.preferEngine == Engine::any || options.preferEngine == Engine::openGL)
		context = new Graphics::OpenGL::Context(*this);

	if (context)
	{
		options.title = context->description;
		setOptions(options);
	}
}

void Window::draw()
{
	if (context)
		context->draw();
}
