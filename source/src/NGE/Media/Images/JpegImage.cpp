#ifdef NGE_USE_IMAGE_JPEG
#include <fstream>
#include <jpeglib.h>
#include "NGE/Media/Images/JpegImage.hpp"
#include "NGE/Tools/Logger.hpp"
using namespace NGE::Media::Images;

JpegImage::JpegImage() {
	width = height = bitsPerPixel = bytesPerPixel = 0;
}

JpegImage::~JpegImage() { }

bool JpegImage::load(const std::string& filename, bool suppressError) {
	//std::ifstream fileIn(filename.c_str(), std::ios::binary);
	FILE* fileIn = fopen(filename.c_str(), "rb");

	if (!fileIn) {
		if (!suppressError)
			log_error("JpegImage -> Could not open the JPEG image file for reading: \"" + filename + "\".");
		return false;
	}

	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, fileIn);
	jpeg_read_header(&cinfo, TRUE);

	width = cinfo.image_width;
	height = cinfo.image_height;

	bitsPerPixel = cinfo.num_components * 8;
	bytesPerPixel = cinfo.num_components;

	jpeg_start_decompress(&cinfo);

	unsigned int imageSize = cinfo.output_width * cinfo.output_height * cinfo.num_components;
	imageData.resize(imageSize);

	JSAMPROW rowPointer[1];
	rowPointer[0] = (unsigned char*) malloc(cinfo.output_width * cinfo.num_components);
	unsigned long location = 0;

	while (cinfo.output_scanline < cinfo.image_height) {
		jpeg_read_scanlines(&cinfo, rowPointer, 1);
		for (int i = 0; i < cinfo.image_width * cinfo.num_components; ++i)
			imageData[location++] = rowPointer[0][i];
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(rowPointer[0]);
	fclose(fileIn);

	//FlipImageVertically();

	return true;
}

void JpegImage::unload() {
	imageData.clear();
}

unsigned int JpegImage::getWidth() const {
	return width;
}

unsigned int JpegImage::getHeight() const {
	return height;
}

unsigned int JpegImage::getBitsPerPixel() const {
	return bitsPerPixel;
}

const unsigned char* JpegImage::getImageData() const {
	return &imageData[0];
}

void JpegImage::flipImageVertically() {
	std::vector<unsigned char> flippedData;
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

#endif




