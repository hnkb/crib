
add_library(crib STATIC "")


target_compile_features(crib PUBLIC cxx_std_17)

target_include_directories(crib PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

include("Platform/CMake/Compiler-config.cmake")


target_sources(crib PRIVATE
	"App/Window.cpp"
	"File/Stream.cpp"
	"Graphics/Text-Layout/Font.cpp"
	"Graphics/Text-Layout/Layout.cpp"
	"Graphics/OpenGL/Context_gl.cpp"
	"Graphics/Scene.cpp"
)

target_link_libraries(crib PRIVATE glad ttf2mesh)


if (WIN32)
	target_sources(crib PRIVATE
		"App/Win/App_win.cpp"
		"App/Win/Window_win.cpp"
		"Graphics/OpenGL/Context_wgl.cpp"
		"Platform/Win/COM.cpp"
		"Platform/Win/Error_win.cpp"
		"Platform/Win/String_win.cpp"
	)

	target_link_libraries(crib PRIVATE opengl32)
elseif (APPLE)
else()
	# Assuming Unix and Linux here. We can also check (UNIX AND NOT APPLE)

	#target_compile_options(crib PRIVATE -mavx -Wall -Wextra -pedantic -Werror)

	target_sources(crib PRIVATE
		"App/Xlib/App_x11.cpp"
		"App/Xlib/Window_x11.cpp"
		"Graphics/OpenGL/Context_glx.cpp"
	)

	find_package(X11 REQUIRED)
	target_link_libraries(crib PRIVATE X11::X11)
	
	find_package(OpenGL REQUIRED)
	target_link_libraries(crib PRIVATE OpenGL::OpenGL OpenGL::GLX)
endif()
