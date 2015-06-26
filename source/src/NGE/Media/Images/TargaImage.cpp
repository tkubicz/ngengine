#include <iostream>
#include "NGE/Media/Images/TargaImage.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::Media::Images;

TargaImage::TargaImage() {
	width = height = bitsPerPixel = bytesPerPixel = 0;
}

TargaImage::~TargaImage() { }

bool TargaImage::IsImageTypeSupported(const TargaHeader& header) {
	if (((header.imageTypeCode != TFT_RGB) && (header.imageTypeCode != TFT_RLE_RGB)) || header.colorMapType != 0)
		return false;
	return true;
}

bool TargaImage::IsCompressedTarga(const TargaHeader& header) {
	return (header.imageTypeCode == TFT_RLE_RGB || header.imageTypeCode == TFT_RLE_GRAYSCALE);
}

bool TargaImage::IsUncompressedTarga(const TargaHeader& header) {
	return (header.imageTypeCode == TFT_RGB || header.imageTypeCode == TFT_GRAYSCALE);
}

bool TargaImage::Load(const std::string& filename, bool suppressError) {
	ifstream fileIn(filename.c_str(), std::ios::binary);

	if (!fileIn.is_open()) {
		if (!suppressError)
			Tools::Logger::WriteErrorLog("Could not open the targa image file for reading: " + filename);
		return false;
	}

	// Przeczytanie pierwszych 12 bajtów pliku
	fileIn.read(reinterpret_cast<char*> (&header), sizeof (TargaHeader));

	if (!IsImageTypeSupported(header)) {
		Tools::Logger::WriteErrorLog("This is not supported image type");
		return false;
	}

	width = header.width;
	height = header.height;

	bitsPerPixel = header.bpp;
	bytesPerPixel = header.bpp / 8;

	// RGB = 3, RGBA = 4
	if (bytesPerPixel < 3) {
		Tools::Logger::WriteErrorLog("Color depth not supported: " + to_string(bytesPerPixel));
		return false;
	}

	// Obliczenie wielkości obrazka
	unsigned int imageSize = width * height * bytesPerPixel;

	// Zalokowanie pamięci
	imageData.resize(imageSize);

	// Pominięcie id jeżeli istnieje
	if (header.idLength > 0) {
		fileIn.ignore(header.idLength);
	}

	bool result = false;

	if (IsUncompressedTarga(header))
		result = LoadUncompressedTarga(fileIn);
	else
		result = LoadCompressedTarga(fileIn);

	if ((header.imageDesc & TOP_LEFT) == TOP_LEFT)
		FlipImageVertically();

	return result;
}

void TargaImage::Unload() {
	imageData.clear();
}

bool TargaImage::LoadCompressedTarga(ifstream& fileIn) {
	unsigned int pixelCount = height * width;
	unsigned int currentPixel = 0;
	unsigned int currentByte = 0;

	vector<unsigned char> colorBuffer(bytesPerPixel);

	do {
		unsigned char chunkheader = 0;
		fileIn.read(reinterpret_cast<char*> (&chunkheader), sizeof (unsigned char));

		if (chunkheader < 128) {
			chunkheader++;

			for (short counter = 0; counter < chunkheader; counter++) {
				fileIn.read(reinterpret_cast<char*> (&colorBuffer[0]), bytesPerPixel);

				imageData[currentByte] = colorBuffer[2];
				imageData[currentByte + 1] = colorBuffer[1];
				imageData[currentByte + 2] = colorBuffer[0];

				if (bytesPerPixel == 4)
					imageData[currentByte + 3] = colorBuffer[3];

				currentByte += bytesPerPixel;
				currentPixel++;

				if (currentPixel > pixelCount)
					return false;
			}
		} else {
			chunkheader -= 127;

			fileIn.read(reinterpret_cast<char*> (&colorBuffer[0]), bytesPerPixel);

			for (short counter = 0; counter < chunkheader; counter++) {
				imageData[currentByte] = colorBuffer[2];
				imageData[currentByte + 1] = colorBuffer[1];
				imageData[currentByte + 2] = colorBuffer[0];

				if (bytesPerPixel == 4)
					imageData[currentByte + 3] = colorBuffer[3];

				currentByte += bytesPerPixel;
				currentPixel++;

				if (currentPixel > pixelCount)
					return false;
			}
		}

	} while (currentPixel < pixelCount);

	return true;
}

bool TargaImage::LoadUncompressedTarga(ifstream& fileIn) {
	unsigned int imageSize = imageData.size();
	fileIn.read(reinterpret_cast<char*> (&imageData[0]), imageSize);

	// Zamiana czerwonego i niebieskiego tak by kolory były w formacie RGB, a nie BGR
	for (unsigned int swap = 0; swap < imageSize; swap += bytesPerPixel) {
		char cswap = imageData[swap];
		imageData[swap] = imageData[swap + 2];
		imageData[swap + 2] = cswap;
	}

	return true;
}

unsigned int TargaImage::GetWidth() const {
	return width;
}

unsigned int TargaImage::GetHeight() const {
	return height;
}

unsigned int TargaImage::GetBitsPerPixel() const {
	return bitsPerPixel;
}

const unsigned char* TargaImage::GetImageData() const {
	return &imageData[0];
}

void TargaImage::FlipImageVertically() {
	vector<unsigned char> flippedData;
	flippedData.reserve(imageData.size());

	int step = bytesPerPixel;

	for (int row = height - 1; row >= 0; row--) {
		unsigned char* rowData = &imageData[row * width * step];

		for (unsigned int i = 0; i < width * step; ++i) {
			flippedData.push_back(*rowData);
			rowData++;
		}
	}

	imageData.assign(flippedData.begin(), flippedData.end());
}
