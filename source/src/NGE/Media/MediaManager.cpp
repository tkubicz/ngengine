#include "NGE/Media/MediaManager.hpp"
using namespace NGE::Media;

MediaManager::MediaManager() {
}

MediaManager::MediaManager(const MediaManager&) {
}

MediaManager::~MediaManager() {
}

void MediaManager::initialize() {
	Images::TextureManager::GetInstance().Initialise();
}

void MediaManager::deinitialize() {
	shaderManager.deinitialize();
	GetTextureManager().Deinitialise();
	fontManager.deinitialize();
}

MediaPathManager& MediaManager::getMediaPathManager() {
	return mediaPathManager;
}

Shaders::ShaderManager& MediaManager::getShaderManager() {
	return shaderManager;
}

Images::TextureManager& MediaManager::GetTextureManager() {
	return Images::TextureManager::GetInstance();
}

Fonts::FontManager& MediaManager::getFontManager() {
	return fontManager;
}
