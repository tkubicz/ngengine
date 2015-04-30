#include <vector>
#include "NGE/Media/Shaders/ShaderManager.hpp"
#include "NGE/Media//MediaManager.hpp"
#include "NGE/Parsers/pugixml.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::Media::Shaders;

void ShaderManager::Deinitialize()
{
    for (std::map<std::string, GLSLProgram>::iterator it = ShaderManager::programs.begin(); it != ShaderManager::programs.end(); ++it)
        it->second.Unload();

    ShaderManager::programs.clear();
}

bool ShaderManager::LoadProgram(const std::string& programName, const std::string& fileName)
{
    std::map<std::string, GLSLProgram>::iterator it = ShaderManager::programs.find(programName);
    if (it != ShaderManager::programs.end())
        return true;
    else
    {
        GLSLProgram program;
        std::vector<std::string> paths = NGE::Media::MediaManager::GetInstance().GetMediaPathManager().GetPaths("shader");

        for (std::vector<std::string>::iterator i = paths.begin(); i != paths.end(); ++i)
        {
            pugi::xml_document shaderDoc;
            std::string path = *i + fileName;

            pugi::xml_parse_result shaderResult = shaderDoc.load_file(path.c_str());

            if (!shaderResult)
                continue;

            for (pugi::xml_node node = shaderDoc.child("Shader"); node; node = node.next_sibling("Shader"))
            {
                if (node.attribute("name").as_string() == programName && program.LoadXMLSettings(node))
                {
                    ShaderManager::programs.insert(std::make_pair(programName, program));
                    Tools::Logger::WriteInfoLog("ShaderManager --> New shader program added: " + programName);
                    return true;
                }
            }
        }

        Tools::Logger::WriteErrorLog(std::string("ShaderManager --> Could not load program: ") + programName + string(" (") + fileName + std::string(")"));
        return false;
    }
}

GLSLProgram* ShaderManager::GetProgram(const std::string& name)
{
    std::map<std::string, GLSLProgram>::iterator it = ShaderManager::programs.find(name);
    if (it != ShaderManager::programs.end())
    {
        return &(it->second);
    }

    Tools::Logger::WriteErrorLog("ShaderManager --> Could not find program: " + name);
    return NULL;
}

GLSLProgram* ShaderManager::GetProgram(const pugi::xml_node& node)
{
    if (string(node.name()) != "Shader")
    {
        Tools::Logger::WriteErrorLog("ShaderManager --> Need Shader node");
        return NULL;
    }

    std::string shaderName = node.attribute("name").as_string();
    std::string shaderFile = node.attribute("fileName").as_string();

    if (shaderName.length() > 0 && shaderFile.length() > 0)
    {
        if (Media::MediaManager::GetInstance().GetShaderManager().LoadProgram(shaderName, shaderFile))
            return GetProgram(shaderName);
    }
    
    return NULL;
}

int ShaderManager::GetProgramCount()
{
    return programs.size();
}