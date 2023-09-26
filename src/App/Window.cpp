
#include <crib/App>
#include "../Graphics/OpenGL/Context.h"

using namespace crib;


App::Window::Window(Window&& other)
{
	*this = std::move(other);
}


void App::Window::createGraphicsContext(Options options)
{
	if (context)
	{
		delete context;
		context = nullptr;
	}

	if (!impl)
		return;

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


void App::Window::onPosChanged(int2 pos)
{}

void App::Window::onSizeChanged(int2 dims)
{
	// try
	//{
	if (context)
		context->resize(dims);
	//}
	// catch (Graphics::Base::ContextInvalid e)
	//{
	//	createGraphicsContext({});
	//}
}
