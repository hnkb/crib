
#include "Font.h"
#include <crib/Graphics>
#include <stdexcept>

using namespace crib;
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


Graphics::ModelPack crib::Graphics::loadMeshFromTTF(const std::filesystem::path& filename)
{
	Font font(filename);

	ModelPack output;

	output.buffer.vertex.reserve(font->nglyphs * 125);
	output.buffer.index.reserve(font->nglyphs * 125);

	int numErrors = 0;
	for (int glyphIdx = 0; glyphIdx < font->nglyphs; glyphIdx++)
	{
		try
		{
			auto g = font.getGlyph(glyphIdx);

			auto& mesh = output.meshes[g.glyph->symbol];

			mesh.font.advance = g.glyph->advance;
			mesh.font.lbearing = g.glyph->lbearing;
			mesh.startVertex = output.buffer.vertex.size();
			mesh.startIndex = output.buffer.index.size();
			mesh.indexCount = g.mesh->nfaces * 3;

			for (int i = 0; i < g.mesh->nvert; i++)
				output.buffer.vertex.emplace_back(g.mesh->vert[i].x, g.mesh->vert[i].y);

			for (int i = 0; i < g.mesh->nfaces; i++)
			{
				output.buffer.index.push_back((uint16_t)g.mesh->faces[i].v1);
				output.buffer.index.push_back((uint16_t)g.mesh->faces[i].v2);
				output.buffer.index.push_back((uint16_t)g.mesh->faces[i].v3);
			}
		}
		catch (...)
		{
			numErrors++;
		}
	}

	//printf("Encountered %d errors while loading\n", numErrors);

	return output;
}
