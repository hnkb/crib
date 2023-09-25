
#include "Context.h"
#include "../../App/Xlib/App.h"
#include <stdexcept>

using namespace crib::Platform;

using crib::Graphics::OpenGL::Context;


namespace
{
	static bool ctxErrorOccurred = false;
	static int ctxErrorHandler(Display*, XErrorEvent*)
	{
		ctxErrorOccurred = true;
		return 0;
	}
}

Context::Context(const App::Window& window) : owner(window)
{
	description = "OpenGL  |  no or unknown device";

	auto& disp = X11::App::display;
	auto& xwnd = *(X11::Window*)owner.impl;
	auto& fbc = xwnd.pixelFormat;

	if (!GLAD_GLX_ARB_create_context || !GLAD_GLX_ARB_create_context_profile)
	{
		// using old-style GLX context as modern GLX is not found
		ctx = glXCreateNewContext(disp, fbc, GLX_RGBA_TYPE, 0, True);
	}
	else
	{
		// Install an X error handler so the application won't exit if GL 3.3 context creation
		// fails.
		//
		// Note this error handler is global.  All display connections in all threads of a
		// process use the same error handler, so be sure to guard against other threads issuing
		// X commands while this code is running.
		ctxErrorOccurred = false;
		auto oldHandler = XSetErrorHandler(&ctxErrorHandler);

		int2 contextAttribs[] = {
			{ GLX_CONTEXT_MAJOR_VERSION_ARB, 3 },
			{ GLX_CONTEXT_MINOR_VERSION_ARB, 3 },
			{ GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB },
			{ None, 0 }
		};

		ctx = glXCreateContextAttribsARB(disp, fbc, 0, True, (int*)contextAttribs);

		// Sync to ensure any errors generated are processed.
		XSync(disp, False);

		if (ctxErrorOccurred || !ctx)
		{
			// Couldn't create GL 3.3 context.  Fall back to old-style 2.x context.
			// When a context version below 3.0 is requested, implementations will return the
			// newest context version compatible with OpenGL versions less than version 3.0.
			contextAttribs[0].y = 1;
			contextAttribs[1].y = 0;
			ctx = glXCreateContextAttribsARB(disp, fbc, 0, True, (int*)contextAttribs);

			XSync(disp, False);
		}

		// Restore the original error handler
		XSetErrorHandler(oldHandler);
	}

	if (!ctx)
		throw std::runtime_error("Failed to create an OpenGL context");

	glXMakeCurrent(disp, xwnd.wnd, ctx);

	if (!gladLoadGL())
		throw std::runtime_error("Unable to load OpenGL");

	// enable V-Sync
	if (GLAD_GLX_EXT_swap_control)
		glXSwapIntervalEXT(disp, glXGetCurrentDrawable(), 1);  // or 0 to disable V-Sync

	if (GLAD_GLX_EXT_swap_control_tear)
		glXSwapIntervalEXT(disp, glXGetCurrentDrawable(), -1);  // to enable adaptive sync

	GLuint interval;
	glXQueryDrawable(disp, glXGetCurrentDrawable(), GLX_SWAP_INTERVAL_EXT, &interval);
	readDeviceDescription(interval);
}

Context::~Context()
{
	if (ctx)
	{
		glXMakeCurrent(X11::App::display, 0, 0);
		glXDestroyContext(X11::App::display, ctx);
	}
}

void Context::draw()
{
	if (ctx)
	{
		drawPlatformIndependent();
		glXSwapBuffers(X11::App::display, ((X11::Window*)owner.impl)->wnd);
	}
}


GLXFBConfig X11::GLX::choosePixelFormat(Display* disp)
{
	if (!gladLoadGLX(disp, DefaultScreen(disp)))
		throw std::runtime_error("Unable to load OpenGL");

	if (!glad_glXChooseFBConfig)
		throw std::runtime_error("invalid GLX version: GLX 1.3 or higher is required");


	// attributes we require
	const int2 visualAttribs[] = {
		{ GLX_X_RENDERABLE, True },
		{ GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT },
		{ GLX_RENDER_TYPE, GLX_RGBA_BIT },
		{ GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR },
		{ GLX_RED_SIZE, 8 },
		{ GLX_GREEN_SIZE, 8 },
		{ GLX_BLUE_SIZE, 8 },
		{ GLX_ALPHA_SIZE, 8 },
		{ GLX_DEPTH_SIZE, 24 },
		{ GLX_STENCIL_SIZE, 8 },
		{ GLX_DOUBLEBUFFER, True },
		// { GLX_SAMPLE_BUFFERS, 1 },
		// { GLX_SAMPLES, 4 },
		{ None, 0 }
	};

	int fbcount;
	auto fbc = glXChooseFBConfig(disp, DefaultScreen(disp), (int*)visualAttribs, &fbcount);

	if (!fbc)
		throw std::runtime_error("failed to retrieve a framebuffer config");


	// Pick the FB config/visual with the most samples per pixel
	int selectedIdx = -1, selectedSamples = -1;

	for (int i = 0; i < fbcount; i++)
	{
		if (auto vi = glXGetVisualFromFBConfig(disp, fbc[i]))
		{
			int samp_buf, samples;

			glXGetFBConfigAttrib(disp, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
			glXGetFBConfigAttrib(disp, fbc[i], GLX_SAMPLES, &samples);

			if (selectedIdx < 0 || ((samp_buf > 0) && (samples > selectedSamples)))
				selectedIdx = i, selectedSamples = samples;

			XFree(vi);
		}
	}

	auto retVal = fbc[selectedIdx];
	XFree(fbc);

	return retVal;
}
