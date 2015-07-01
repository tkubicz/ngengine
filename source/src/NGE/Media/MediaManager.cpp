#include "NGE/Media/MediaManager.hpp"
using namespace NGE::Media;

MediaManager::MediaManager() { }

MediaManager::MediaManager(const MediaManager&) { }

MediaManager::~MediaManager() { }

void MediaManager::initialize() { }

void MediaManager::deinitialize() {
	shaderManager.deinitialize();
	textureManager.Deinitialize();
	fontManager.Deinitialize();
}

MediaPathManager& MediaManager::getMediaPathManager() {
	return mediaPathManager;
}

Shaders::ShaderManager& MediaManager::getShaderManager() {
	return shaderManager;
}

Images::TextureManager& MediaManager::getTextureManager() {
	return textureManager;
}

Fonts::FontManager& MediaManager::getFontManager() {
	return fontManager;
}
