#include "NGE/Media/Images/TextureManager.hpp"
#include "NGE/Media/Images/LoadTextureEvent.hpp"
#include "NGE/Media/Images/GetTextureEvent.hpp"
#include "NGE/Media/MediaManager.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Events/EventManager.hpp"
#include "NGE/Core/Delegate.hpp"
#include "pugixml.hpp"

using namespace NGE::Media::Images;

void TextureManager::Initialise() {
	NGE::Events::EventManager& eventManager = NGE::Events::EventManager::GetInstance();

	eventDelegates.insert(EventDelegatePair(LoadTextureEvent::eventType, NGE::Events::EventDelegate("load-texture", NGE::Core::make_delegate(GetInstance(), &TextureManager::LoadTextureListener))));
	eventDelegates.insert(EventDelegatePair(GetTextureEvent::eventType, NGE::Events::EventDelegate("get-texture", NGE::Core::make_delegate(GetInstance(), &TextureManager::GetTextureListener))));

	for (auto& delegate : eventDelegates) {
		eventManager.AddListener(delegate.second, delegate.first);
	}
}

void TextureManager::Deinitialise() {
	// Remove all textures from memory.
	for (std::map<std::string, Texture*>::iterator i = textures.begin(); i != textures.end(); ++i) {
		(i->second)->destroy();
		delete i->second;
	}

	// Remove event listeners.
	NGE::Events::EventManager& eventManager = NGE::Events::EventManager::GetInstance();
	for (auto& delegate : eventDelegates) {
		eventManager.RemoveListener(delegate.second, delegate.first);
	}
}

bool TextureManager::AddTexture(const std::string& name, Texture* texture) {
	std::map<std::string, Texture*>::iterator it = textures.find(name);
	if (it != textures.end())
		return true;
	else {
		textures.insert(std::make_pair(name, texture));
		log_info("New texture added: '{} ({} kB)'", name, texture->getSizeInBytes() / 1024);
		return true;
	}
}

bool TextureManager::LoadTexture(const pugi::xml_node& node) {
	if (node.empty() || (std::string(node.name()) != "Texture2D" && std::string(node.name()) != "TextureCubeMap")) {
		log_error("Need \"Texture2D\" or \"TextureCubeMap\" node");
		return false;
	}

	std::string name = node.attribute("name").as_string();
	if (name.length() == 0) {
		log_error("Could not find name attribute");
		return false;
	}

	auto it = textures.find(name);
	if (it != textures.end())
		return true;
	else {
		Texture* texture = new Texture();
		std::vector<std::string> paths = NGE::Media::MediaManager::GetInstance().getMediaPathManager().getPaths("texture");

		for (std::vector<std::string>::iterator i = paths.begin(); i != paths.end(); ++i) {
			if (texture->loadXMLSettings(node, *i)) {
				textures.insert(std::make_pair(name, texture));
				log_info("New texture added: '{} ({} kB)'", name, texture->getSizeInBytes() / 1024);
				return true;
			}
		}

		log_error("Could not load texture: '{}'", name);

		return false;
	}
}

void TextureManager::LoadTextureListener(NGE::Events::IEventDataPtr event) {
	std::shared_ptr<LoadTextureEvent> loadTextureEvent = std::dynamic_pointer_cast<LoadTextureEvent>(event);
	bool loadTextureResult = LoadTexture(loadTextureEvent->GetXmlNode());
	loadTextureEvent->SetSuccessful(loadTextureResult);
}

void TextureManager::GetTextureListener(NGE::Events::IEventDataPtr event) {
	std::shared_ptr<GetTextureEvent> getTextureEvent = std::dynamic_pointer_cast<GetTextureEvent>(event);
	Texture* texture = GetTexture(getTextureEvent->GetTextureName());
	if (texture != nullptr) {
		getTextureEvent->SetSuccessful(true);
		getTextureEvent->SetTexture(texture);
	} else {
		getTextureEvent->SetSuccessful(false);
	}
}

Texture* TextureManager::GetTexture(const std::string& name) {
	auto it = textures.find(name);
	if (it != textures.end()) {
		return (it->second);
	}

	log_error("Could not find texture: '{}'", name);

	return NULL;
}

Texture* TextureManager::GetTexture(const pugi::xml_node& node) {
	if (std::string(node.name()) != "Texture2D" && std::string(node.name()) != "TextureCubeMap") {
		log_error("TextureManager --> Need \"Texture2D\" or \"TextureCubeMap\" node");
		return NULL;
	}

	std::string name = node.attribute("name").as_string();

	if (name.length() > 0) {
		if (LoadTexture(node))

			return GetTexture(name);
	}

	return NULL;
}

unsigned int TextureManager::GetTextureCount() {
	return textures.size();
}