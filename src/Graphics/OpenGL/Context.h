
#pragma once

#include <crib/Graphics>
#include <crib/App>

#if defined(_WIN32)
#	include <crib/Platform/Win>
#	define PLATFORM_GL_CONTEXT HGLRC
#	define PLATFORM_OWNER_TYPE HWND
#elif defined(__unix__)
#	include <glad/glad_glx.h>
#	define PLATFORM_GL_CONTEXT GLXContext
#	define PLATFORM_OWNER_TYPE crib::Platform::X11::Window&

namespace crib::Platform::X11
{
	class Window;
}
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
		PLATFORM_OWNER_TYPE owner;
	};

}
