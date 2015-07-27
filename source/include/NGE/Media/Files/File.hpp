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
				std::string name;
				unsigned int length;

				static AAssetManager* assetManager;
				AAsset* asset;

			  public:
				File();
				explicit File(std::string name);
				virtual ~File();

				bool Open();
				void Close();

				void Read(void* buffer, size_t& bytesRead);
				void Read(void* buffer, const unsigned int bytesToRead, size_t& bytesRead);

				unsigned int GetLength() const;

				void SetName(const std::string& name);
				std::string GetName() const;

				static const size_t READ_FAILED = 0xFFFFFFFF;

				static void SetAssetManager(AAssetManager* assetManager);
			};
		}
	}
}

#endif  /* ANDROID */

#endif	/* FILE_HPP */

