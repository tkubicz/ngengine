/* 
 * File:   Image.hpp
 * Author: tku
 *
 * Created on 26 czerwiec 2012, 01:36
 */

#ifndef IMAGE_HPP
#define	IMAGE_HPP

#include <string>

namespace NGE {
    namespace Media {
        namespace Images {

            class Image {
              public:
                virtual bool load(const std::string& filename, bool suppressError = false) = 0;
                virtual void unload() = 0;

                virtual unsigned int getWidth() const = 0;
                virtual unsigned int getHeight() const = 0;
                virtual unsigned int getBitsPerPixel() const = 0;
                virtual const unsigned char* getImageData() const = 0;
            };
        }
    }
}

#endif	/* IMAGE_HPP */

