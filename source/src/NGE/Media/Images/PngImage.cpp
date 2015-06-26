#ifdef NGE_USE_IMAGE_PNG
#include <png.h>
#include "NGE/Media/Images/PngImage.hpp"
#include "NGE/Tools/Logger.hpp"
using namespace NGE::Media::Images;

PngImage::PngImage() {
	width = height = bitsPerPixel = bytesPerPixel = 0;
}

PngImage::~PngImage() { }

bool PngImage::Load(const std::string& fileName, bool suppressError) {
	FILE* fileIn = fopen(fileName.c_str(), "rb");

	if (!fileIn) {
		if (!suppressError)
			log_error("PngImage -> Could not open the JPEG image file for reading: \"" + fileName + "\".");
		return false;
	}

	// Read the header.
	png_byte header[8];
	fread(header, 1, 8, fileIn);

	if (png_sig_cmp(header, 0, 8)) {
		if (!suppressError)
			log_error("PngImage -> \"" + fileName + "\" is not a PNG file.");
		return false;
	}

	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr) {
		if (!suppressError)
			log_error("PngImage -> png_create_read_struct returned 0.");
		fclose(fileIn);
		return false;
	}

	// Create png info struct.
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr) {
		if (!suppressError)
			log_error("PngImage -> png_create_info_struct returned 0.");
		png_destroy_read_struct(&pngPtr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fileIn);
		return false;
	}

	// Create png end info struct.
	png_infop endInfo = png_create_info_struct(pngPtr);
	if (!endInfo) {
		if (!suppressError)
			log_error("PngImage -> png_create_info_struct returned 0.");
		png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp) NULL);
		fclose(fileIn);
		return false;
	}

	// Check if we have error from libPNG.
	if (setjmp(png_jmpbuf(pngPtr))) {
		if (!suppressError)
			log_error("PngImage -> Error from libPNG.");
		png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
		fclose(fileIn);
		return false;
	}

	// Initialise png reading.
	png_init_io(pngPtr, fileIn);

	// Let libPNG know you already read the first 8 bytes.
	png_set_sig_bytes(pngPtr, 8);

	// Read all the info up to the image data.
	png_read_info(pngPtr, infoPtr);

	// Variables to pass to get info.
	int bitDepth, colorType;
	png_uint_32 tempWidth, tempHeight;

	// Get info about png.
	png_get_IHDR(pngPtr, infoPtr, &tempWidth, &tempHeight, &bitDepth, &colorType, NULL, NULL, NULL);

	width = tempWidth;
	height = tempHeight;

	// Update the png info struct.
	png_read_update_info(pngPtr, infoPtr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(pngPtr, infoPtr);

	// glTexImage2D requires rows to be 4-byte aligned.
	rowbytes += 3 - ((rowbytes - 1) % 4);

	// Allocate the imageData as big block.
	this->imageData.resize(rowbytes * height * sizeof (png_byte) + 15);

	// rowPointers is pointing to imageData for reading the png.
	png_bytep* rowPointers = (png_bytep*) malloc(height * sizeof (png_bytep));
	if (rowPointers == NULL) {
		if (!suppressError)
			log_error("PngImage -> Could not allocate memory for PNG row pointers.");
		png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
		fclose(fileIn);
		return false;
	}

	// Set the individual rowPointers to point at the correft offset of imageData.
	for (int i = 0; i < height; ++i)
		rowPointers[height - 1 - i] = &imageData[0] + i * rowbytes;

	// Read the png info imageData through rowPointers.
	png_read_image(pngPtr, rowPointers);

	png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
	free(rowPointers);
	fclose(fileIn);

	return true;
}

void PngImage::Unload() {
	imageData.clear();
}

unsigned int PngImage::GetWidth() const {
	return width;
}

unsigned int PngImage::GetHeight() const {
	return height;
}

unsigned int PngImage::GetBitsPerPixel() const {
	return bitsPerPixel;
}

const unsigned char* PngImage::GetImageData() const {
	return &imageData[0];
}

#endif
