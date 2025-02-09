
if (TARGET glad)
	return()
endif()

if (NOT EMSCRIPTEN)
	add_library(glad STATIC "glad/src/glad.c")
	target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glad/include")
else()
	add_library(glad INTERFACE)
	target_link_options(glad INTERFACE -sMAX_WEBGL_VERSION=2)
endif()
