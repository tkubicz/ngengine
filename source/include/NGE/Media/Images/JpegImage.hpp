/* 
 * File:   JpegImage.hpp
 * Author: tku
 *
 * Created on 1 marzec 2014, 13:15
 */

#ifndef JPEGIMAGE_HPP
#define	JPEGIMAGE_HPP

#ifdef NGE_USE_IMAGE_JPEG

#include <vector>
#include <string>
#include "NGE/Media/Images/Image.hpp"

namespace NGE {
	namespace Media {
		namespace Images {

			class JpegImage : public Image {
			  public:
				JpegImage();
				virtual ~JpegImage();

				bool load(const std::string& filename, bool suppressError = false);
				void unload();

				unsigned int getWidth() const;
				unsigned int getHeight() const;
				unsigned int getBitsPerPixel() const;
				const unsigned char* getImageData() const;

			  protected:
				void flipImageVertically();

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
#endif	/* JPEGIMAGE_HPP */

