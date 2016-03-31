/* 
 * File:   XMLSettings.hpp
 * Author: tku
 *
 * Created on 29 March 2016, 17:16
 */

#ifndef XMLSETTINGS_HPP
#define XMLSETTINGS_HPP

namespace NGE {
	namespace Core {

		class XMLSettings {
		  public:
			virtual bool LoadXMLSettings(const std::string& xmlFilePath) = 0;
			virtual bool SaveXMLSettings(const std::string& xmlFilePath) = 0;
		};
	}
}

#endif /* XMLSETTINGS_HPP */

