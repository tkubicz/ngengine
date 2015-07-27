#ifdef ANDROID

#include "NGE/Media/Files/File.hpp"
using namespace NGE::Media::Files;

AAssetManager* File::assetManager = nullptr;

File::File() {
	asset = nullptr;
}

File::File(std::string name) : name(name) {
	asset = nullptr;
}

File::~File() {
	Close();
}

bool File::Open() {
	if (assetManager == nullptr) {
		Tools::Logger::WriteErrorLog("File --> AAssetManager is null");
		return false;
	}

	if (name.empty()) {
		Tools::Logger::WriteErrorLog("File --> filename is empty");
		return false;
	}

	asset = AAssetManager_open(assetManager, name.c_str(), AASSET_MODE_UNKNOWN);
	if (asset) {
		length = AAsset_getLength(asset);
	}
	return !!asset;
}

void File::Read(void* buffer, size_t& bytesRead) {
	bytesRead = AAsset_read(asset, buffer, length);
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

unsigned int File::GetLength() const {
	return length;
}

void File::SetName(const std::string& name) {
	this->name = name;
}

std::string File::GetName() const {
	return name;
}

void File::SetAssetManager(AAssetManager* assetManager) {
	File::assetManager = assetManager;
}

#endif  /* ANDROID */





