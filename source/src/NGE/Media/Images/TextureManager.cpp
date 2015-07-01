#include "NGE/Media/Images/TextureManager.hpp"
#include "NGE/Media/MediaManager.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::Media::Images;

void TextureManager::deinitialize() {
	for (std::map<std::string, Texture*>::iterator i = textures.begin(); i != textures.end(); ++i) {
		(i->second)->destroy();
		delete i->second;
	}
}

bool TextureManager::addTexture(const std::string& name, Texture* texture) {
	std::map<std::string, Texture*>::iterator it = textures.find(name);
	if (it != textures.end())
		return true;
	else {
		textures.insert(std::make_pair(name, texture));
		Tools::Logger::WriteInfoLog("TextureManager --> New texture added: " + name + " (" + to_string(texture->getSizeInBytes() / 1024) + " kB)");
		return true;
	}
}

bool TextureManager::loadTexture(const pugi::xml_node& node) {
	if (node.empty() || (std::string(node.name()) != "Texture2D" && std::string(node.name()) != "TextureCubeMap")) {
		Tools::Logger::WriteErrorLog("TextureManager --> Need \"Texture2D\" or \"TextureCubeMap\" node");
		return false;
	}

	std::string name = node.attribute("name").as_string();
	if (name.length() == 0) {
		Tools::Logger::WriteErrorLog("TextureManager --> Could not find name attribute");
		return false;
	}

	std::map<std::string, Texture*>::iterator it = textures.find(name);
	if (it != textures.end())
		return true;
	else {
		Texture* texture = new Texture();
		std::vector<std::string> paths = NGE::Media::MediaManager::getInstance().getMediaPathManager().getPaths("texture");

		for (std::vector<std::string>::iterator i = paths.begin(); i != paths.end(); ++i) {
			if (texture->loadXMLSettings(node, *i)) {
				textures.insert(std::make_pair(name, texture));
				Tools::Logger::WriteInfoLog("TextureManager --> New texture added: " + name + " (" + to_string(texture->getSizeInBytes() / 1024) + " kB)");
				return true;
			}
		}

		Tools::Logger::WriteErrorLog("TextureManager --> Could not load texture: " + name);
		return false;
	}
}

Texture* TextureManager::getTexture(const std::string& name) {
	std::map<std::string, Texture*>::iterator it = textures.find(name);
	if (it != textures.end()) {
		return (it->second);
	}

	Tools::Logger::WriteErrorLog("TextureManager --> Could not find texture: " + name);
	return NULL;
}

Texture* TextureManager::getTexture(const pugi::xml_node& node) {
	if (std::string(node.name()) != "Texture2D" && std::string(node.name()) != "TextureCubeMap") {
		Tools::Logger::WriteErrorLog("TextureManager --> Need \"Texture2D\" or \"TextureCubeMap\" node");
		return NULL;
	}

	std::string name = node.attribute("name").as_string();

	if (name.length() > 0) {
		if (loadTexture(node))
			return getTexture(name);
	}

	return NULL;
}

int TextureManager::getTextureCount() {
	return textures.size();
}