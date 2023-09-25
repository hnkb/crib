
#pragma once

#include <filesystem>
#include <ttf2mesh.h>


namespace crib
{
	class Font
	{
	public:
		Font(const std::filesystem::path& filename);
		~Font() { ttf_free(font); }

		ttf_t* operator->() { return font; }

		struct Glyph
		{
			~Glyph();
			ttf_glyph_t* glyph = nullptr;
			ttf_mesh_t* mesh = nullptr;
		};

		Glyph getGlyph(wchar_t symbol);
		Glyph getGlyph(int index);

	private:
		ttf_t* font = nullptr;
	};

}
