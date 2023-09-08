
#include "Context.h"
#include <glad/glad.h>

using crib::Graphics::OpenGL::Context;


void Context::onResize(int2 dims)
{
	glViewport(0, 0, dims.x, dims.y);
}

void Context::readDeviceDescription(int swapInterval)
{
	std::string profile;
	{
		GLint mask;
		glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &mask);
		if (mask & GL_CONTEXT_CORE_PROFILE_BIT)
			profile = "Core";
		else if (mask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
			profile = "Compatibility";
	}

	std::string sync;
	{
		if (swapInterval == 1)
			sync = " (V-Sync)";
		else if (swapInterval == -1)
			sync = " (adaptive sync)";
	}

	description =
		std::string("GL ") + (char*)glGetString(GL_VERSION) + "  " + profile + "  GLSL "
		+ (char*)glGetString(GL_SHADING_LANGUAGE_VERSION) + "  |  "
		+ (char*)glGetString(GL_RENDERER) + sync;
}

void Context::drawPlatformIndependent()
{
	glClearColor(0.6f, 0.2f, 0.15f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
