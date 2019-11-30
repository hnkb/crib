
#pragma once


namespace crib
{
	namespace graphics
	{
		namespace base
		{

			class renderer
			{
			public:
				virtual ~renderer() {}

				virtual void render() = 0;
				virtual void resize(const float width, const float height) {}
			};

		}
	}
}
