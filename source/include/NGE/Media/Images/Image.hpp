/* 
 * File:   Image.hpp
 * Author: tku
 *
 * Created on 26 czerwiec 2012, 01:36
 */

#ifndef IMAGE_HPP
#define	IMAGE_HPP

#include <string>

namespace NGE
{
    namespace Media
    {
        namespace Images
        {
            class Image
            {
              public:
                virtual bool Load(const std::string& filename, bool suppressError = false) = 0;
                virtual void Unload() = 0;

                virtual unsigned int GetWidth() const = 0;
                virtual unsigned int GetHeight() const = 0;
                virtual unsigned int GetBitsPerPixel() const = 0;
                virtual const unsigned char* GetImageData() const = 0;
            };
        }
    }
}

#endif	/* IMAGE_HPP */

