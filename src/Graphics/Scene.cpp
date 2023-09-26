
#include <crib/Graphics>

using crib::Graphics::View;


namespace crib::Graphics
{
	void updateText(Scene::Entity& div, const ModelPack& font);
}


void View::update()
{
	for (auto& e: scene.entities)
		if (e.type == Scene::Entity::Type::Text)
			updateText(e, scene.assets.models.at(e.fontFace));
}
