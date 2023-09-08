
#include <crib/App>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string>


namespace crib::Platform::X11
{

	namespace App
	{
		void open();
		void close();

		extern Display* display;
		extern Atom windowClosed;
		extern XContext windowClass;
	}

	class Window
	{
	public:
		Window(const crib::App::Window::Options&);
		~Window();

		void proc(XEvent& event);
		void close();
		void setTitle(const std::string& title);

	private:
		int screen;
		::Window wnd;
		::GC gc;

		bool alreadyDeleted = false;
	};

}
