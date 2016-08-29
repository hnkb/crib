
#pragma once


namespace crib
{
	namespace graphics
	{

		class renderer
		{
		public:
			virtual ~renderer() {}

			virtual void render() = 0;
		};

	}
}
