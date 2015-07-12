#ifdef ANDROID

#include "NGE/Media/Files/File.hpp"
using namespace NGE::Media::Files;

AAssetManager* File::assetManager = nullptr;

File::File(std::string name) : filename(name) {
    asset = nullptr;
}

File::~File() {
    Close();
}

bool File::Open() {
    if (assetManager != nullptr) {
        asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_UNKNOWN);
        return !!asset;
    } else {
        Tools::Logger::WriteErrorLog("File --> AAssetManager is null");
        return false;
    }
}

void File::Read(void* buffer, const unsigned int bytesToRead, size_t& bytesRead) {
    bytesRead = AAsset_read(asset, buffer, bytesToRead);
}

void File::Close() {
    if (asset) {
        AAsset_close(asset);
        asset = nullptr;
    }
}

unsigned int File::Length() const {
    return AAsset_getLength(asset);
}

void File::SetAssetManager(AAssetManager* assetManager) {
    File::assetManager = assetManager;
}

#endif  /* ANDROID */





