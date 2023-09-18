
cmake_minimum_required(VERSION 3.15)


include("src/Platform/CMake/Git-version.cmake")

project(crib
	VERSION "${CRIB_VERSION_TAG}.${CRIB_VERSION_PATCH}"
	DESCRIPTION "crib: C++ library for rendering and processing"
	HOMEPAGE_URL https://github.com/hnkb/crib
)


if (TARGET crib)
	return()
endif()


add_subdirectory("third-party")
add_subdirectory("src")

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	enable_testing()
	add_subdirectory("test")

	add_subdirectory("examples")
endif()
