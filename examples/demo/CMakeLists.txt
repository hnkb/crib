
add_executable(crib-demo WIN32 "")

target_link_libraries(crib-demo Crib)

target_sources(crib-demo PRIVATE
	"hello/Hello.cpp"
	"hello/hello_scene.cpp"
	"hello3d/hello3d_assets.cpp"
	"hello3d/Hello3D.cpp"
	"hello3d/Hello3D-Overlay.cpp"
	"house/house_assets.cpp"
	"house/House.cpp"
	"menu/MenuRender.cpp"
	"menu/Menu.cpp"
	"strawman/Strawman.cpp"
	"strawman/strawman_scene.cpp"
	"tetris/Tetris.cpp"
	"tetris/tetris_scene.cpp"
	"DemoWindow.cpp"
	"main.cpp"
)
