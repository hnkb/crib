
#include <crib/App>
#include <crib/Graphics>
#include <iostream>
#include <cmath>

using namespace crib;


Graphics::Scene scene;
Graphics::View view { scene, {} };
auto& camera = view.camera;


class MyWindow : public App::Window
{
public:
	MyWindow() : Window(Options { "HelloTriangle", { 400, 300 }, { 800, 800 } })
	{
		camera.view.scale = .25f;
		camera.setViewport(getOptions().size);
	}
	void onSizeChanged(int2 dims) override
	{
		Window::onSizeChanged(dims);
		printf("size %d %d\n", dims.x, dims.y);
		camera.setViewport(dims);

		// TODO: update scale so that objects are in similar size as before
	}
	void onPosChanged(int2 pos) override
	{
		Window::onPosChanged(pos);
		// printf("move\n");
	}
	void onKeyChar(const std::string& str) override
	{
		if (str[0] == 27)  // Escape
		{
			if (text.empty())
				close();
			else
				text.clear();
		}
		else if (str[0] == 8)  // Backspace
		{
			// remove all UTF-8 continuation bytes
			while (text.size() && (text.back() & 0xc0) == 0x80)
				text.pop_back();
			if (text.size())
				text.pop_back();
		}
		else
		{
			text += str;
		}
		scene.entities.front().text = text + "_";
		draw();
	}
	void onMouseEvent(const App::MouseEvent& ev) override
	{
		using App::MouseEvent;

		if (ev.type == MouseEvent::Type::ButtonDown)
		{
			if (std::isnan(dragStart.x))
				dragStart = camera.pixelToWorld((float2)ev.pos);
		}
		else if (ev.type == MouseEvent::Type::ButtonUp)
		{
			dragStart = float2::nan();
			this->draw();
		}
		else if (ev.type == MouseEvent::Type::Move && !std::isnan(dragStart.x))
		{
			// our mouse pointer is now pointing at a world position, but, we want it to be
			// pointing to the same position as dragStart => so fix the offset to achieve this
			camera.view.offset -= dragStart - camera.pixelToWorld((float2)ev.pos);
			draw();
		}
		else if (ev.type == MouseEvent::Type::Wheel)
		{
			// I want the cursor position on screen to point to the same world coordinates as
			// before the scaling. So, I subtract the difference between the new world position
			// and the original one from the view offset.

			auto before = camera.pixelToWorld((float2)ev.pos);

			camera.view.scale *= powf(1.25f, ev.wheel);

			auto after = camera.pixelToWorld((float2)ev.pos);

			camera.view.offset -= before - after;

			draw();
		}
	}

private:
	float2 dragStart = float2::nan();
	std::string text;
};


#include <crib/File>
char buffer[5'000'000];


int main()
{
	try
	{
		for (auto font: { "Overpass-Regular", "Overpass-Bold" })
			scene.assets.models[font] = Graphics::loadMeshFromTTF(
				std::filesystem::current_path().parent_path().parent_path() / "assets"
				/ (std::string(font) + ".ttf"));

		{
			auto& div = scene.entities.emplace_back();
			div.type = Graphics::Scene::Entity::Type::Text;
			div.pos = { -2, 1.1 };
			div.fontFace = "Overpass-Bold";
			div.text = "_";
		}
		if (0)
		{
			auto& div = scene.entities.emplace_back();
			div.type = Graphics::Scene::Entity::Type::Text;
			div.pos = { -2, -2 };
			div.fontFace = "Overpass-Regular";

			File txt(LR"(C:\Users\hani\Downloads\(personal projects)\~~dataset\~dataset\The Claiming of Sleeping Beauty; Roquelaure.txt)", "rb");
			fread(buffer, 1, sizeof(buffer),txt);
			div.text = buffer;
		}

		{
			auto& model = scene.assets.models["shapes"];
			model.buffer.vertex.emplace_back(-1, -1);
			model.buffer.vertex.emplace_back(-1, 1);
			model.buffer.vertex.emplace_back(1, 1);
			model.buffer.vertex.emplace_back(1, -1);
			model.buffer.index = { 0, 1, 2, 0, 2, 3 };
			auto& mesh = model.meshes[0];
			mesh.startVertex = 0;
			mesh.startIndex = 0;
			mesh.indexCount = 6;

			{
				auto& square = scene.entities.emplace_back();
				square.pos = { -3.2f, .9f };
				square.model = &mesh;
			}
			{
				auto& square = scene.entities.emplace_back();
				square.pos = { -3.2f, -1.3f };
				square.model = &mesh;
			}
		}


		// App::Window win;
		MyWindow win2;
		auto win = std::move(win2);
		return App::run();
	}
	catch (std::exception& ex)
	{
		std::cout << "App finished with error:\n" << ex.what() << "\n";
	}

	return 0;
}
