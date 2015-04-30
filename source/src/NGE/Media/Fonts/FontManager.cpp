#include "NGE/Media/Fonts/FontManager.hpp"
#include "NGE/Media/MediaManager.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::Media::Fonts;

void FontManager::Deinitialize()
{
#ifdef NGE_USE_FREETYPE
    for (std::map<std::string, FreeTypeFont*>::iterator i = fonts.begin(); i != fonts.end(); ++i)
        delete i->second;
#endif
}

bool FontManager::LoadFont(const pugi::xml_node& node)
{
    if (node.empty() || std::string(node.name()) != "FreeTypeFont")
    {
        Tools::Logger::WriteErrorLog("FontManager --> Need FreeTypeFont node");
        return false;
    }

    std::string name = node.attribute("name").as_string();
    if (name.length() == 0)
    {
        Tools::Logger::WriteErrorLog("FontManager --> Could not find name attribute");
        return false;
    }
#ifdef NGE_USE_FREETYPE  
    std::map<std::string, FreeTypeFont*>::iterator it = fonts.find(name);
    if (it != fonts.end())
        return true;
    else
    {
        FreeTypeFont* font = new FreeTypeFont();
        std::vector<std::string> paths = MediaManager::GetInstance().GetMediaPathManager().GetPaths("font");

        for (std::vector<std::string>::iterator i = paths.begin(); i != paths.end(); ++i)
        {
            if (font->LoadXMLSettings(node, *i))
            {
                fonts.insert(std::make_pair(name, font));
                Tools::Logger::WriteInfoLog("FontManager --> New font added: " + name + " (" + font->GetFontName() + ")");
                return true;
            }
        }

        delete font;
        Tools::Logger::WriteErrorLog(std::string("FontManager --> Could not load font: ") + name);
        return false;
    }
#else
    return false;
#endif
}

#ifdef NGE_USE_FREETYPE
FreeTypeFont* FontManager::GetFont(const std::string& name)
{
    std::map<std::string, FreeTypeFont*>::iterator it = fonts.find(name);
    if (it != fonts.end())
    {
        return (it->second);
    }

    Tools::Logger::WriteErrorLog("FontManager --> Could not find font: " + name);
    return NULL;
}

FreeTypeFont* FontManager::GetFont(const pugi::xml_node& node)
{
    if (std::string(node.name()) != "FreeTypeFont")
    {
        Tools::Logger::WriteErrorLog("FontManager --> Need FreeTypeFont node");
        return NULL;
    }

    std::string name = node.attribute("name").as_string();

    if (name.length() > 0)
    {
        if (LoadFont(node))
            return GetFont(name);
    }

    return NULL;
}
#endif

int FontManager::GetFontCount()
{
#ifdef NGE_USE_FREETYPE
    return fonts.size();
#else
    return 0;
#endif

}

