
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
	target_sources(crib PRIVATE
		"Platform/Win/COM.cpp"
		"Platform/Win/Error_win.cpp"
		"Platform/Win/String_win.cpp"
	)
endif()
