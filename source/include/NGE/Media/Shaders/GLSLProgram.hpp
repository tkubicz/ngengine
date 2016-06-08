/* 
 * File:   GLSLProgram.hpp
 * Author: tku
 *
 * Created on 12 January 2012, 18:02
 */

#ifndef GLSLPROGRAM_HPP
#define GLSLPROGRAM_HPP

#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "pugixml.hpp"
#include "NGE/Core/Core.hpp"
#include "NGE/Math/Vector4.hpp"

using std::string;
using std::ifstream;
using std::map;
using std::vector;

namespace NGE {
	namespace Media {
		namespace Shaders {

			class GLSLProgram {
			  public:

				class GLSLShader {
				  public:

					unsigned int id;
					std::string filename;
					std::string source;

					GLSLShader() : id(0) { }

					void Clear() {
						id = 0;
						filename.clear();
						source.clear();
					}
				};

				class UniformVariable {
				  public:
					unsigned int index;
					std::string name;
					std::string type;
				};

				GLSLProgram() { }
				GLSLProgram(const pugi::xml_node& node);
				GLSLProgram(const string& vertexShaderPath, const string& fragmentShaderPath);
				GLSLProgram(const string& vertexShaderPath, const string& geometryShaderPath, const string& fragmentShaderPath);

				virtual ~GLSLProgram();

				void SetShaderSource(const string& vertexShaderSource, const string& fragmentShaderSource);
				void SetShaderSource(const string& vertexShaderSource, const string& geometryShaderSource, const string& fragmentShaderSource);

				void SetShaderPath(const string& vertexShaderPath, const string& fragmentShaderPath);
				void SetShaderPath(const string& vertexShaderPath, const string& geometryShaderPath, const string& fragmentShaderPath);

				bool LoadXMLSettings(const pugi::xml_node& node);

				bool Initialise(bool bindAttribs = false);

				void Terminate();

				void LinkProgram();

				GLuint GetUniformLocation(const string& name);
				GLuint GetAttribLocation(const string& name);

				void sendUniform(const string& name, const int id);
				void sendUniform(const string& name, const unsigned int id);
				void sendUniform(const string& name, const float red, const float green, const float blue, const float alpha);
				void sendUniform(const string& name, const Math::vec4f& vec);
				void sendUniform(const string& name, const float x, const float y, const float z);
				void sendUniform(const string& name, const Math::vec3f& vec);
				void sendUniform(const string& name, const float scalar);
				void sendUniformArray4(const string& name, const int size, const float* array);
				void sendUniformArray2(const string& name, const int size, const float* array);
				void sendUniform4x4(const string& name, const float* matrix, bool transpose = false);
				void sendUniform3x3(const string& name, const float* matrix, bool transpose = false);

				void AutoBindAttribs();
				void BindAttrib(unsigned int index, const string& attribName);

				void AutoEnableVertexAttribArray();
				void AutoDisableVertexAttribArray();

				void BindShader();
				void UnbindShader();

				const std::string& GetName();

				bool IsInitialised() const;

			  private:

				string ReadFile(const string& filename);
				bool CompileShader(const GLSLShader& shader);
				void OutputShaderLog(unsigned int shaderId);
				void OutputProgramLog(unsigned int programId);

				void DetachShader(unsigned int programId, unsigned int shaderId);
				void DeleteShader(unsigned int shaderId);
				void DeleteProgram(unsigned int programId);

				GLSLShader vertexShader;
				GLSLShader fragmentShader;
				GLSLShader geometryShader;

				unsigned int programId = 0;

				bool xmlShader = false;
				bool fileShader = false;
				bool geometryShaderAvailable = false;

				std::string name;

				map<string, GLuint> uniformMap;
				map<string, GLuint> attribMap;
			};
		}
	}
}

#endif /* GLSLPROGRAM_HPP */

