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
				};

				class UniformVariable {
				  public:
					unsigned int index;
					std::string name;
					std::string type;
				};

				GLSLProgram() : xmlShader(false) { }
				GLSLProgram(const pugi::xml_node& node);
				GLSLProgram(const string& vertexShader, const string& fragmentShader);

				virtual ~GLSLProgram() { }

				bool setShaderSource(const string vertexShaderSource, const string fragmentShaderSource);
				bool setShader(const string& vertexShaderPath, const string& fragmentShaderPath);
				bool loadXMLSettings(const pugi::xml_node& node);

				void unload();
				bool initialize(bool bindAttribs = false);
				void linkProgram();

				GLuint getUniformLocation(const string& name);
				GLuint getAttribLocation(const string& name);

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

				void autoBindAttribs();
				void bindAttrib(unsigned int index, const string& attribName);

				void autoEnableVertexAttribArray();
				void autoDisableVertexAttribArray();

				void bindShader();
				void unbindShader();

				const std::string& getName();

			  private:

				string readFile(const string& filename);
				bool compileShader(const GLSLShader& shader);
				void outputShaderLog(unsigned int shaderId);
				void outputProgramLog(unsigned int programId);

				GLSLShader vertexShader;
				GLSLShader fragmentShader;
				GLSLShader geometryShader;

				unsigned int programId;
				bool xmlShader;
				bool fileShader;
				std::string name;

				map<string, GLuint> uniformMap;
				map<string, GLuint> attribMap;
			};
		}
	}
}

#endif	/* GLSLPROGRAM_HPP */

