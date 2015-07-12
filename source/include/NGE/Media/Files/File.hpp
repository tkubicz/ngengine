/* 
 * File:   File.hpp
 * Author: tku
 *
 * Created on 11 lipiec 2015, 22:36
 */

#ifndef FILE_HPP
#define	FILE_HPP

#ifdef ANDROID

#include "NGE/Core/Core.hpp"
#include <android/asset_manager.h>

namespace NGE {
    namespace Media {
        namespace Files {

            class File {
              private:
                std::string filename;
                static AAssetManager* assetManager;
                AAsset* asset;

              public:
                explicit File(std::string name);
                virtual ~File();

                bool Open();
                void Read(void* buffer, const unsigned int bytesToRead, size_t& bytesRead);
                void Close();

                unsigned int Length() const;

                static const size_t READ_FAILED = 0xFFFFFFFF;

                static void SetAssetManager(AAssetManager* assetManager);
            };
        }
    }
}

#endif  /* ANDROID */

#endif	/* FILE_HPP */

