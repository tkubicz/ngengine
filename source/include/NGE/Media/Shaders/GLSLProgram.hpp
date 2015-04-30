/* 
 * File:   GLSLProgram.hpp
 * Author: tku
 *
 * Created on 12 styczeń 2012, 18:02
 */

#ifndef GLSLPROGRAM_HPP
#define	GLSLPROGRAM_HPP

#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "NGE/Core/Core.hpp"
#include "NGE/Parsers/pugixml.hpp"
#include "NGE/Math/Vector4.hpp"

using std::string;
using std::ifstream;
using std::map;
using std::vector;

namespace NGE
{
    namespace Media
    {
        namespace Shaders
        {

            class GLSLProgram
            {
              public:

                struct GLSLShader
                {
                    unsigned int id;
                    string filename;
                    string source;
                };

                struct UniformVariable
                {
                    unsigned int index;
                    string name;
                    string type;
                };

                GLSLProgram() : xmlShader(false) { }
                GLSLProgram(const pugi::xml_node& node);
                GLSLProgram(const string& vertexShader, const string& fragmentShader);

                virtual ~GLSLProgram() { }

                bool SetShader(const string& vertexShaderPath, const string& fragmentShaderPath);
                bool LoadXMLSettings(const pugi::xml_node& node);

                void Unload();
                bool Initialize(bool autoBindAttribs = false);
                void LinkProgram();

                GLuint GetUniformLocation(const string& name);
                GLuint GetAttribLocation(const string& name);

                void SendUniform(const string& name, const int id);
                void SendUniform(const string& name, const unsigned int id);
                void SendUniform(const string& name, const float red, const float green, const float blue, const float alpha);
                void SendUniform(const string& name, const Math::vec4f& vec);
                void SendUniform(const string& name, const float x, const float y, const float z);
                void SendUniform(const string& name, const Math::vec3f& vec);
                void SendUniform(const string& name, const float scalar);
                void SendUniformArray4(const string& name, const int size, const float* array);
                void SendUniformArray2(const string& name, const int size, const float* array);
                void SendUniform4x4(const string& name, const float* matrix, bool transpose = false);
                void SendUniform3x3(const string& name, const float* matrix, bool transpose = false);

                void AutoBindAttribs();
                void BindAttrib(unsigned int index, const string& attribName);

                void AutoEnableVertexAttribArray();
                void AutoDisableVertexAttribArray();

                void BindShader();
                void UnbindShader();

                const std::string& GetName();

              private:
                string ReadFile(const string& filename);
                bool CompileShader(const GLSLShader& shader);
                void OutputShaderLog(unsigned int shaderId);
                void OutputProgramLog(unsigned int programId);

                GLSLShader vertexShader;
                GLSLShader fragmentShader;
                GLSLShader geometryShader;

                unsigned int programId;
                bool xmlShader;
                std::string name;

                map<string, GLuint> uniformMap;
                map<string, GLuint> attribMap;
            };
        }
    }
}

#endif	/* GLSLPROGRAM_HPP */

