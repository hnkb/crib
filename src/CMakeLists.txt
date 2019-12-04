
add_library(Crib STATIC "")


target_include_directories(Crib PUBLIC
	"${PROJECT_SOURCE_DIR}/include"
)


target_sources(Crib PRIVATE
	"core/settings.cpp"
	"Graphics/D3D11/Context_dx11.cpp"
	"Graphics/D3D11/Drawable_dx11.cpp"
	"Graphics/D3D11/Effects_dx11.cpp"
	"Graphics/D3D12/Context_dx12.cpp"
	"Graphics/Camera3D.cpp"
	"Graphics/Vertex.cpp"
	"Input/CameraControl/FirstPerson.cpp"
	"Input/CameraControl/ThirdPerson.cpp"
	"Input/Buffer.cpp"
	"Input/Timer.cpp"
	"Platform/Windows/Application_win.cpp"
	"Platform/Windows/COM.cpp"
	"Platform/Windows/Window_win.cpp"
)


# Build HLSL shaders

set(VertexShaderFiles
	"${PROJECT_SOURCE_DIR}/examples/demo/hello/shaders/hello_vs.hlsl"
	"Graphics/Shaders/HLSL/vs_basic_pcn.hlsl"
)
set_source_files_properties(${VertexShaderFiles} PROPERTIES ShaderType "vs")

set(PixelShaderFiles
	"${PROJECT_SOURCE_DIR}/examples/demo/hello/shaders/hello_ps.hlsl"
	"Graphics/Shaders/HLSL/ps_basic_pcn.hlsl"
)
set_source_files_properties(${PixelShaderFiles} PROPERTIES ShaderType "ps")

set(ShaderCompileDir "${CMAKE_CURRENT_BINARY_DIR}/shaders")
file(MAKE_DIRECTORY "${ShaderCompileDir}")
target_include_directories(Crib PUBLIC ${ShaderCompileDir}) # this is really bad, but we're not supposed to be responsible for others' embedded resources

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
