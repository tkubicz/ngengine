#include <vector>
#include "NGE/Media/Shaders/ShaderManager.hpp"
#include "NGE/Media//MediaManager.hpp"
#include <pugixml.hpp>
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "NGE/Events/EventManager.hpp"
#include "NGE/Core/Delegate.hpp"

using namespace NGE::Media::Shaders;

void ShaderManager::Initialise() {
    NGE::Events::EventManager& eventManager = NGE::Events::EventManager::GetInstance();

    eventDelegates.insert(std::make_pair(LoadShaderEvent::eventType, NGE::Events::EventDelegate("load-shader", NGE::Core::make_delegate(GetInstance(), &ShaderManager::LoadProgramListener))));
    eventDelegates.insert(std::make_pair(GetShaderEvent::eventType, NGE::Events::EventDelegate("get-shader", NGE::Core::make_delegate(GetInstance(), &ShaderManager::GetProgramListener))));

    for (auto& delegate : eventDelegates) {
        eventManager.AddListener(delegate.second, delegate.first);
    }
}

void ShaderManager::Deinitialise() {
    for (std::map<std::string, GLSLProgram>::iterator it = ShaderManager::programs.begin(); it != ShaderManager::programs.end(); ++it)
        it->second.unload();

    ShaderManager::programs.clear();

    // Remove event listeners.
    NGE::Events::EventManager& eventManager = NGE::Events::EventManager::GetInstance();
    for (auto& delegate : eventDelegates) {
        eventManager.RemoveListener(delegate.second, delegate.first);
    }
}

bool ShaderManager::LoadProgram(const std::string& programName, const std::string& fileName) {
    std::map<std::string, GLSLProgram>::iterator it = ShaderManager::programs.find(programName);
    if (it != ShaderManager::programs.end())
        return true;
    else {
        GLSLProgram program;
        std::vector<std::string> paths = NGE::Media::MediaManager::GetInstance().getMediaPathManager().getPaths("shader");

        for (std::vector<std::string>::iterator i = paths.begin(); i != paths.end(); ++i) {
            pugi::xml_document shaderDoc;
            std::string path = *i + fileName;

            pugi::xml_parse_result shaderResult = shaderDoc.load_file(path.c_str());

            if (!shaderResult)
                continue;

            for (pugi::xml_node node = shaderDoc.child("Shader"); node; node = node.next_sibling("Shader")) {
                if (node.attribute("name").as_string() == programName && program.loadXMLSettings(node)) {
                    ShaderManager::programs.insert(std::make_pair(programName, program));
                    log_info("New shader program added: '{}'", programName);
                    return true;
                }
            }
        }

        log_error("Could not load program: '{} ({})'", programName, fileName);
        return false;
    }
}

GLSLProgram* ShaderManager::GetProgram(const std::string& name) {
    std::map<std::string, GLSLProgram>::iterator it = ShaderManager::programs.find(name);
    if (it != ShaderManager::programs.end()) {
        return &(it->second);
    }

    log_error("Could not find program: '{}'", name);
    return NULL;
}

GLSLProgram* ShaderManager::GetProgram(const pugi::xml_node& node) {
    if (string(node.name()) != "Shader") {
        log_error("ShaderManager --> Need Shader node");
        return NULL;
    }

    std::string shaderName = node.attribute("name").as_string();
    std::string shaderFile = node.attribute("fileName").as_string();

    if (shaderName.length() > 0 && shaderFile.length() > 0) {
        if (Media::MediaManager::GetInstance().GetShaderManager().LoadProgram(shaderName, shaderFile))
            return GetProgram(shaderName);
    }

    return NULL;
}

int ShaderManager::GetProgramCount() {
    return programs.size();
}