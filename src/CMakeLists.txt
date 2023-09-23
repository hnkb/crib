
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
		"Platform/Windows/Console_win.cpp"
	)

	if (MSVC)
		target_link_options(crib PUBLIC /INCLUDE:?globalSetupConsoleUtf8@Windows@Platform@crib@@3VSetupConsoleUtf8@123@A)
	elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		target_link_options(crib PUBLIC -Xlinker /INCLUDE:?globalSetupConsoleUtf8@Windows@Platform@crib@@3VSetupConsoleUtf8@123@A)
	else()
		target_link_options(crib PUBLIC -u _ZN4crib8Platform7Windows22globalSetupConsoleUtf8E)
	endif()
endif()
