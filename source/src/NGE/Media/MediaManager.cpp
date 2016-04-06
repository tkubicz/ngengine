#include "NGE/Media/MediaManager.hpp"
using namespace NGE::Media;

MediaManager::MediaManager() { }

MediaManager::MediaManager(const MediaManager&) { }

MediaManager::~MediaManager() { }

void MediaManager::initialize() {
    GetTextureManager().Initialise();
    GetShaderManager().Initialise();
}

void MediaManager::deinitialize() {
    GetShaderManager().Deinitialise();
    GetTextureManager().Deinitialise();
    getFontManager().deinitialize();
}

MediaPathManager& MediaManager::getMediaPathManager() {
    return mediaPathManager;
}

Shaders::ShaderManager& MediaManager::GetShaderManager() {
    return Shaders::ShaderManager::GetInstance();
}

Images::TextureManager& MediaManager::GetTextureManager() {
    return Images::TextureManager::GetInstance();
}

Fonts::FontManager& MediaManager::getFontManager() {
    return fontManager;
}
