
#include "Font.h"
#include <stdexcept>

using crib::Font;

Font::Font(const std::filesystem::path& filename)
{
	ttf_load_from_file(filename.u8string().c_str(), &font, false);
	if (!font)
		throw std::runtime_error("Unable to load font");
}

Font::Glyph::~Glyph()
{
	if (mesh)
		ttf_free_mesh(mesh);
}

Font::Glyph Font::getGlyph(wchar_t symbol)
{
	auto index = ttf_find_glyph(font, symbol);
	if (index < 0)
		throw std::invalid_argument("glyph not found in the font");
	return getGlyph(index);
}

Font::Glyph Font::getGlyph(int index)
{
	Glyph ret;
	ret.glyph = &font->glyphs[index];

	if (ttf_glyph2mesh(&font->glyphs[index], &ret.mesh, TTF_QUALITY_HIGH, TTF_FEATURES_DFLT)
		!= TTF_DONE)
		throw std::runtime_error("unable to load glyph mesh");

	return ret;
}
