
add_executable(hello2d WIN32 "main.cpp")

target_link_libraries(hello2d PRIVATE crib)

if (MSVC)
	target_link_options(hello2d PRIVATE /ENTRY:mainCRTStartup)
elseif (WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")) # and CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
	target_link_options(hello2d PRIVATE -Xlinker /ENTRY:mainCRTStartup)
endif()
