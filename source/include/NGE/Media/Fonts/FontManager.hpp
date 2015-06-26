/* 
 * File:   FontManager.h
 * Author: tku
 *
 * Created on 16 marzec 2013, 23:15
 */

#ifndef FONTMANAGER_HPP
#define	FONTMANAGER_HPP

#include <map>

#include "NGE/Media/Fonts/FreeTypeFont.hpp"
#include "NGE/Parsers/pugixml.hpp"

namespace NGE {
	namespace Media {
		namespace Fonts {

			class FontManager {
			  public:
				void Initialize();
				void Deinitialize();

				bool LoadFont(const pugi::xml_node& node);
#ifdef NGE_USE_FREETYPE
				FreeTypeFont* GetFont(const std::string& name);
				FreeTypeFont* GetFont(const pugi::xml_node& node);
#endif

				int GetFontCount();

			  private:
#ifdef NGE_USE_FREETYPE
				std::map<std::string, FreeTypeFont*> fonts;
#endif
			};

		}
	}
}

#endif	/* FONTMANAGER_HPP */

