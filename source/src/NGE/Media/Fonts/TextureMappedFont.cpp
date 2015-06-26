#include "NGE/Media/Fonts/TextureMappedFont.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::Media::Fonts;

TextureMappedFont::TextureMappedFont(float fontSize, NGE::Media::Images::Texture* texture, NGE::Media::Shaders::GLSLProgram* shader) {
	this->fontSize = fontSize;
	this->texture = texture;
	this->shader = shader;
	defaultColor = Math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
}

bool TextureMappedFont::Initialize() {
	if (texture == NULL) {
		Tools::Logger::WriteErrorLog("Font texture is not initialized");
		return false;
	}

	float vertices[] = {
		0.0f, 0.0f,
		fontSize, 0.0f,
		fontSize, fontSize,
		0.0f, fontSize
	};

	float texCoords[] = {
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 8, &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 8, &texCoords[0], GL_STATIC_DRAW);

	return true;
}

void TextureMappedFont::PrintString(const std::string& str, const NGE::Math::vec2f& position) {
	PrintString(str, position.x, position.y, defaultColor);
}

void TextureMappedFont::PrintString(const std::string& str, float x, float y) {
	PrintString(str, x, y, defaultColor);
}

void TextureMappedFont::PrintString(const std::string& str, const NGE::Math::vec2f& position, const NGE::Math::vec4f& color) {
	PrintString(str, position.x, position.y, color);
}

void TextureMappedFont::PrintString(const std::string& str, float x, float y, const NGE::Math::vec4f& color) {
	shader->BindShader();
	float texCoords[8];

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	texture->Activate();
	shader->AutoEnableVertexAttribArray();

	Rendering::Renderer::GetInstance().GetMatrixStack().PushMatrix();
	Rendering::Renderer::GetInstance().GetMatrixStack().Identity();
	Rendering::Renderer::GetInstance().GetMatrixStack().Translate(x, y, 0.0f);
	for (string::size_type i = 0; i < str.size(); ++i) {
		const float oneOver = 1.0f / 16.0f;
		int ch = int(str[i]);
		float xPos = float(ch % 16) * oneOver;
		float yPos = float(ch / 16) * oneOver;

		texCoords[0] = xPos;
		texCoords[1] = 1.0f - yPos - oneOver;

		texCoords[2] = xPos + oneOver;
		texCoords[3] = 1.0f - yPos - oneOver;

		texCoords[4] = xPos + oneOver;
		texCoords[5] = 1.0f - yPos - 0.001f;

		texCoords[6] = xPos;
		texCoords[7] = 1.0f - yPos - 0.001f;

		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof (float) * 8, &texCoords[0]);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer((GLint) 0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glVertexAttribPointer((GLint) 1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		shader->SendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
		shader->SendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));
		shader->SendUniform("texture0", 0);
		shader->SendUniform("color", color);

		// TODO: Fix that!
#ifndef ANDROID
		glDrawArrays(GL_QUADS, 0, 4);
#endif

		Rendering::Renderer::GetInstance().GetMatrixStack().Translate(fontSize * 0.80f, 0, 0);
	}
	Rendering::Renderer::GetInstance().GetMatrixStack().PopMatrix();

	shader->AutoDisableVertexAttribArray();

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void TextureMappedFont::SetShader(NGE::Media::Shaders::GLSLProgram* shader) {
	this->shader = shader;
}

void TextureMappedFont::SetTexture(NGE::Media::Images::Texture* texture) {
	this->texture = texture;
}