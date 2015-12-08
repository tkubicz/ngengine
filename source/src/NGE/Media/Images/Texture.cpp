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
		setID(copy.id);

		currentUnit = copy.currentUnit;
		target = copy.target;
		height = copy.height;
		width = copy.width;
	}

	return* this;
}

Texture::~Texture() {
	destroy();
}

bool Texture::loadXMLSettings(const pugi::xml_node& node, const std::string& path) {
	std::string textureType(node.name());

	std::string name = node.attribute("name").as_string();
	if (name.length() == 0) {
		nge_log_error("Texture2D attribute name is not valid");
		return false;
	}

	pugi::xml_node wrapNode = node.child("Wrap");
	GLuint wrapS = 0, wrapT = 0;
	if (!wrapNode.empty() && !wrapNode.attribute("s").empty() && !wrapNode.attribute("t").empty()) {
		wrapS = getValidWrapMode(wrapNode.attribute("s").as_string());
		wrapT = getValidWrapMode(wrapNode.attribute("t").as_string());
	}

	pugi::xml_node filterNode = node.child("Filter");
	GLuint magFilter = 0, minFilter = 0;
	if (!filterNode.empty() && !filterNode.attribute("min").empty() && !filterNode.attribute("mag").empty()) {
		magFilter = getValidMagFilter(filterNode.attribute("mag").as_string());
		minFilter = getValidMinFilter(filterNode.attribute("min").as_string());
	}

	std::string type = node.attribute("type").as_string();

	pugi::xml_node formatNode = node.child("Format");
	GLuint internalFormat = 0, format = 0;
	if (!formatNode.empty() && !formatNode.attribute("internal").empty() && !formatNode.attribute("format").empty()) {
		internalFormat = getValidFormat(formatNode.attribute("internal").as_string());
		format = getValidFormat(formatNode.attribute("format").as_string());
	}

	if (textureType == "Texture2D") {
		std::string file = node.attribute("file").as_string();
		if (file.length() == 0) {
			nge_log_error("Texture2D attribute file is not valid");
			return false;
		}

		TargaImage image;
		if (type != "tga") {
			nge_log_error("Currently only targa (type=\"tga\") image files are supported");
			return false;
		} else {
			if (!image.load(path + file, true))
				return false;
		}

		bool mipmap = node.attribute("mipmap").empty() ? false : node.attribute("mipmap").as_bool();

		return load2DImage(image, wrapS, wrapT, magFilter, minFilter, internalFormat, format, mipmap);
	} else if (textureType == "TextureCubeMap") {
		if (type != "tga") {
			nge_log_error("Currently only targa (type=\"tga\") image files are supported");
			return false;
		} else {
			TargaImage positiveX, negativeX, positiveY, negativeY, positiveZ, negativeZ;

			for (pugi::xml_node texture = node.child("Texture"); texture; texture = texture.next_sibling("Texture")) {
				std::string file = texture.attribute("file").as_string();
				std::string target = texture.attribute("target").as_string();

				if (file.length() == 0) {
					nge_log_error("Texture attribute file is not valid");
					return false;
				}

				if (target == "positive_x") {
					if (!positiveX.load(path + file, true))
						return false;
				} else if (target == "positive_y") {
					if (!positiveY.load(path + file, true))
						return false;
				} else if (target == "positive_z") {
					if (!positiveZ.load(path + file, true))
						return false;
				} else if (target == "negative_x") {
					if (!negativeX.load(path + file, true))
						return false;
				} else if (target == "negative_y") {
					if (!negativeY.load(path + file, true))
						return false;
				} else if (target == "negative_z") {
					if (!negativeZ.load(path + file, true))
						return false;
				} else
					return false;
			}

			return loadCubemap(positiveX, negativeX, positiveY, negativeY, positiveZ, negativeZ, wrapS, wrapT, magFilter, minFilter, internalFormat, format);
		}
	} else {
		nge_log_error("Texture --> \"" + textureType + "\" is not supported type");
		return false;
	}

	return true;
}

bool Texture::load2DImage(const Image& image, GLuint clampS, GLuint clampT, GLuint magFilter, GLuint minFilter, GLuint internalFormat, GLuint format, bool mipmap) {
	destroy();
	this->target = GL_TEXTURE_2D;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getValidMagFilter(magFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getValidMinFilter(minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getValidWrapMode(clampS));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getValidWrapMode(clampT));

	height = image.getHeight();
	width = image.getWidth();

	internalFormat = getValidFormat(internalFormat);
	format = getValidFormat(format);

	sizeInBytes = height * width;

	if (format == GL_RGB)
		sizeInBytes *= 3;
	else if (format == GL_RGBA)
		sizeInBytes *= 4;

	if (mipmap) {
		// TODO: Fix that on android!
#ifndef ANDROID
		gluBuild2DMipmaps(target, internalFormat, image.getWidth(), image.getHeight(), format, GL_UNSIGNED_BYTE, image.getImageData());
#endif
	} else
		glTexImage2D(target, 0, internalFormat, image.getWidth(), image.getHeight(), 0, format, GL_UNSIGNED_BYTE, image.getImageData());

	return true;
}

bool Texture::loadCubemap(const Image& positiveX, const Image& negativeX, const Image& positiveY, const Image& negativeY, const Image& positiveZ, const Image& negativeZ,
		GLuint clampS, GLuint clampT, GLuint magFilter, GLuint minFilter, GLuint internalFormat, GLuint format) {
	destroy();
	this->target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	internalFormat = getValidFormat(internalFormat);
	format = getValidFormat(format);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, positiveX.getWidth(), positiveX.getHeight(), 0, format, GL_UNSIGNED_BYTE, positiveX.getImageData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, negativeX.getWidth(), negativeX.getHeight(), 0, format, GL_UNSIGNED_BYTE, negativeX.getImageData());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, positiveY.getWidth(), positiveY.getHeight(), 0, format, GL_UNSIGNED_BYTE, positiveY.getImageData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, negativeY.getWidth(), negativeY.getHeight(), 0, format, GL_UNSIGNED_BYTE, negativeY.getImageData());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, positiveZ.getWidth(), positiveZ.getHeight(), 0, format, GL_UNSIGNED_BYTE, positiveZ.getImageData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, negativeZ.getWidth(), negativeZ.getHeight(), 0, format, GL_UNSIGNED_BYTE, negativeZ.getImageData());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, getValidMinFilter(minFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, getValidMagFilter(magFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, getValidWrapMode(clampS));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, getValidWrapMode(clampT));

	sizeInBytes = positiveX.getWidth() * positiveX.getHeight() +
			negativeX.getWidth() * negativeY.getHeight() +
			positiveY.getWidth() * positiveY.getHeight() +
			negativeY.getWidth() * negativeY.getHeight() +
			positiveZ.getWidth() * negativeZ.getHeight() +
			negativeZ.getWidth() * negativeZ.getHeight();

	return true;
}

const bool Texture::activate(int unit) {
	if (!id)
		return false;

	if (unit > -1)
		glActiveTexture(GL_TEXTURE0 + unit);

	glEnable(target);
	glBindTexture(target, id);

	currentUnit = unit;
	return true;
}

const bool Texture::deactivate() {
	if (id > 0) {
		if (currentUnit > -1)
			glActiveTexture(GL_TEXTURE0 + currentUnit);

		glDisable(target);
		return true;
	}

	return false;
}

void Texture::setID(GLuint textureID) {
	//static int unknown = 1;

	if (!textureID) {
		nge_log_error("Invalid textureID, Texture::SetID(GLuint textureID)");
		return;
	}

	id = textureID;
	GLint info;

	// TODO: Fix that on android!
#ifndef ANDROID
	activate();
	glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &info);
	width = info;
	glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &info);
	height = info;
	deactivate();
#endif
}

const GLuint Texture::getID() const {
	return id;
}

void Texture::setTarget(GLuint target) {
	this->target = target;
}

const GLuint Texture::getTarget() const {
	return target;
}

const GLuint Texture::getWidth() const {
	return width;
}

const GLuint Texture::getHeight() const {
	return height;
}

const GLuint Texture::getSizeInBytes() const {
	return sizeInBytes;
}

void Texture::destroy() {
	glDeleteTextures(1, &id);

	height = 0;
	width = 0;
	id = 0;
}

int Texture::getValidWrapMode(int clamp) {
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

int Texture::getValidWrapMode(const std::string& clamp) {
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

int Texture::getValidMagFilter(int filter) {
	return (filter == GL_NEAREST) ? GL_NEAREST : GL_LINEAR;
}

int Texture::getValidMagFilter(const std::string& filter) {
	if (filter == "NEAREST")
		return GL_NEAREST;
	else
		return GL_LINEAR;
}

int Texture::getValidMinFilter(int filter) {
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

int Texture::getValidMinFilter(const std::string& filter) {
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

int Texture::getValidCubeMap(int type) {
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
			nge_log_error("Texture --> \"" + nge_to_string(type) + "\" is not a valid cube map type");
			return -1;
	}
}

int Texture::getValidCubeMap(const std::string& type) {
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
		nge_log_error("Texture --> \"" + nge_to_string(type) + "\" is not a valid cube map type");
		return -1;
	}
}

int Texture::getValidFormat(int type) {
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
			nge_log_error("Texture --> \"" + nge_to_string(type) + "\" is not valid internal format");
			return -1;
	}
}

int Texture::getValidFormat(const std::string& type) {
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
		nge_log_error("Texture --> \"" + nge_to_string(type) + "\" is not valid internal format");
		return -1;
	}
}