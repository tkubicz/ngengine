#include "NGE/GUI/GUIClippedRectangle.hpp"
#include "NGE/Math/MathUtils.hpp"
#include "NGE/Rendering/Renderer.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Media/MediaManager.hpp"

using namespace NGE::GUI;

GUIClippedRectangle::GUIClippedRectangle(int clipSize) {
	SetTextureRectangle(0, 1, 1, 0);
	SetBordersColor(255, 165, 0);
	SetClipSize(clipSize);
	SetBackgroundColor(70, 70, 70, 0.8f);

	drawBackground = false;
	drawBounds = false;
	shader = NULL;
	texture = NULL;
}

GUIClippedRectangle::~GUIClippedRectangle() {
	glDeleteBuffers(1, &vertexBuffer);
}

void GUIClippedRectangle::SetClipSize(int clipSize) {
	this->clipSize = Math::MathUtils::Clamp(clipSize, 0, 100);
}

int GUIClippedRectangle::GetClipSize() {
	return clipSize;
}

void GUIClippedRectangle::SetVisibleBounds(bool visible) {
	drawBounds = visible;
}

bool GUIClippedRectangle::BoundsVisible() {
	return drawBounds;
}

void GUIClippedRectangle::SetBackgroundColor(const NGE::Math::vec4f& color) {
	SetBackgroundColor(color.x, color.y, color.z, color.w);
}

void GUIClippedRectangle::SetBackgroundColor(float r, float g, float b, float alpha) {
	backgroundColor.Set(Math::MathUtils::Clamp(r, 0.0f, 255.0f),
			Math::MathUtils::Clamp(g, 0.0f, 255.0f),
			Math::MathUtils::Clamp(b, 0.0f, 255.0f),
			Math::MathUtils::Clamp(alpha, 0.0f, 255.0f));

	backgroundColor.x /= (backgroundColor.x > 1.0) ? 255.0f : 1.0f;
	backgroundColor.y /= (backgroundColor.y > 1.0) ? 255.0f : 1.0f;
	backgroundColor.z /= (backgroundColor.z > 1.0) ? 255.0f : 1.0f;
	backgroundColor.w /= (backgroundColor.w > 1.0) ? 255.0f : 1.0f;
}

void GUIClippedRectangle::SetBordersColor(const NGE::Math::vec3f& color) {
	SetBordersColor(color.x, color.y, color.z);
}

const NGE::Math::vec3f& GUIClippedRectangle::GetBordersColor() {
	return bordersColor;
}

void GUIClippedRectangle::SetBordersColor(float r, float g, float b) {
	bordersColor.Set(Math::MathUtils::Clamp(r, 0.0f, 255.0f),
			Math::MathUtils::Clamp(g, 0.0f, 255.0f),
			Math::MathUtils::Clamp(b, 0.0f, 255.0f));
	bordersColor.x /= (bordersColor.x > 1.0) ? 255.0f : 1.0f;
	bordersColor.y /= (bordersColor.y > 1.0) ? 255.0f : 1.0f;
	bordersColor.z /= (bordersColor.z > 1.0) ? 255.0f : 1.0f;
}

void GUIClippedRectangle::ComputeClippedBounds(const NGE::Math::vec4i& windowBounds) {
	float yTexOffset = float(clipSize) / (windowBounds.w - windowBounds.y);
	float xTexOffset = float(clipSize) / (windowBounds.z - windowBounds.x);

	xTexOffset *= textureRectangle.z - textureRectangle.x;
	yTexOffset *= textureRectangle.w - textureRectangle.y;

	vertices[0].Set((float) windowBounds.x, (float) (windowBounds.y + clipSize), textureRectangle.x, textureRectangle.w - yTexOffset);
	vertices[1].Set((float) windowBounds.x, (float) windowBounds.w, textureRectangle.x, textureRectangle.y);
	vertices[2].Set((float) (windowBounds.z - clipSize), (float) windowBounds.w, textureRectangle.z - xTexOffset, textureRectangle.y);
	vertices[3].Set((float) windowBounds.z, (float) (windowBounds.w - clipSize), textureRectangle.z, textureRectangle.y + yTexOffset);
	vertices[4].Set((float) windowBounds.z, (float) windowBounds.y, textureRectangle.z, textureRectangle.w);
	vertices[5].Set((float) (windowBounds.x + clipSize), (float) windowBounds.y, textureRectangle.x + xTexOffset, textureRectangle.w);
	vertices[6].Set((float) windowBounds.x, (float) (windowBounds.y + clipSize), textureRectangle.x, textureRectangle.w - yTexOffset);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (int) * 7 * 4, &vertices[0], GL_STATIC_DRAW);
}

void GUIClippedRectangle::EnableBackgroundColor(bool enable) {
	drawBackground = enable;
}

void GUIClippedRectangle::SetTextureRectangle(float x, float y, float z, float w) {
	if (x > 1.0f || y > 1.0f || z > 1.0f || w > 1.0f) {
		if (texture->GetID()) {
			x = Math::MathUtils::Clamp(x, 0.0f, (float) texture->GetWidth());
			y = Math::MathUtils::Clamp(y, 0.0f, (float) texture->GetHeight());
			z = Math::MathUtils::Clamp(z, 0.0f, (float) texture->GetWidth());
			w = Math::MathUtils::Clamp(w, 0.0f, (float) texture->GetHeight());

			x /= texture->GetWidth();
			z /= texture->GetWidth();

			w /= texture->GetHeight();
			y /= texture->GetHeight();
		}
	}

	textureRectangle.Set(Math::MathUtils::Clamp(x, 0.0f, 1.0f),
			Math::MathUtils::Clamp(y, 0.0f, 1.0f),
			Math::MathUtils::Clamp(z, 0.0f, 1.0f),
			Math::MathUtils::Clamp(w, 0.0f, 1.0f));
}

void GUIClippedRectangle::SetTextureRectangle(const NGE::Math::vec4f& textureRectangle) {
	SetTextureRectangle(textureRectangle.x,
			textureRectangle.y,
			textureRectangle.z,
			textureRectangle.w);
}

const NGE::Math::vec4f& GUIClippedRectangle::GetTextureRectangle() {
	return textureRectangle;
}

NGE::Media::Images::Texture* GUIClippedRectangle::GetTexture() {
	return texture;
}

void GUIClippedRectangle::SetTexture(NGE::Media::Images::Texture* texture) {
	this->texture = texture;
}

bool GUIClippedRectangle::IsBackgroundColorOn() {
	return drawBackground;
}

const NGE::Math::vec4f& GUIClippedRectangle::GetBackgroundColor() {
	return backgroundColor;
}

NGE::Media::Shaders::GLSLProgram* GUIClippedRectangle::GetShader() {
	return shader;
}

void GUIClippedRectangle::SetShader(NGE::Media::Shaders::GLSLProgram* shader) {
	this->shader = shader;
}

void GUIClippedRectangle::RenderClippedBounds() {
	if (drawBackground || drawBounds) {
		if (!shader)
			return;

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		Rendering::Renderer::GetInstance().GetMatrixStack().PushMatrix();

		shader->bindShader();
		shader->sendUniform4x4("modelview_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(MODELVIEW_MATRIX));
		shader->sendUniform4x4("projection_matrix", Rendering::Renderer::GetInstance().GetMatrixStack().GetMatrix(PROJECTION_MATRIX));

		if (drawBackground && texture && texture->GetID()) {
			texture->Activate();
			shader->sendUniform("texture0", 0);
			shader->sendUniform("guiType", 1);
		} else
			shader->sendUniform("guiType", 0);

		if (drawBackground) {
			shader->sendUniform("color", backgroundColor);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glVertexAttribPointer((GLint) 0, 4, GL_FLOAT, GL_FALSE, 0, 0);

			glDrawArrays(GL_TRIANGLE_FAN, 0, 7);

			glDisableVertexAttribArray(0);
			glDisable(GL_BLEND);
		}

		if (drawBounds) {
			shader->sendUniform("color", bordersColor);
			shader->sendUniform("guiType", 0);

			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glVertexAttribPointer((GLint) 0, 4, GL_FLOAT, GL_FALSE, 0, 0);

			glDrawArrays(GL_LINE_STRIP, 0, 7);

			glDisableVertexAttribArray(0);
		}

		shader->unbindShader();
		Rendering::Renderer::GetInstance().GetMatrixStack().PopMatrix();

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}
}

bool GUIClippedRectangle::LoadXMLClippedRectangleInfo(const pugi::xml_node& node) {
	if (!node)
		return false;

	if (!node.child("Texture2D").empty()) {
		texture = Media::MediaManager::GetInstance().GetTextureManager().GetTexture(node.child("Texture2D"));
	}

	if (!node.child("TextureRectangle").empty()) {
		if (!node.child("TextureRectangle").attribute("x").empty() &&
				!node.child("TextureRectangle").attribute("y").empty() &&
				!node.child("TextureRectangle").attribute("z").empty() &&
				!node.child("TextureRectangle").attribute("w").empty()) {
			textureRectangle.Set(node.child("TextureRectangle").attribute("x").as_float(),
					node.child("TextureRectangle").attribute("y").as_float(),
					node.child("TextureRectangle").attribute("z").as_float(),
					node.child("TextureRectangle").attribute("w").as_float());
		}
	}


	if (!node.child("BordersColor").empty()) {
		if (!node.child("BordersColor").attribute("r").empty() && !node.child("BordersColor").attribute("g").empty() &&
				!node.child("BordersColor").attribute("b").empty()) {
			SetBordersColor(node.child("BordersColor").attribute("r").as_float(),
					node.child("BordersColor").attribute("g").as_float(),
					node.child("BordersColor").attribute("b").as_float());
		}
	}

	if (!node.child("BackgroundColor").empty()) {
		if (!node.child("BackgroundColor").attribute("r").empty() &&
				!node.child("BackgroundColor").attribute("g").empty() &&
				!node.child("BackgroundColor").attribute("b").empty() &&
				!node.child("BackgroundColor").attribute("a").empty()) {
			SetBackgroundColor(node.child("BackgroundColor").attribute("r").as_float(),
					node.child("BackgroundColor").attribute("g").as_float(),
					node.child("BackgroundColor").attribute("b").as_float(),
					node.child("BackgroundColor").attribute("a").as_float());
		}
	}

	if (!node.child("Shader").empty()) {
		shader = Media::MediaManager::GetInstance().GetShaderManager().getProgram(node.child("Shader"));
	}

	if (!node.attribute("drawBackground").empty())
		drawBackground = node.attribute("drawBackground").as_bool();

	if (!node.attribute("drawBounds").empty())
		drawBounds = node.attribute("drawBounds").as_bool();

	if (!node.attribute("clipSize").empty())
		clipSize = node.attribute("clipSize").as_int();

	SetTextureRectangle(textureRectangle);
	SetClipSize(clipSize);
	EnableBackgroundColor(drawBackground);
	SetVisibleBounds(drawBounds);

	return true;
}
