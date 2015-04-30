/* 
 * File:   PngImage.hpp
 * Author: tku
 *
 * Created on 1 marzec 2014, 16:25
 */

#ifndef PNGIMAGE_HPP
#define	PNGIMAGE_HPP

#ifdef NGE_USE_IMAGE_PNG

#include <vector>
#include <string>
#include "NGE/Media/Images/Image.hpp"

namespace NGE
{
    namespace Media
    {
        namespace Images
        {
            class PngImage : public Image
            {
              public:
                PngImage();
                virtual ~PngImage();

                bool Load(const std::string& fileName, bool suppressError = false);
                void Unload();

                unsigned int GetWidth() const;
                unsigned int GetHeight() const;
                unsigned int GetBitsPerPixel() const;
                const unsigned char* GetImageData() const;

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
#endif	/* PNGIMAGE_HPP */

