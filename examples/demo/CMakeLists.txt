
add_executable(crib-demo WIN32 "")

target_link_libraries(crib-demo Crib)

target_sources(crib-demo PRIVATE
	#"hello/Hello.cpp"
	#"hello3d/Hello3D.cpp"
	#"hello3d/Hello3D-Overlay.cpp"
	"house/House.cpp"
	"menu/MenuRender.cpp"
	"menu/Menu.cpp"
	"strawman/Strawman.cpp"
	#"tetris/Tetris.cpp"
	"DemoWindow.cpp"
	"main.cpp"
)
