#include <iostream>

#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Core/Core.hpp"
#include "NGE/Media/Images/Texture.hpp"

#include "NGE/Media/Images/TargaImage.hpp"
#include "pugixml.hpp"

#ifdef NGE_USE_IMAGE_PNG
#include "NGE/Media/Images/PngImage.hpp"
#endif

#ifdef NGE_USE_IMAGE_JPEG
#include "NGE/Media/Images/JpegImage.hpp"
#endif

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
	Unload();
}

bool Texture::LoadXMLSettings(const pugi::xml_node& node, const std::string& path) {
	std::string textureType(node.name());

	std::string name = node.attribute("name").as_string();
	if (name.length() == 0) {
		log_error("Texture attribute 'name' is not valid");
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
	if (type.empty()) {
		log_error("Texture attribute 'type' is empty")
	}

	pugi::xml_node formatNode = node.child("Format");
	GLuint internalFormat = 0, format = 0;
	if (!formatNode.empty() && !formatNode.attribute("internal").empty() && !formatNode.attribute("format").empty()) {
		internalFormat = GetValidFormat(formatNode.attribute("internal").as_string());
		format = GetValidFormat(formatNode.attribute("format").as_string());
	}


	if (textureType == "Texture2D") {
		return Load2DTexture(node, path, type, wrapS, wrapT, magFilter, minFilter, internalFormat, format);
	} else if (textureType == "TextureCubeMap") {
		return LoadCubemap(node, path, type, wrapS, wrapT, magFilter, minFilter, internalFormat, format);
	} else {
		log_error("Texture type: '{}' is not supported", textureType);
		return false;
	}

	return true;
}

bool Texture::Load2DTexture(const pugi::xml_node& node, const std::string& path, const std::string& type, GLuint wrapS,
		GLuint wrapT, GLuint magFilter, GLuint minFilter, GLuint internalFormat, GLuint format) {
	std::string file = node.attribute("file").as_string();
	if (file.length() == 0) {
		log_error("Texture2D attribute 'file' is not valid");
		return false;
	}

	Image* image = CreateImagePointer(type);
	if (image == nullptr) {
		return false;
	}

	if (!image->load(path + file, true)) {
		log_error("Could not load image from: '{}{}'", path, file);
		delete image;
		return false;
	}

	bool mipmap = node.attribute("mipmap").empty() ? false : node.attribute("mipmap").as_bool();

	bool loadResult = Create2DTexture(image, wrapS, wrapT, magFilter, minFilter, internalFormat, format, mipmap);

	// Cleanup image pointers.
	DeleteImagePointer(image);

	return loadResult;
}

bool Texture::LoadCubemap(const pugi::xml_node& node, const std::string& path, const std::string& type, GLuint wrapS,
		GLuint wrapT, GLuint magFilter, GLuint minFilter, GLuint internalFormat, GLuint format) {
	Image* positiveX, *negativeX, *positiveY, *negativeY, *positiveZ, *negativeZ;

	positiveX = CreateImagePointer(type);
	negativeX = CreateImagePointer(type);
	positiveY = CreateImagePointer(type);
	negativeY = CreateImagePointer(type);
	positiveZ = CreateImagePointer(type);
	negativeZ = CreateImagePointer(type);

	bool result = true;
	for (pugi::xml_node texture = node.child("Texture"); texture; texture = texture.next_sibling("Texture")) {
		std::string file = texture.attribute("file").as_string();
		std::string target = texture.attribute("target").as_string();

		if (file.length() == 0) {
			log_error("Texture attribute file is not valid");
			result = false;
			break;
		}

		if (target == "positive_x") {
			if (!positiveX->load(path + file, true)) {
				result = false;
				break;
			}
		} else if (target == "positive_y") {
			if (!positiveY->load(path + file, true)) {
				result = false;
				break;
			}
		} else if (target == "positive_z") {
			if (!positiveZ->load(path + file, true)) {
				result = false;
				break;
			}
		} else if (target == "negative_x") {
			if (!negativeX->load(path + file, true)) {
				result = false;
				break;
			}
		} else if (target == "negative_y") {
			if (!negativeY->load(path + file, true)) {
				result = false;
				break;
			}
		} else if (target == "negative_z") {
			if (!negativeZ->load(path + file, true)) {
				result = false;
				break;
			}
		} else {
			result = false;
			break;
		}
	}

	if (!result) {
		DeleteImagePointer(positiveX, positiveY, positiveZ, negativeX, negativeY, negativeZ);
		return false;
	}

	bool loadResult = CreateCubemap(positiveX, negativeX, positiveY, negativeY, positiveZ, negativeZ, wrapS, wrapT, magFilter, minFilter, internalFormat, format);

	// Cleanup image pointers.
	DeleteImagePointer(positiveX, positiveY, positiveZ, negativeX, negativeY, negativeZ);

	return loadResult;
}

bool Texture::Create2DTexture(Image* image, GLuint clampS, GLuint clampT, GLuint magFilter, GLuint minFilter, GLuint internalFormat, GLuint format, bool mipmap) {
	Unload();
	this->target = GL_TEXTURE_2D;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetValidMagFilter(magFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetValidMinFilter(minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetValidWrapMode(clampS));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetValidWrapMode(clampT));

	height = image->getHeight();
	width = image->getWidth();

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
		gluBuild2DMipmaps(target, internalFormat, image->getWidth(), image->getHeight(), format, GL_UNSIGNED_BYTE, image->getImageData());
#endif
	} else
		glTexImage2D(target, 0, internalFormat, image->getWidth(), image->getHeight(), 0, format, GL_UNSIGNED_BYTE, image->getImageData());

	return true;
}

bool Texture::CreateCubemap(Image* positiveX, Image* negativeX, Image* positiveY, Image* negativeY, Image* positiveZ, Image* negativeZ,
		GLuint clampS, GLuint clampT, GLuint magFilter, GLuint minFilter, GLuint internalFormat, GLuint format) {
	Unload();
	this->target = GL_TEXTURE_CUBE_MAP;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	internalFormat = GetValidFormat(internalFormat);
	format = GetValidFormat(format);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, positiveX->getWidth(), positiveX->getHeight(), 0, format, GL_UNSIGNED_BYTE, positiveX->getImageData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, negativeX->getWidth(), negativeX->getHeight(), 0, format, GL_UNSIGNED_BYTE, negativeX->getImageData());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, positiveY->getWidth(), positiveY->getHeight(), 0, format, GL_UNSIGNED_BYTE, positiveY->getImageData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, negativeY->getWidth(), negativeY->getHeight(), 0, format, GL_UNSIGNED_BYTE, negativeY->getImageData());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, positiveZ->getWidth(), positiveZ->getHeight(), 0, format, GL_UNSIGNED_BYTE, positiveZ->getImageData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, negativeZ->getWidth(), negativeZ->getHeight(), 0, format, GL_UNSIGNED_BYTE, negativeZ->getImageData());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GetValidMinFilter(minFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GetValidMagFilter(magFilter));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GetValidWrapMode(clampS));
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GetValidWrapMode(clampT));

	sizeInBytes = positiveX->getWidth() * positiveX->getHeight() +
			negativeX->getWidth() * negativeY->getHeight() +
			positiveY->getWidth() * positiveY->getHeight() +
			negativeY->getWidth() * negativeY->getHeight() +
			positiveZ->getWidth() * negativeZ->getHeight() +
			negativeZ->getWidth() * negativeZ->getHeight();

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
		log_error("Invalid textureID, Texture::SetID(GLuint textureID)");
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

void Texture::Unload() {
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
			log_error("'{}' is not a valid cube map type", type);
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
		log_error("'{}' is not a valid cube map type", type);
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
			log_error("'{}' is not valid internal format", type);
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
		log_error("'{}' is not valid internal format", type);
		return -1;
	}
}

int Texture::GetTextureType(const std::string& textureType) {
	int typeId = -1;
	if (textureType == "tga" || textureType == "TGA") {
		typeId = 0;
	} else if (textureType == "png" || textureType == "PNG") {
		typeId = 1;
	} else if (textureType == "jpeg" || textureType == "JPEG") {
		typeId = 2;
	}
	return typeId;
}

Image* Texture::CreateImagePointer(const std::string& textureType) {
	int typeId = GetTextureType(textureType);

	Image* image = nullptr;
	switch (typeId) {
		case 0:
			image = new TargaImage();
			break;
#ifdef NGE_USE_IMAGE_PNG
		case 1:
			image = new PngImage();
			break;
#endif
#ifdef NGE_USE_IMAGE_JPEG
		case 2:
			image = new JpegImage();
			break;
#endif
		default:
			log_error("Type '{}' is not supported", textureType);
	}

	return image;
}