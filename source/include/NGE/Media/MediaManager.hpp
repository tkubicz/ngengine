/* 
 * File:   MediaManager.hpp
 * Author: tku
 *
 * Created on 6 marzec 2013, 12:55
 */

#ifndef MEDIAMANAGER_HPP
#define	MEDIAMANAGER_HPP

#include "NGE/Media/MediaPathManager.hpp"
#include "NGE/Media/Shaders/ShaderManager.hpp"
#include "NGE/Media/Images/TextureManager.hpp"
#include "NGE/Media/Fonts/FontManager.hpp"

namespace NGE {
	namespace Media {

		class MediaManager {
		  public:
			MediaManager();
			MediaManager(const MediaManager&);
			MediaManager& operator=(const MediaManager&);
			~MediaManager();

			static MediaManager& getInstance() {
				static MediaManager instance;
				return instance;
			}

			void initialize();
			void deinitialize();

			MediaPathManager& getMediaPathManager();
			Shaders::ShaderManager& getShaderManager();
			Images::TextureManager& getTextureManager();
			Fonts::FontManager& getFontManager();

		  private:
			MediaPathManager mediaPathManager;
			Shaders::ShaderManager shaderManager;
			Images::TextureManager textureManager;
			Fonts::FontManager fontManager;
		};
	}
}

#endif	/* MEDIAMANAGER_HPP */

