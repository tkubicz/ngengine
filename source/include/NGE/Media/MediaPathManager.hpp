/* 
 * File:   MediaPathManager.hpp
 * Author: tku
 *
 * Created on 6 marzec 2013, 12:49
 */

#ifndef MEDIAPATHMANAGER_HPP
#define	MEDIAPATHMANAGER_HPP

#include <vector>
#include <string>
#include <map>

#include "NGE/Parsers/pugixml.hpp"

namespace NGE
{
    namespace Media
    {
        class MediaPathManager
        {
          public:
            bool LoadXMLSettings(const pugi::xml_node& node);
            
            std::vector<std::string>& GetPaths(std::string key);
            
            void SetPaths(const std::string& key, std::vector<std::string>& paths);
            void SetPath(const std::string& key, const std::string& path);
            
            int GetMediaPathCount();
            
          private:
            std::map<std::string, std::vector<std::string> > mediaPaths;
        };
    }
}

#endif	/* MEDIAPATHMANAGER_HPP */

