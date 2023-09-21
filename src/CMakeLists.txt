
add_library(crib STATIC "")


target_compile_features(crib PUBLIC cxx_std_17)

target_include_directories(crib PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

include("Platform/CMake/Compiler-config.cmake")

target_sources(crib PRIVATE
	"File/Stream.cpp"
)


if (WIN32)

elseif (UNIX)

	# for Ubuntu, Debian
	#    sudo apt install libx11-dev libgl1-mesa-dev

	# for openSUSE
	#    sudo zypper install libX11-devel Mesa-libGL-devel

	# for Fedora, Red Hat, Amazon
	#    sudo dnf install libX11-devel mesa-libGL-devel

	# for Clear Linux
	#    sudo swupd bundle-add devpkg-libX11
	# or
	# - libX11client
	# - lib-opengl
	# - devpkg-ba
	# - devpkg-libX11
	# - libX11client
	# - mesa


	find_package(X11 REQUIRED)
	target_link_libraries(crib PRIVATE X11::X11)

	#set(OpenGL_GL_PREFERENCE "GLVND")
	find_package(OpenGL REQUIRED)
	target_link_libraries(crib PRIVATE OpenGL::OpenGL OpenGL::GLX)
endif()
