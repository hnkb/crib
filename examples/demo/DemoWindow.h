
#include <Crib/Window.h>


namespace CribDemo
{

	class Window : Crib::Window
	{
	public:
		Window();

	protected:
		virtual void frame() override;
		void createScene(const std::wstring& name);
	};

}
