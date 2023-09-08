
#include <crib/App>
#include "../Graphics/OpenGL/Context.h"

using namespace crib;


App::Window::Window(Window&& other)
{
	*this = std::move(other);
}

App::Window& App::Window::operator=(Window&& other)
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


void App::Window::createGraphicsContext(Options options)
{
	if (options.preferEngine == Engine::any || options.preferEngine == Engine::openGL)
		context = new Graphics::OpenGL::Context(*this);

	if (context)
	{
		options.title = context->description;
		setOptions(options);
	}
}

void App::Window::draw()
{
	if (context)
		context->draw();
}
