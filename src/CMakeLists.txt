
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
#	target_link_libraries(crib
#		PUBLIC
#			Comctl32
#			d3d11 d2d1 dwrite)
	if (CMAKE_COMPILER_IS_GNUCXX)
		target_link_libraries(crib PUBLIC windowscodecs)
	endif()
endif()
