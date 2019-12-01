
add_executable(Crib WIN32 "")


target_include_directories(Crib PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/src"
)


target_sources(Crib PRIVATE
	"${PROJECT_SOURCE_DIR}/scenes/hello/hello_d3d11_renderer.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/hello/hello_d3d12_renderer.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/hello/hello_scene.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/hello3d/hello3d_assets.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/hello3d/hello3d_renderer_3d.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/hello3d/hello3d_scene.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/house/house_assets.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/house/house_scene.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/menu/menu_d3d11_renderer.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/menu/menu_scene.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/strawman/strawman_renderer.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/strawman/strawman_scene.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/tetris/tetris_renderer.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/tetris/tetris_scene.cpp"
	"${PROJECT_SOURCE_DIR}/scenes/scene_factory.cpp"
	"core/application.cpp"
	"core/settings.cpp"
	"core/timer.cpp"
	"core/window.cpp"
	"geometry/definition.cpp"
	"graphics/base/context.cpp"
	"graphics/dx11/context_dx11.cpp"
	"graphics/dx11/effect_dx11.cpp"
	"graphics/dx11/vertex_buffer_dx11.cpp"
	"graphics/dx12/context_dx12.cpp"
	"graphics/camera_3d.cpp"
	"input/buffer.cpp"
	"input/camera_control_first_person.cpp"
	"input/camera_control_third_person.cpp"
	"scene/scene.cpp"
	"main.cpp"
	"Platform/Windows/COM.cpp"
)


# Build HLSL shaders

set(VertexShaderFiles
	"${PROJECT_SOURCE_DIR}/scenes/hello/shaders/hello_vs.hlsl"
	"graphics/dx11/shaders/vs_basic_pcn.hlsl"
)
set_source_files_properties(${VertexShaderFiles} PROPERTIES ShaderType "vs")

set(PixelShaderFiles
	"${PROJECT_SOURCE_DIR}/scenes/hello/shaders/hello_ps.hlsl"
	"graphics/dx11/shaders/ps_basic_pcn.hlsl"
)
set_source_files_properties(${PixelShaderFiles} PROPERTIES ShaderType "ps")

set(ShaderCompileDir "${CMAKE_CURRENT_BINARY_DIR}/shaders")
file(MAKE_DIRECTORY "${ShaderCompileDir}")
target_include_directories(Crib PRIVATE ${ShaderCompileDir})

add_custom_target(CribShaders)
add_dependencies(Crib CribShaders)

foreach(FILE ${VertexShaderFiles} ${PixelShaderFiles})
	get_filename_component(ShaderName ${FILE} NAME_WE)
	get_source_file_property(ShaderType ${FILE} ShaderType)
	set(ShaderHeaderFile "${ShaderName}_fxc.h")
	add_custom_command(TARGET CribShaders
		COMMAND fxc.exe /nologo /Emain /T${ShaderType}_4_0 /Fh "${ShaderCompileDir}/${ShaderHeaderFile}" /Vn ${ShaderName} "${FILE}"
		BYPRODUCTS "${ShaderCompileDir}/${ShaderHeaderFile}"
		DEPENDS ${FILE}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		VERBATIM
	)
endforeach(FILE)
