/* 
 * File:   MediaManager.hpp
 * Author: tku
 *
 * Created on 6 March 2013, 12:55
 */

#ifndef MEDIAMANAGER_HPP
#define MEDIAMANAGER_HPP

#include "NGE/Core/Singleton.hpp"
#include "NGE/Media/MediaPathManager.hpp"
#include "NGE/Media/Shaders/ShaderManager.hpp"
#include "NGE/Media/Images/TextureManager.hpp"
#include "NGE/Media/Fonts/FontManager.hpp"

namespace NGE {
	namespace Media {

		class MediaManager : public NGE::Core::Singleton<MediaManager> {
			friend class NGE::Core::Singleton<MediaManager>;

			MediaManager();
			MediaManager(const MediaManager&);
			MediaManager& operator=(const MediaManager&);
			~MediaManager();

		  public:

			void initialize();
			void deinitialize();

			MediaPathManager& getMediaPathManager();
			Shaders::ShaderManager& GetShaderManager();
			Images::TextureManager& GetTextureManager();
			Fonts::FontManager& getFontManager();

		  private:
			MediaPathManager mediaPathManager;
			Fonts::FontManager fontManager;
		};
	}
}

#endif /* MEDIAMANAGER_HPP */

