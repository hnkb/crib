
add_library(crib STATIC "")


target_compile_features(crib PUBLIC cxx_std_17)

target_include_directories(crib PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

include("Platform/CMake/Compiler-config.cmake")


target_sources(crib PRIVATE
	"App/Window.cpp"
	"File/Stream.cpp"
	"Graphics/OpenGL/Context_gl.cpp"
)

target_link_libraries(crib PRIVATE glad)


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
endif()
