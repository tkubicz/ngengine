#include <sstream>
#include <boost/lexical_cast.hpp>
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Tools/Logger.hpp"

using namespace NGE::Media::Shaders;

GLSLProgram::GLSLProgram(const string& vertexShader, const string& fragmentShader) {
	this->vertexShader.filename = vertexShader;
	this->fragmentShader.filename = fragmentShader;
	this->xmlShader = false;
}

GLSLProgram::GLSLProgram(const pugi::xml_node& node) {
	if (!LoadXMLSettings(node)) {
		Tools::Logger::WriteErrorLog("Could not load xml settings for " + boost::lexical_cast<string>(node.name()));
	}
}

bool GLSLProgram::SetShader(const string& vertexShaderPath, const string& fragmentShaderPath) {
	this->vertexShader.filename = vertexShaderPath;
	this->fragmentShader.filename = vertexShaderPath;
	this->xmlShader = false;

	// TODO: Check if this method realy should be boolean.
	return true;
}

bool GLSLProgram::LoadXMLSettings(const pugi::xml_node& node) {
	bool link = false, autobind = false;

	name = node.attribute("name").as_string();
	if (name.length() == 0) {
		Tools::Logger::WriteErrorLog("Shader attribute name is not valid");
		return false;
	}

	link = !node.attribute("autoinit").empty() ? node.attribute("autoinit").as_bool() : link;
	autobind = !node.attribute("autobind").empty() ? node.attribute("autobind").as_bool() : autobind;

	pugi::xml_node vertexShaderNode = node.child("VertexShader");
	if (vertexShaderNode.empty() || vertexShaderNode.child("Raw").empty()) {
		Tools::Logger::WriteErrorLog("<VertexShader> node not found in " + name);
		return false;
	} else {
		for (pugi::xml_node attrib = vertexShaderNode.child("Attrib"); attrib; attrib = attrib.next_sibling("Attrib"))
			attribMap.insert(std::make_pair<string, GLuint>(attrib.attribute("name").as_string(), attrib.attribute("index").as_uint()));

		// TODO: Odczytanie zmiennych uniform
		vertexShader.source = vertexShaderNode.child_value("Raw");
	}

	pugi::xml_node fragmentShaderNode = node.child("FragmentShader");
	if (fragmentShaderNode.empty() || fragmentShaderNode.child("Raw").empty()) {
		Tools::Logger::WriteErrorLog("<FragmentShader> node not found in " + name);
		return false;
	} else {
		// TODO: Odczytanie zmiennych uniform
		fragmentShader.source = fragmentShaderNode.child_value("Raw");
	}

	xmlShader = true;

	if (link)
		Initialize(autobind);

	return true;
}

void GLSLProgram::Unload() {
	glDetachShader(programId, vertexShader.id);
	glDetachShader(programId, fragmentShader.id);

	glDeleteShader(vertexShader.id);
	glDeleteShader(fragmentShader.id);
	glDeleteShader(programId);
}

bool GLSLProgram::Initialize(bool autoBindAttribs) {
	programId = -1;
	programId = glCreateProgram();
	if (programId == -1) {
		Tools::Logger::WriteErrorLog("GLSL Shader --> Could not create program");
		return false;
	}

	vertexShader.id = glCreateShader(GL_VERTEX_SHADER);
	if (vertexShader.id == -1) {
		Tools::Logger::WriteErrorLog("GLSL Shader --> Could not create vertex shader");
		return false;
	}

	fragmentShader.id = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragmentShader.id == -1) {
		Tools::Logger::WriteErrorLog("GLSL Shader --> Could not create fragment shader");
		return false;
	}

	if (!xmlShader) {
		vertexShader.source = ReadFile(vertexShader.filename);
		fragmentShader.source = ReadFile(fragmentShader.filename);
	}

	if (vertexShader.source.empty() || fragmentShader.source.empty()) {
		Tools::Logger::WriteErrorLog("GLSL Shader --> Shader source code could not be empty");
		return false;
	}

	const GLchar* temp = static_cast<const GLchar*> (vertexShader.source.c_str());
	glShaderSource(vertexShader.id, 1, (const GLchar**) &temp, NULL);

	temp = static_cast<const GLchar*> (fragmentShader.source.c_str());
	glShaderSource(fragmentShader.id, 1, (const GLchar**) &temp, NULL);

	if (!CompileShader(vertexShader) || !CompileShader(fragmentShader)) {
		Tools::Logger::WriteErrorLog("GLSL Shader --> Could not compile the shaders, they are invalid");
		return false;
	}

	glAttachShader(programId, vertexShader.id);
	glAttachShader(programId, fragmentShader.id);

	if (autoBindAttribs)
		AutoBindAttribs();

	return true;
}

void GLSLProgram::LinkProgram() {
	glLinkProgram(programId);
	OutputProgramLog(programId);
}

GLuint GLSLProgram::GetUniformLocation(const string& name) {
	map<string, GLuint>::iterator i = uniformMap.find(name);
	if (i == uniformMap.end()) {
		GLuint location = glGetUniformLocation(programId, name.c_str());
		uniformMap.insert(std::make_pair(name, location));
		return location;
	}

	return (*i).second;
}

GLuint GLSLProgram::GetAttribLocation(const string& name) {
	map<string, GLuint>::iterator i = attribMap.find(name);
	if (i == attribMap.end()) {
		GLuint location = glGetAttribLocation(programId, name.c_str());
		attribMap.insert(std::make_pair(name, location));
		return location;
	}

	return (*i).second;
}

void GLSLProgram::SendUniform(const string& name, const int id) {
	GLuint location = GetUniformLocation(name);
	glUniform1i(location, id);
}

void GLSLProgram::SendUniform(const string& name, const unsigned int id) {
	GLuint location = GetUniformLocation(name);
	glUniform1i(location, id);
}

void GLSLProgram::SendUniform(const string& name, const float red, const float green, const float blue, const float alpha) {
	GLuint location = GetUniformLocation(name);
	glUniform4f(location, red, green, blue, alpha);
}

void GLSLProgram::SendUniform(const string& name, const NGE::Math::vec4f& vec) {
	SendUniform(name, vec.x, vec.y, vec.z, vec.w);
}

void GLSLProgram::SendUniform(const string& name, const float x, const float y, const float z) {
	GLuint location = GetUniformLocation(name);
	glUniform3f(location, x, y, z);
}

void GLSLProgram::SendUniform(const string& name, const NGE::Math::vec3f& vec) {
	GLuint location = GetUniformLocation(name);
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void GLSLProgram::SendUniform(const string& name, const float scalar) {
	GLuint location = GetUniformLocation(name);
	glUniform1f(location, scalar);
}

void GLSLProgram::SendUniformArray4(const string& name, const int size, const float* array) {
	GLuint location = GetUniformLocation(name);
	glUniform4fv(location, size, array);
}

void GLSLProgram::SendUniformArray2(const string& name, const int size, const float* array) {
	GLuint location = GetUniformLocation(name);
	glUniform2fv(location, size, array);
}

void GLSLProgram::SendUniform3x3(const string& name, const float* matrix, bool transpose) {
	GLuint location = GetUniformLocation(name);
	glUniformMatrix3fv(location, 1, transpose, matrix);
}

void GLSLProgram::SendUniform4x4(const string& name, const float* matrix, bool transpose) {
	GLuint location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, transpose, matrix);
}

void GLSLProgram::AutoBindAttribs() {
	if (attribMap.size() <= 0)
		return;

	for (map<string, GLuint>::iterator i = attribMap.begin(); i != attribMap.end(); ++i)
		BindAttrib(i->second, i->first);

	LinkProgram();
}

void GLSLProgram::BindAttrib(unsigned int index, const string& attribName) {
	glBindAttribLocation(programId, index, attribName.c_str());
}

void GLSLProgram::AutoEnableVertexAttribArray() {
	for (map<string, GLuint>::iterator i = attribMap.begin(); i != attribMap.end(); ++i)
		glEnableVertexAttribArray(i->second);
}

void GLSLProgram::AutoDisableVertexAttribArray() {
	for (map<string, GLuint>::iterator i = attribMap.begin(); i != attribMap.end(); ++i)
		glDisableVertexAttribArray(i->second);
}

void GLSLProgram::BindShader() {
	glUseProgram(programId);
	OutputProgramLog(programId);
}

void GLSLProgram::UnbindShader() {
	glUseProgram(0);
}

const std::string& GLSLProgram::GetName() {
	return name;
}

string GLSLProgram::ReadFile(const string& filename) {
	ifstream fileIn(filename.c_str());

	if (!fileIn.good()) {
		Tools::Logger::WriteErrorLog("Could not load shader: " + to_string(filename));
		return string();
	}

	string stringBuffer(std::istreambuf_iterator<char>(fileIn), (std::istreambuf_iterator<char>()));
	return stringBuffer;
}

bool GLSLProgram::CompileShader(const GLSLShader& shader) {
	glCompileShader(shader.id);
	GLint result = 0xDEADBEEF;
	glGetShaderiv(shader.id, GL_COMPILE_STATUS, &result);

	if (!result) {
		Tools::Logger::WriteErrorLog("Could not compile shader: " + to_string(shader.id));
		OutputShaderLog(shader.id);
		return false;
	}

	return true;
}

void GLSLProgram::OutputShaderLog(unsigned int shaderId) {
	vector<char> infoLog;
	GLint infoLen;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLen);
	infoLog.resize(infoLen);

	std::stringstream ss;
	ss << "GLSL Shader --> Shader " << shaderId << " contains errors, please validate this shader!" << std::endl;
	glGetShaderInfoLog(shaderId, infoLog.size(), &infoLen, &infoLog[0]);
	ss << string(infoLog.begin(), infoLog.end()) << std::endl;

	Tools::Logger::WriteErrorLog(ss.str());
}

void GLSLProgram::OutputProgramLog(unsigned int programId) {
	vector<char> infoLog;
	GLint infoLen;
	GLint result;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);

	if (result == GL_TRUE)
		return;

	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLen);
	infoLog.resize(infoLen);

	std::stringstream ss;
	ss << "GLSL Shader --> Shader " << programId << " contains errors, please validate this shader!" << std::endl;
	glGetProgramInfoLog(programId, infoLog.size(), &infoLen, &infoLog[0]);
	ss << string(infoLog.begin(), infoLog.end()) << std::endl;

	Tools::Logger::WriteErrorLog(ss.str());
}
