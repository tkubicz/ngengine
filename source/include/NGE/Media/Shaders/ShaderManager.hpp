/* 
 * File:   ShaderManager.hpp
 * Author: tku
 *
 * Created on 1 marzec 2013, 19:48
 */

#ifndef SHADERMANAGER_HPP
#define	SHADERMANAGER_HPP

#include <map>

#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Parsers/pugixml.hpp"

namespace NGE
{
    namespace Media
    {
        namespace Shaders
        {
            class ShaderManager
            {
              public:
                void Initialize();
                void Deinitialize();

                bool LoadProgram(const std::string& programName, const std::string& fileName);
                GLSLProgram* GetProgram(const std::string& name);
                GLSLProgram* GetProgram(const pugi::xml_node& node);
                
                int GetProgramCount();

              private:
                std::map<std::string, GLSLProgram> programs;
            };
        }
    }
}

#endif	/* SHADERMANAGER_HPP */

