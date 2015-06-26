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

				bool Load(const std::string& filename, bool suppressError = false);
				void Unload();

				unsigned int GetWidth() const;
				unsigned int GetHeight() const;
				unsigned int GetBitsPerPixel() const;
				const unsigned char* GetImageData() const;

			  protected:
				void FlipImageVertically();

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

