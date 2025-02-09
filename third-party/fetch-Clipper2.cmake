
if (TARGET Clipper2)
	return()
endif()

include(FetchContent)


FetchContent_Declare(
	Clipper2
	SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/clipper2"
	SOURCE_SUBDIR CPP
	GIT_REPOSITORY "https://github.com/AngusJohnson/Clipper2.git"
	GIT_TAG "Clipper2_1.4.0"
	GIT_SHALLOW ON
	EXCLUDE_FROM_ALL)

set(CLIPPER2_EXAMPLES OFF)
set(CLIPPER2_TESTS OFF)

FetchContent_MakeAvailable(Clipper2)
