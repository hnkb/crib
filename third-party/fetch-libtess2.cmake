
if (TARGET libtess2)
	return()
endif()

include(FetchContent)


FetchContent_Declare(
	libtess2
	SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libtess2"
	GIT_REPOSITORY "https://github.com/memononen/libtess2.git"
	GIT_TAG "v1.0.2"
	GIT_SHALLOW ON
	EXCLUDE_FROM_ALL)

FetchContent_MakeAvailable(libtess2)

file(GLOB LIBTESS2_SOURCES "libtess2/Source/*.c")
add_library(libtess2 STATIC ${LIBTESS2_SOURCES})
target_include_directories(libtess2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libtess2/Include")
