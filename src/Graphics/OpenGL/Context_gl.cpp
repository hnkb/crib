
#include "Context.h"
#include <glad/glad.h>

using crib::Graphics::OpenGL::Context;


void Context::onResize(int2 dims)
{
	glViewport(0, 0, dims.x, dims.y);
}
