#include <map>
#include <vector>

#include "NGE/Media/MediaPathManager.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::Media;

bool MediaPathManager::loadXMLSettings(const pugi::xml_node& node) {
	for (pugi::xml_node path = node.child("MediaPath"); path; path = path.next_sibling("MediaPath")) {
		if (path.attribute("type").empty() || path.attribute("path").empty())
			continue;

		std::string strType = path.attribute("type").as_string();
		std::string strPath = path.attribute("path").as_string();

		std::map<std::string, std::vector<std::string> >::iterator i = mediaPaths.find(strType);
		if (i != mediaPaths.end()) {
			bool addNew = true;
			for (std::vector<std::string>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
				if (*j == strPath) {
					addNew = false;
					break;
				}
			}
			if (addNew) {
				i->second.push_back(strPath);
				Tools::Logger::WriteInfoLog("MediaPathManager --> New media path added - type: " + i->first + ", path: " + strPath);
			}
		} else {
			std::vector<std::string> paths;
			paths.push_back(strPath);
			mediaPaths.insert(std::make_pair(strType, paths));
			Tools::Logger::WriteInfoLog("MediaPathManager --> New media path type registered - type: " + strType + ", path: " + strPath);
		}
	}

	return true;
}

// TODO: Zmodyfikować kod tak, by zwracać wskaźnik, a nie referencję z uwagi
// na brak możliwości zwrócenia pustej referencji.

std::vector<std::string>& MediaPathManager::getPaths(std::string key) {
	std::map<std::string, std::vector<std::string> >::iterator i = mediaPaths.find(key);
	if (i != mediaPaths.end())
		return i->second;
}

int MediaPathManager::getMediaPathCount() {
	int count = 0;
	for (std::map<std::string, std::vector<std::string> >::iterator i = mediaPaths.begin(); i != mediaPaths.end(); ++i)
		count += i->second.size();

	return count;
}
