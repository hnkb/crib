
if (TARGET glfw AND TARGET glfw3webgpu)
	return()
endif()

include(FetchContent)


if (NOT EMSCRIPTEN)
	FetchContent_Declare(
		glfw
		SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glfw"
		GIT_REPOSITORY "https://github.com/glfw/glfw.git"
		GIT_TAG "3.4"
		GIT_SHALLOW ON
		EXCLUDE_FROM_ALL)

	FetchContent_MakeAvailable(glfw)
else()
	add_library(glfw INTERFACE)
	target_link_options(glfw INTERFACE -sUSE_GLFW=3)
endif()

include("fetch-WebGPU.cmake")
add_subdirectory("glfw3webgpu")
