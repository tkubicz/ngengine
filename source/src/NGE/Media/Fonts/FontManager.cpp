#include "NGE/Media/Fonts/FontManager.hpp"
#include "NGE/Media/MediaManager.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"

using namespace NGE::Media::Fonts;

void FontManager::deinitialize() {
#ifdef NGE_USE_FREETYPE
    for (std::map<std::string, FreeTypeFont*>::iterator i = fonts.begin(); i != fonts.end(); ++i)
        delete i->second;
#endif
}

bool FontManager::loadFont(const pugi::xml_node& node) {
    if (node.empty() || std::string(node.name()) != "FreeTypeFont") {
        log_error("Need FreeTypeFont node");
        return false;
    }

    std::string name = node.attribute("name").as_string();
    if (name.length() == 0) {
        log_error("Could not find name attribute");
        return false;
    }
#ifdef NGE_USE_FREETYPE  
    std::map<std::string, FreeTypeFont*>::iterator it = fonts.find(name);
    if (it != fonts.end())
        return true;
    else {
        FreeTypeFont* font = new FreeTypeFont();
        std::vector<std::string> paths = MediaManager::getInstance().getMediaPathManager().getPaths("font");

        for (std::vector<std::string>::iterator i = paths.begin(); i != paths.end(); ++i) {
            if (font->LoadXMLSettings(node, *i)) {
                fonts.insert(std::make_pair(name, font));
                log_info("New font added: '{} ({})'", name, font->GetFontName());
                return true;
            }
        }

        delete font;
        log_error("Could not load font: '{}'", name);
        return false;
    }
#else
    return false;
#endif
}

#ifdef NGE_USE_FREETYPE

FreeTypeFont* FontManager::GetFont(const std::string& name) {
    std::map<std::string, FreeTypeFont*>::iterator it = fonts.find(name);
    if (it != fonts.end()) {
        return (it->second);
    }

    log_error("Could not find font: '{}'", name);
    return NULL;
}

FreeTypeFont* FontManager::GetFont(const pugi::xml_node& node) {
    if (std::string(node.name()) != "FreeTypeFont") {
        log_error("Need FreeTypeFont node");
        return NULL;
    }

    std::string name = node.attribute("name").as_string();

    if (name.length() > 0) {
        if (LoadFont(node))
            return GetFont(name);
    }

    return NULL;
}
#endif

int FontManager::getFontCount() {
#ifdef NGE_USE_FREETYPE
    return fonts.size();
#else
    return 0;
#endif

}

