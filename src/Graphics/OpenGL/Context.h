
#pragma once

#include <crib/Graphics>
#include <crib/App>

#if defined(_WIN32)
#	include <crib/Platform/Win>
#	define PLATFORM_GL_CONTEXT HGLRC
#endif


namespace crib::Graphics::OpenGL
{

	class Context : public Graphics::Context
	{
	public:
		Context(const App::Window&);
		virtual ~Context();

		virtual void draw() override;
		virtual void onResize(int2 dims) override;

	private:
		void drawPlatformIndependent();
		void readDeviceDescription(int swapInterval);

		PLATFORM_GL_CONTEXT ctx = nullptr;
		const App::Window& owner;
	};

}
