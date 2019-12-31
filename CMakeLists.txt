
cmake_minimum_required (VERSION 3.8)

project (Crib)


add_library (Crib STATIC "")


target_include_directories (Crib
	PUBLIC
		"include")

target_compile_features (Crib
	PUBLIC
		cxx_std_17)


if (MSVC)
	# CMake 3.14 and older add /W3 by default
	# CMP0092 is meant to remove /W3, but does not work in Visual Studio-bundled CMake
	string(REGEX REPLACE " */W3 *" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

	target_compile_options (Crib
		PRIVATE
			/permissive- /W4 /WX
			/utf-8)
else()
	target_compile_options (Crib
		PRIVATE
			-Wall -Wextra -pedantic -Werror)
endif()


target_sources (Crib PRIVATE
	"include/Crib/Application.h"
	"src/Platform/Error.cpp")

if (WIN32)

	target_sources (Crib PRIVATE
		"include/Crib/Platform/Windows.h"
		"src/Platform/Windows/COM.cpp")

elseif (UNIX)

	target_sources (Crib PRIVATE
		"include/Crib/Platform/POSIX.h")

endif()
