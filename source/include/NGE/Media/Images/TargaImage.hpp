/* 
 * File:   Targa.hpp
 * Author: tku
 *
 * Created on 26 luty 2012, 23:56
 */

#ifndef TARGA_HPP
#define	TARGA_HPP

#include <vector>
#include <string>
#include <fstream>

#include "NGE/Media/Images/Image.hpp"

using std::vector;
using std::string;
using std::ifstream;

namespace NGE
{
    namespace Media
    {
        namespace Images
        {

            class TargaImage : public Image
            {
              public:
                TargaImage();
                virtual ~TargaImage();

                bool Load(const std::string& filename, bool suppressError = false);
                void Unload();

                unsigned int GetWidth() const;
                unsigned int GetHeight() const;
                unsigned int GetBitsPerPixel() const;
                const unsigned char* GetImageData() const;

              private:

                enum IMAGE_ORIENTATION
                {
                    BOTTOM_LEFT = 0x00,
                    BOTTOM_RIGHT = 0x10,
                    TOP_LEFT = 0x20,
                    TOP_RIGHT = 0x30
                };

                struct TargaHeader
                {
                    unsigned char idLength;
                    unsigned char colorMapType;
                    unsigned char imageTypeCode;
                    unsigned char colorMapSpec[5];
                    unsigned short xOrigin;
                    unsigned short yOrigin;
                    unsigned short width;
                    unsigned short height;
                    unsigned char bpp;
                    unsigned char imageDesc;
                };

                enum TargaFileTypes
                {
                    TFT_NO_DATA = 0,
                    TFT_INDEXED = 1,
                    TFT_RGB = 2,
                    TFT_GRAYSCALE = 3,
                    TFT_RLE_INDEXED = 9,
                    TFT_RLE_RGB = 10,
                    TFT_RLE_GRAYSCALE = 11
                };

                TargaHeader header;
                unsigned int width;
                unsigned int height;
                unsigned int bitsPerPixel;
                unsigned int bytesPerPixel;

                vector<unsigned char> imageData;

                bool LoadUncompressedTarga(ifstream& fileIn);
                bool LoadCompressedTarga(ifstream& fileIn);

                bool IsImageTypeSupported(const TargaHeader& header);
                bool IsCompressedTarga(const TargaHeader& header);
                bool IsUncompressedTarga(const TargaHeader& header);

                void FlipImageVertically();
            };
        }
    }
}

#endif	/* TARGA_HPP */

