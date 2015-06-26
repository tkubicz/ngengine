#include "NGE/Media/Images/Texture.hpp"
#include "NGE/Media/Images/TargaImage.hpp"
#include "NGE/Tools/Logger.hpp"
#include "NGE/Core/Core.hpp"
#include <iostream>

using namespace NGE::Media::Images;

Texture::Texture(GLuint target) {
	currentUnit = -1;
	this->target = target;
	height = width = id = sizeInBytes = 0;
}

Texture::Texture(const Texture& copy) {
	this->operator=(copy);
}

Texture& Texture::operator=(const Texture& copy) {
	if (this != &copy) {
		SetID(copy.id);

		currentUnit = copy.currentUnit;
		target = copy.target;
		height = copy.height;
		width = copy.width;
	}

	return* this;
}

Texture::~Texture() {
	Destroy();
}

bool Texture::LoadXMLSettings(const pugi::xml_node& node, const std::string& path) {
	std::string textureType(node.name());

	std::string name = node.attribute("name").as_string();
	if (name.length() == 0) {
		Tools::Logger::WriteErrorLog("Texture2D attribute name is not valid");
		return false;
	}

	pugi::xml_node wrapNode = node.child("Wrap");
	GLuint wrapS = 0, wrapT = 0;
	if (!wrapNode.empty() && !wrapNode.attribute("s").empty() && !wrapNode.attribute("t").empty()) {
		wrapS = GetValidWrapMode(wrapNode.attribute("s").as_string());
		wrapT = GetValidWrapMode(wrapNode.attribute("t").as_string());
	}

	pugi::xml_node filterNode = node.child("Filter");
	GLuint magFilter = 0, minFilter = 0;
	if (!filterNode.empty() && !filterNode.attribute("min").empty() && !filterNode.attribute("mag").empty()) {
		magFilter = GetValidMagFilter(filterNode.attribute("mag").as_string());
		minFilter = GetValidMinFilter(filterNode.attribute("min").as_string());
	}

	std::string type = node.attribute("type").as_string();

	pugi::xml_node formatNode = node.child("Format");
	GLuint internalFormat = 0, format = 0;
	if (!formatNode.empty() && !formatNode.attribute("internal").empty() && !formatNode.attribute("format").empty()) {
		internalFormat = GetValidFormat(formatNode.attribute("internal").as_string());
		format = GetValidFormat(formatNode.attribute("format").as_string());
	}

	if (textureType == "Texture2D") {
		std::string file = node.attribute("file").as_string();
		if (file.length() == 0) {
			Tools::Logger::WriteErrorLog("Texture2D attribute file is not valid");
			return false;
		}

		TargaImage image;
		if (type != "tga") {
			Tools::Logger::WriteErrorLog("Currently only targa (type=\"tga\") image files are supported");
			return false;
		} else {
			if (!image.Load(path + file, true))
				return false;
		}

		bool mipmap = node.attribute("mipmap").empty() ? false : node.attribute("mipmap").as_bool();

		return Load2DImage(image, wrapS, wrapT, magFilter, minFilter, internalFormat, format, mipmap);
	} else if (textureType == "TextureCubeMap") {
		if (type != "tga") {
			Tools::Logger::WriteErrorLog("Currently only targa (type=\"tga\") image files are supported");
			return false;
		} else {
			TargaImage positiveX, negativeX, positiveY, negativeY, positiveZ, negativeZ;

			for (pugi::xml_node texture = node.child("Texture"); texture; texture = texture.next_sibling("Texture")) {
				std::string file = texture.attribute("file").as_string();
				std::string target = texture.attribute("target").as_string();

				if (file.length() == 0) {
					Tools::Logger::WriteErrorLog("Texture attribute file is not valid");
					return false;
				}

				if (target == "positive_x") {
					if (!positiveX.Load(path + file, true))
						return false;
				} else if (target == "positive_y") {
					if (!positiveY.Load(path + file, true))
						return false;
				} else if (target == "positive_z") {
					if (!positiveZ.Load(path + file, true))
						return false;
				} else if (target == "negative_x") {
					if (!negativeX.Load(path + file, true))
						return false;
				} else if (target == "negative_y") {
					if (!negativeY.Load(path + file, true))
						return false;
				} else if (target == "negative_z") {
					if (!negativeZ.Load(path + file, true))
						return false;
				} else
					return false;
			}

			return LoadCubemap(positiveX, negativeX, positiveY, negativeY, positiveZ, negativeZ, wrapS, wrapT, magFilter, minFilter, internalFormat, format);
		}
	} else {
		Tools::Logger::WriteErrorLog("Texture --> \"" + textureType + "\" is not supported type");
		return false;
	}

	return true;
}

bool Texture::Load2DImage(const Image& image, GLuint clampS, GLuint clampT, GLuint magFilter, GLuint minFilter, GLuint internalFormat, GLuint format, bool mipmap) {
	Destroy();
	this->target = GL_TEXTURE_2D;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetValidMagFilter(magFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetValidMinFilter(minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetValidWrapMode(clampS));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetValidWrapMode(clampT));

	height = image.GetHeight();
	width = image.GetWidth();

	internalFormat = GetValidFormat(internalFormat);
	format = GetValidFormat(format);

	sizeInBytes = height * width;

	if (format == GL_RGB)
		sizeInBytes *= 3;
	else if (format == GL_RGBA)
		sizeInBytes *= 4;

	if (mipmap) {
		// TODO: Fix that on android!
#ifndef ANDROID
		gluBuild2DMipmaps(target, internalFormat, image.GetWidth(), image.GetHeight(), format, GL_UNSIGNED_BYTE, image.GetImageData());
#endif
	} else
		glTexImage2D(target, 0, internalFormat, image.GetWidth(), image.GetHeight(), 0, format, GL_UNSIGNED_BYTE, image.GetImageData());

	return true;
}

bool Texture::LoadCubemap(const Image& positiveX, const Image& negativeX, const Image& positiveY, const Image& negativeY, const Image& positiveZ, const Image& negativeZ,
		GLuint clampS, GLuint clampT, GLuint magFilter, GLuint minFilter, GLuint internalFormat, GLuint format) {
	Destroy();
	this->target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	internalFormat = GetValidFormat(internalFormat);
	format = GetValidFormat(format);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, positiveX.GetWidth(), positiveX.GetHeight(), 0, format, GL_UNSIGNED_BYTE, positiveX.GetImageData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, negativeX.GetWidth(), negativeX.GetHeight(), 0, format, GL_UNSIGNED_BYTE, negativeX.GetImageData());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, positiveY.GetWidth(), positiveY.GetHeight(), 0, format, GL_UNSIGNED_BYTE, positiveY.GetImageData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, negativeY.GetWidth(), negativeY.GetHeight(), 0, format, GL_UNSIGNED_BYTE, negativeY.GetImageData());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, positiveZ.GetWidth(), positiveZ.GetHeight(), 0, format, GL_UNSIGNED_BYTE, positiveZ.GetImageData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, negativeZ.GetWidth(), negativeZ.GetHeight(), 0, format, GL_UNSIGNED_BYTE, negativeZ.GetImageData());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GetValidMinFilter(minFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GetValidMagFilter(magFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GetValidWrapMode(clampS));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GetValidWrapMode(clampT));

	sizeInBytes = positiveX.GetWidth() * positiveX.GetHeight() +
			negativeX.GetWidth() * negativeY.GetHeight() +
			positiveY.GetWidth() * positiveY.GetHeight() +
			negativeY.GetWidth() * negativeY.GetHeight() +
			positiveZ.GetWidth() * negativeZ.GetHeight() +
			negativeZ.GetWidth() * negativeZ.GetHeight();

	return true;
}

const bool Texture::Activate(int unit) {
	if (!id)
		return false;

	if (unit > -1)
		glActiveTexture(GL_TEXTURE0 + unit);

	glEnable(target);
	glBindTexture(target, id);

	currentUnit = unit;
	return true;
}

const bool Texture::Deactivate() {
	if (id > 0) {
		if (currentUnit > -1)
			glActiveTexture(GL_TEXTURE0 + currentUnit);

		glDisable(target);
		return true;
	}

	return false;
}

void Texture::SetID(GLuint textureID) {
	//static int unknown = 1;

	if (!textureID) {
		Tools::Logger::WriteErrorLog("Invalid textureID, Texture::SetID(GLuint textureID)");
		return;
	}

	id = textureID;
	GLint info;

	// TODO: Fix that on android!
#ifndef ANDROID
	Activate();
	glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &info);
	width = info;
	glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &info);
	height = info;
	Deactivate();
#endif
}

const GLuint Texture::GetID() const {
	return id;
}

void Texture::SetTarget(GLuint target) {
	this->target = target;
}

const GLuint Texture::GetTarget() const {
	return target;
}

const GLuint Texture::GetWidth() const {
	return width;
}

const GLuint Texture::GetHeight() const {
	return height;
}

const GLuint Texture::GetSizeInBytes() const {
	return sizeInBytes;
}

void Texture::Destroy() {
	glDeleteTextures(1, &id);

	height = 0;
	width = 0;
	id = 0;
}

int Texture::GetValidWrapMode(int clamp) {
	switch (clamp) {
			// TODO: Fix that on android!
#ifndef ANDROID
		case GL_CLAMP:
			return GL_CLAMP;
#endif
		case GL_REPEAT:
			return GL_REPEAT;
		case GL_CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
#ifndef ANDROID
		case GL_CLAMP_TO_BORDER:
			return GL_CLAMP_TO_BORDER;
#endif
		default:
			return GL_REPEAT;
	}
}

int Texture::GetValidWrapMode(const std::string& clamp) {
	if (clamp == "CLAMP") {
#ifndef ANDROID
		return GL_CLAMP;
#endif
	} else if (clamp == "REPEAT")
		return GL_REPEAT;
	else if (clamp == "CLAMP_TO_EDGE")
		return GL_CLAMP_TO_EDGE;
	else if (clamp == "CLAMP_TO_BORDER") {
#ifndef ANDROID
		return GL_CLAMP_TO_BORDER;
#endif
	} else
		return GL_REPEAT;
}

int Texture::GetValidMagFilter(int filter) {
	return (filter == GL_NEAREST) ? GL_NEAREST : GL_LINEAR;
}

int Texture::GetValidMagFilter(const std::string& filter) {
	if (filter == "NEAREST")
		return GL_NEAREST;
	else
		return GL_LINEAR;
}

int Texture::GetValidMinFilter(int filter) {
	switch (filter) {
		case GL_NEAREST:
			return GL_NEAREST;
		case GL_LINEAR_MIPMAP_LINEAR:
			return GL_LINEAR_MIPMAP_LINEAR;
		case GL_NEAREST_MIPMAP_LINEAR:
			return GL_NEAREST_MIPMAP_LINEAR;
		case GL_LINEAR_MIPMAP_NEAREST:
			return GL_LINEAR_MIPMAP_NEAREST;
		case GL_NEAREST_MIPMAP_NEAREST:
			return GL_NEAREST_MIPMAP_NEAREST;
		default:
			return GL_LINEAR;
	}
}

int Texture::GetValidMinFilter(const std::string& filter) {
	if (filter == "NEAREST")
		return GL_NEAREST;
	else if (filter == "LINEAR_MIPMAP_LINEAR")
		return GL_LINEAR_MIPMAP_LINEAR;
	else if (filter == "NEAREST_MIPMAP_LINEAR")
		return GL_NEAREST_MIPMAP_LINEAR;
	else if (filter == "LINEAR_MIPMAP_NEAREST")
		return GL_LINEAR_MIPMAP_NEAREST;
	else if (filter == "NEAREST_MIPMAP_NEAREST")
		return GL_NEAREST_MIPMAP_NEAREST;
	else
		return GL_LINEAR;
}

int Texture::GetValidCubeMap(int type) {
	switch (type) {
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		default:
			Tools::Logger::WriteErrorLog("Texture --> \"" + to_string(type) + "\" is not a valid cube map type");
			return -1;
	}
}

int Texture::GetValidCubeMap(const std::string& type) {
	if (type == "POSITIVE_X")
		return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	else if (type == "POSITIVE_Y")
		return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	else if (type == "POSITIVE_Z")
		return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	else if (type == "NEGATIVE_X")
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	else if (type == "NEGATIVE_Y")
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	else if (type == "NEGATIVE_Z")
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	else {
		Tools::Logger::WriteErrorLog("Texture --> \"" + to_string(type) + "\" is not a valid cube map type");
		return -1;
	}
}

int Texture::GetValidFormat(int type) {
	switch (type) {
		case GL_DEPTH_COMPONENT:
			return GL_DEPTH_COMPONENT;
		case GL_DEPTH_STENCIL:
			return GL_DEPTH_STENCIL;
		case GL_RED:
			return GL_RED;
		case GL_RG:
			return GL_RG;
		case GL_RGB:
			return GL_RGB;
		case GL_RGBA:
			return GL_RGBA;
		default:
			Tools::Logger::WriteErrorLog("Texture --> \"" + to_string(type) + "\" is not valid internal format");
			return -1;
	}
}

int Texture::GetValidFormat(const std::string& type) {
	if (type == "DEPTH_COMPONENT")
		return GL_DEPTH_COMPONENT;
	else if (type == "DEPTH_STENCIL")
		return GL_DEPTH_STENCIL;
	else if (type == "RED")
		return GL_RED;
	else if (type == "RG")
		return GL_RG;
	else if (type == "RGB")
		return GL_RGB;
	else if (type == "RGBA")
		return GL_RGBA;
	else {
		Tools::Logger::WriteErrorLog("Texture --> \"" + to_string(type) + "\" is not valid internal format");
		return -1;
	}
}