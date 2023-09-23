
add_library(crib STATIC "")


target_compile_features(crib PUBLIC cxx_std_17)

target_include_directories(crib PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

include("Platform/CMake/Compiler-config.cmake")

target_sources(crib PRIVATE
	"File/Stream.cpp"
)
