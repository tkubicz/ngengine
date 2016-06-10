/* 
 * File:   PngImage.hpp
 * Author: tku
 *
 * Created on 1 marzec 2014, 16:25
 */

#ifndef PNGIMAGE_HPP
#define PNGIMAGE_HPP

#ifdef NGE_USE_IMAGE_PNG

#include <vector>
#include <string>
#include "NGE/Media/Images/Image.hpp"

namespace NGE {
	namespace Media {
		namespace Images {

			class PngImage : public Image {
			  public:
				PngImage();
				virtual ~PngImage();

				bool load(const std::string& fileName, bool suppressError = false);
				void unload();

				unsigned int getWidth() const;
				unsigned int getHeight() const;
				unsigned int getBitsPerPixel() const;
				const unsigned char* getImageData() const;

			  protected:
				unsigned int width;
				unsigned int height;
				unsigned int bitsPerPixel;
				unsigned int bytesPerPixel;

				std::vector<unsigned char> imageData;
			};
		}
	}
}

#endif
#endif /* PNGIMAGE_HPP */

