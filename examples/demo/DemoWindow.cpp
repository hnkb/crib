
#include "DemoWindow.h"
#include "menu/Menu.h"
#include "hello/Hello.h"
#include "hello3d/Hello3D.h"
#include "house/House.h"
#include "strawman/Strawman.h"
#include "tetris/Tetris.h"

using CribDemo::Window;
using Crib::Graphics::SceneBase;


Window::Window() : Crib::Window(L"crib_demo", L"Crib Demo")
{
	createScene(settings->get(L"startup", L"menu"));
}

void Window::frame()
{
	if (scene)
	{
		auto s = scene->update(timer.nextFrame(), input.swap());
		if (s.size())
		{
			if (s == L"quit")
			{
				if (getTitle() == L"crib: menu")
					DestroyWindow(handle);
				else
					createScene(L"menu");
				return;
			}
			else if (s.compare(0, 5, L"scene") == 0)
				createScene(s.substr(6));
			else if (s == L"reset-graphics")
				createGraphicsContext();
		}
		Crib::Window::frame();
	}
}

void Window::createScene(const std::wstring& name)
{
	scene = [&](Crib::PersistentSettings& setting) {
		if (name == L"menu") return std::unique_ptr<SceneBase>(new Menu::Scene(setting));
		if (name == L"hello") return std::unique_ptr<SceneBase>(new Hello::Scene());
		if (name == L"hello3d") return std::unique_ptr<SceneBase>(new Hello3D::Scene(setting));
		if (name == L"house") return std::unique_ptr<SceneBase>(new House::Scene(setting));
		if (name == L"strawman") return std::unique_ptr<SceneBase>(new Strawman::Scene());
		if (name == L"tetris") return std::unique_ptr<SceneBase>(new Tetris::Scene());

		throw std::invalid_argument("demo scene name is invalid.");
	}(*settings);

	setTitle(L"crib: " + name);
	if (graphics) graphics->bind(scene.get());
}
