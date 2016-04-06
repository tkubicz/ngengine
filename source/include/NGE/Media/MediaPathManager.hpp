/* 
 * File:   MediaPathManager.hpp
 * Author: tku
 *
 * Created on 6 March 2013, 12:49
 */

#ifndef MEDIAPATHMANAGER_HPP
#define	MEDIAPATHMANAGER_HPP

#include <vector>
#include <string>
#include <map>

#include <pugixml.hpp>

namespace NGE {
	namespace Media {

		class MediaPathManager {
		  public:
			bool loadXMLSettings(const pugi::xml_node& node);

			std::vector<std::string> getPaths(std::string key);

			void setPaths(const std::string& key, std::vector<std::string>& paths);
			void setPath(const std::string& key, const std::string& path);

			int getMediaPathCount();

		  private:
			std::map<std::string, std::vector<std::string> > mediaPaths;
		};
	}
}

#endif	/* MEDIAPATHMANAGER_HPP */

