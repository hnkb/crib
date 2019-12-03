
#include <Crib/Window.h>


class demo_window : crib::core::window
{
public:
	demo_window();

protected:
	virtual void frame() override;
	void create_scene(const std::wstring& name);
};
