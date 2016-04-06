/* 
 * File:   ShaderManager.hpp
 * Author: tku
 *
 * Created on 1 March 2013, 19:48
 */

#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include <map>
#include <pugixml.hpp>

#include "NGE/Core/Singleton.hpp"
#include "NGE/Events/IEventData.hpp"
#include "NGE/Events/EventDelegate.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "LoadShaderEvent.hpp"
#include "GetShaderEvent.hpp"

namespace NGE {
    namespace Media {
        namespace Shaders {

            class ShaderManager : public Core::Singleton<ShaderManager> {
                friend class Core::Singleton<ShaderManager>;

                typedef std::map<NGE::Events::EventType, NGE::Events::EventDelegate> EventDelegateMap;

              private:
                std::map<std::string, GLSLProgram> programs;
                EventDelegateMap eventDelegates;

              private:

                ShaderManager() { }

                ~ShaderManager() { }

              public:
                void Initialise();
                void Deinitialise();

                bool LoadProgram(const std::string& programName, const std::string& fileName);
                GLSLProgram* GetProgram(const std::string& name);
                GLSLProgram* GetProgram(const pugi::xml_node& node);

                void LoadProgramListener(NGE::Events::IEventDataPtr event) {
                    std::shared_ptr<LoadShaderEvent> loadShaderEvent = std::dynamic_pointer_cast<LoadShaderEvent>(event);
                    bool loadShaderResult = LoadProgram(loadShaderEvent->GetProgramName(), loadShaderEvent->GetFileName());
                    loadShaderEvent->successful = loadShaderResult;
                }

                void GetProgramListener(NGE::Events::IEventDataPtr event) {
                    std::shared_ptr<GetShaderEvent> getShaderEvent = std::dynamic_pointer_cast<GetShaderEvent>(event);
                    GLSLProgram* program = GetProgram(getShaderEvent->GetShaderName());
                    if (program != nullptr) {
                        getShaderEvent->successful = true;
                        getShaderEvent->shader = program;
                    } else {
                        getShaderEvent->successful = false;
                    }
                }

                int GetProgramCount();
            };
        }
    }
}

#endif /* SHADERMANAGER_HPP */

