
#pragma once

#include <crib/Graphics>
#include <crib/App>

#if defined(_WIN32)
#	include <crib/Platform/Win>
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
		HGLRC ctx = nullptr;
		const App::Window& owner;
	};

}
