#include "NGE/Media/MediaManager.hpp"
using namespace NGE::Media;

MediaManager::MediaManager() { }

MediaManager::MediaManager(const MediaManager&) { }

MediaManager::~MediaManager() { }

void MediaManager::Initialize() { }

void MediaManager::Deinitialize() {
	shaderManager.deinitialize();
	textureManager.Deinitialize();
	fontManager.Deinitialize();
}

MediaPathManager& MediaManager::GetMediaPathManager() {
	return mediaPathManager;
}

Shaders::ShaderManager& MediaManager::GetShaderManager() {
	return shaderManager;
}

Images::TextureManager& MediaManager::GetTextureManager() {
	return textureManager;
}

Fonts::FontManager& MediaManager::GetFontManager() {
	return fontManager;
}
