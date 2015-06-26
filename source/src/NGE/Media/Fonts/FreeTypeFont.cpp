#ifdef NGE_USE_FREETYPE

#include "NGE/Media/Fonts/FreeTypeFont.hpp"
#include "NGE/Tools/Logger.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Media/MediaManager.hpp"
#include "NGE/Core/Core.hpp"
#include FT_GLYPH_H

using namespace NGE::Media::Fonts;

FreeTypeFont::FreeTypeFont(int fontSize, const std::string& fontName, NGE::Media::Shaders::GLSLProgram* shader) {
	this->fontName = fontName;
	this->fontSize = fontSize;
	this->shader = shader;
}

FreeTypeFont::~FreeTypeFont() {
	glDeleteTextures(1, &atlasTex);
}

bool FreeTypeFont::Initialize() {
	FT_Library library;
	if (FT_Init_FreeType(&library)) {
		Tools::Logger::WriteErrorLog("FreeTypeFont --> Cound not initialize the freetype library");
		return false;
	}

	FT_Face fontInfo;
	if (FT_New_Face(library, fontName.c_str(), 0, &fontInfo)) {
		Tools::Logger::WriteErrorLog("FreeTypeFont --> Could not load the specified font");
		return false;
	}

	GenerateAtlasTexture(fontInfo);

	FT_Done_Face(fontInfo);
	FT_Done_FreeType(library);

	if (!shader) {
		Tools::Logger::WriteErrorLog("FreeTypeFont --> Font shader is not set");
		return false;
	}

	return true;
}

bool FreeTypeFont::GenerateAtlasTexture(FT_Face& fontInfo) {
	FT_Set_Pixel_Sizes(fontInfo, 0, fontSize);
	FT_GlyphSlot g = fontInfo->glyph;

	int rowWidth, rowHeight;
	rowWidth = rowHeight = atlasWidth = atlasHeight = 0;

	// Znalezienie rozmiaru tekstury ktora bedzie przechowywac wszystkie widoczne znaki w ASCII
	for (int i = 32; i < 128; ++i) {
		if (FT_Load_Char(fontInfo, i, FT_LOAD_RENDER)) {
			Tools::Logger::WriteErrorLog("FreeTypeFont --> Could not load character: " + to_string(i));
			continue;
		}

		if (rowWidth + g->bitmap.width + 1 >= maxWidth) {
			atlasWidth = std::max(atlasWidth, rowWidth);
			atlasHeight += rowHeight;
			rowWidth = 0;
			rowHeight = 0;
		}

		rowWidth += g->bitmap.width + 1;
		rowHeight = std::max(rowHeight, g->bitmap.rows);
	}

	atlasWidth = std::max(atlasWidth, rowWidth);
	atlasHeight += rowHeight;

	// Utworzenie tekstury
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &atlasTex);
	glBindTexture(GL_TEXTURE_2D, atlasTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlasWidth, atlasHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int ox = 0;
	int oy = 0;
	rowHeight = 0;

	for (int i = 32; i < 128; ++i) {
		if (FT_Load_Char(fontInfo, i, FT_LOAD_RENDER)) {
			Tools::Logger::WriteErrorLog("FreeTypeFont --> Could not load character: " + to_string(i));
			continue;
		}

		if (ox + g->bitmap.width + 1 >= maxWidth) {
			oy += rowHeight;
			rowHeight = 0;
			ox = 0;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		glyphAdvances[i].first = g->advance.x >> 6;
		glyphAdvances[i].second = g->advance.y >> 6;

		glyphDimensions[i].first = g->bitmap.width;
		glyphDimensions[i].second = g->bitmap.rows;

		glyphPositions[i].first = g->bitmap_left;
		glyphPositions[i].second = g->bitmap_top;

		glyphTextureOffset[i].first = (float) ox / (float) atlasWidth;
		glyphTextureOffset[i].second = (float) oy / (float) atlasHeight;

		rowHeight = std::max(rowHeight, g->bitmap.rows);
		ox += g->bitmap.width + 1;
	}

	glGenBuffers(1, &vertexBuffer);

	Tools::Logger::WriteInfoLog("FreeTypeFont --> Generated a " + to_string(atlasWidth) + " x "
			+ to_string(atlasHeight) + " (" + to_string(atlasWidth * atlasHeight / 1024)
			+ " kb) texture atlas for " + fontName);

	return true;
}

void FreeTypeFont::GenerateVertices(GLuint& buffer, unsigned& size, const std::string& str, NGE::Math::vec2f position, const NGE::Math::vec2f& scale) {
	//Math::vec4f vertices[6 * str.size()];
	Math::vec4f* vertices = new Math::vec4f[6 * str.size()];
	int c = 0;

	for (string::size_type i = 0; i < str.size(); ++i) {
		int ch = int(str[i]);

		// Obliczenie wierzcholkow i wspolrzednych tesktury
		float x2 = position.x + glyphPositions[ch].first * scale.x;
		float y2 = -position.y - glyphPositions[ch].second * scale.y;
		float w = glyphDimensions[ch].first * scale.x;
		float h = glyphDimensions[ch].second * scale.y;

		// Przesuniecie kursora do poczatku nastepnego znaku
		position.x += glyphAdvances[ch].first * scale.x;
		position.y += glyphAdvances[ch].second * scale.y;

		// Pominiecie glifow bez pikseli
		if (!w || !h)
			continue;

		*(vertices + (c++)) = Math::vec4f(x2, -y2, glyphTextureOffset[ch].first, glyphTextureOffset[ch].second);
		*(vertices + (c++)) = Math::vec4f(x2 + w, -y2, glyphTextureOffset[ch].first + (float) glyphDimensions[ch].first / (float) atlasWidth, glyphTextureOffset[ch].second);
		*(vertices + (c++)) = Math::vec4f(x2, -y2 - h, glyphTextureOffset[ch].first, glyphTextureOffset[ch].second + (float) glyphDimensions[ch].second / (float) atlasHeight);
		*(vertices + (c++)) = Math::vec4f(x2 + w, -y2, glyphTextureOffset[ch].first + (float) glyphDimensions[ch].first / (float) atlasWidth, glyphTextureOffset[ch].second);
		*(vertices + (c++)) = Math::vec4f(x2, -y2 - h, glyphTextureOffset[ch].first, glyphTextureOffset[ch].second + (float) glyphDimensions[ch].second / (float) atlasHeight);
		*(vertices + (c++)) = Math::vec4f(x2 + w, -y2 - h, glyphTextureOffset[ch].first + (float) glyphDimensions[ch].first / (float) atlasWidth, glyphTextureOffset[ch].second + (float) glyphDimensions[ch].second / (float) atlasHeight);
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 4 * 6 * str.size(), vertices, GL_STATIC_DRAW);
	size = c;

	delete vertices;
}

void FreeTypeFont::PrintString(const std::string& str, NGE::Math::vec2f position, NGE::Math::vec2f scale, NGE::Math::vec4f color) {
	unsigned size = 0;
	GenerateVertices(vertexBuffer, size, str, position, scale);

	shader->BindShader();
	shader->SendUniform("texture0", 0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->AutoEnableVertexAttribArray();

	glBindTexture(GL_TEXTURE_2D, atlasTex);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer((GLint) 0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	shader->SendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
	shader->SendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
	shader->SendUniform("color", color);

	glDrawArrays(GL_TRIANGLES, 0, size);

	shader->AutoDisableVertexAttribArray();

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void FreeTypeFont::PrintString(const GLuint& vertexBuffer, const unsigned& size, const NGE::Math::vec4f& color) {
	shader->BindShader();
	shader->SendUniform("texture0", 0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->AutoEnableVertexAttribArray();

	glBindTexture(GL_TEXTURE_2D, atlasTex);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer((GLint) 0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	shader->SendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
	shader->SendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
	shader->SendUniform("color", color);

	glDrawArrays(GL_TRIANGLES, 0, size);

	shader->AutoDisableVertexAttribArray();

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

NGE::Math::vec2i FreeTypeFont::GetStringDimensions(const std::string& str, const NGE::Math::vec2f& scale) {
	int width = 0, height = 0;
	for (string::size_type i = 0; i < str.size(); ++i) {
		int ch = int(str[i]);
		width += glyphPositions[ch].first * (int) scale.x + glyphAdvances[ch].first * (int) scale.x;

		if (glyphDimensions[ch].second * scale.y > height)
			height = glyphDimensions[ch].second * (int) scale.y;
	}

	return Math::vec2i(width, height);
}

int FreeTypeFont::GetStringWidth(const std::string& str) {
	return GetStringDimensions(str).x;
}

int FreeTypeFont::GetStringHeight(const std::string& str) {
	return GetStringDimensions(str).y;
}

void FreeTypeFont::SetShader(NGE::Media::Shaders::GLSLProgram* shader) {
	this->shader = shader;
}

NGE::Media::Shaders::GLSLProgram* FreeTypeFont::GetShader() {
	return shader;
}

void FreeTypeFont::SetFontName(const std::string& fontName) {
	this->fontName = fontName;
}

const std::string& FreeTypeFont::GetFontName() {
	return fontName;
}

void FreeTypeFont::SetFontSize(int fontSize) {
	this->fontSize = fontSize;
}

int FreeTypeFont::GetFontSize() {
	return fontSize;
}

int FreeTypeFont::GetAtlasHeight() {
	return atlasHeight;
}

const std::map<char, std::pair<int, int> >& FreeTypeFont::GetGlyphDimensions() {
	return glyphDimensions;
}

const std::map<char, std::pair<int, int> >& FreeTypeFont::GetGlyphAdvances() {
	return glyphAdvances;
}

bool FreeTypeFont::LoadXMLSettings(const pugi::xml_node& node, const std::string& path) {
	if (std::string(node.name()) != "FreeTypeFont") {
		Tools::Logger::WriteErrorLog("FreeTypeFont --> Need a font node");
		return false;
	}

	if (path.length() == 0) {
		Tools::Logger::WriteErrorLog("Need a path");
		return false;
	}

	if (node.attribute("fileName").empty()) {
		Tools::Logger::WriteErrorLog("FreeTypeFont --> Need a fileName attribute");
		return false;
	} else
		this->fontName = path + std::string(node.attribute("fileName").as_string());

	if (!node.attribute("size").empty())
		this->fontSize = node.attribute("size").as_int();
	else
		this->fontSize = 16;

	shader = MediaManager::GetInstance().GetShaderManager().GetProgram(node.child("Shader"));

	bool autoinit = node.attribute("autoinit").empty() ? false : node.attribute("autoinit").as_bool();

	if (autoinit)
		Initialize();

	return true;
}

#endif