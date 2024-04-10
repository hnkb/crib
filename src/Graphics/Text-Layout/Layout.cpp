
#include <crib/Graphics>


namespace crib::Graphics
{
	namespace
	{
		std::map<Scene::Entity*, std::string> cached;
	}

	void updateText(Scene::Entity& div, const ModelPack& font)
	{
		auto& cachedText = cached[&div];

		if (cachedText == div.text)
			return;

		div.children.clear();
		cachedText = div.text;


		auto textPos = div.pos;

		for (auto symbol: div.text)
		{
			if (symbol == ' ')
				textPos.x += .2f;
			else if (symbol == '\n' || symbol == '\r')
			{
				textPos.x = div.pos.x;
				textPos.y -= 1.1f;
			}
			else
			{
				auto modelId = font.meshes.find(symbol) == font.meshes.end() ? 175 : symbol;

				auto& e = div.children.emplace_back();
				e.model = &font.meshes.at(modelId);
				e.pos = textPos;
				textPos.x += e.model->font.advance;
			}
		}
	}

}
