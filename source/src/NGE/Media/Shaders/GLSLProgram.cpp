#include <sstream>
#include <boost/lexical_cast.hpp>
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Tools/Logger/NewLogger.hpp"
#include "pugixml.hpp"

using namespace NGE::Media::Shaders;

GLSLProgram::GLSLProgram(const string& vertexShaderPath, const string& fragmentShaderPath) {
	SetShaderPath(vertexShaderPath, fragmentShaderPath);
}

GLSLProgram::GLSLProgram(const string& vertexShaderPath, const string& geometryShaderPath, const string& fragmentShaderPath) {
	SetShaderSource(vertexShaderPath, geometryShaderPath, fragmentShaderPath);
}

GLSLProgram::GLSLProgram(const pugi::xml_node& node) {
	if (!LoadXMLSettings(node)) {
		log_error("Could not load xml settings for: '{}'", node.name());
	}
}

GLSLProgram::~GLSLProgram() {
	Terminate();
}

void GLSLProgram::SetShaderSource(const string& vertexShaderSource, const string& fragmentShaderSource) {
	vertexShader.source = vertexShaderSource;
	fragmentShader.source = fragmentShaderSource;

	this->xmlShader = false;
	this->fileShader = false;
	this->geometryShaderAvailable = false;
}

void GLSLProgram::SetShaderSource(const string& vertexShaderSource, const string& geometryShaderSource, const string& fragmentShaderSource) {
	vertexShader.source = vertexShaderSource;
	geometryShader.source = geometryShaderSource;
	fragmentShader.source = fragmentShaderSource;

	this->xmlShader = false;
	this->fileShader = false;
	this->geometryShaderAvailable = true;
}

void GLSLProgram::SetShaderPath(const string& vertexShaderPath, const string& fragmentShaderPath) {
	this->vertexShader.filename = vertexShaderPath;
	this->fragmentShader.filename = fragmentShaderPath;

	this->xmlShader = false;
	this->fileShader = true;
	this->geometryShaderAvailable = false;
}

void GLSLProgram::SetShaderPath(const string& vertexShaderPath, const string& geometryShaderPath, const string& fragmentShaderPath) {
	this->vertexShader.filename = vertexShaderPath;
	this->geometryShader.filename = geometryShaderPath;
	this->fragmentShader.filename = fragmentShaderPath;

	this->xmlShader = false;
	this->fileShader = true;
	this->geometryShaderAvailable = true;
}

bool GLSLProgram::LoadXMLSettings(const pugi::xml_node& node) {
	bool link = false, autobind = false;

	name = node.attribute("name").as_string();
	if (name.length() == 0) {
		log_error("Shader attribute name is not valid");
		return false;
	}

	link = !node.attribute("autoinit").empty() ? node.attribute("autoinit").as_bool() : link;
	autobind = !node.attribute("autobind").empty() ? node.attribute("autobind").as_bool() : autobind;

	pugi::xml_node vertexShaderNode = node.child("VertexShader");
	if (vertexShaderNode.empty() || vertexShaderNode.child("Raw").empty()) {
		log_error("<VertexShader> node not found in: '{}'", name);
		return false;
	} else {
		for (pugi::xml_node attrib = vertexShaderNode.child("Attrib"); attrib; attrib = attrib.next_sibling("Attrib"))
			attribMap.insert(std::make_pair<string, GLuint>(attrib.attribute("name").as_string(), attrib.attribute("index").as_uint()));

		// TODO: Read uniform variables here.
		vertexShader.source = vertexShaderNode.child_value("Raw");
	}

	pugi::xml_node geometryShaderNode = node.child("GeometryShader");
	if (!geometryShaderNode.empty() && !geometryShaderNode.child("Raw").empty()) {
		geometryShaderAvailable = true;
		geometryShader.source = geometryShaderNode.child_value("Raw");
	}

	pugi::xml_node fragmentShaderNode = node.child("FragmentShader");
	if (fragmentShaderNode.empty() || fragmentShaderNode.child("Raw").empty()) {
		log_error("<FragmentShader> node not found in: '{}'", name);
		return false;
	} else {
		// TODO: Read uniform variables here.
		fragmentShader.source = fragmentShaderNode.child_value("Raw");
	}

	xmlShader = true;

	if (link)
		Initialise(autobind);

	return true;
}

void GLSLProgram::Terminate() {

	DetachShader(programId, vertexShader.id);
	DetachShader(programId, geometryShader.id);
	DetachShader(programId, fragmentShader.id);

	DeleteShader(vertexShader.id);
	DeleteShader(geometryShader.id);
	DeleteShader(fragmentShader.id);

	DeleteShader(programId);

	vertexShader.Clear();
	geometryShader.Clear();
	fragmentShader.Clear();
}

// TOOD: Refactor this method.

bool GLSLProgram::Initialise(bool bindAttribs) {
	if (!xmlShader && fileShader) {
		vertexShader.source = ReadFile(vertexShader.filename);
		fragmentShader.source = ReadFile(fragmentShader.filename);

		if (geometryShaderAvailable) {
			geometryShader.source = ReadFile(geometryShader.filename);
		}
	}

	if (vertexShader.source.empty() || fragmentShader.source.empty()) {
		log_error("Shader source code could not be empty");
		return false;
	}

	if (geometryShaderAvailable && geometryShader.source.empty()) {
		log_error("Geometry shader source code could not be empty");
		return false;
	}

	programId = glCreateProgram();
	if (programId == 0) {
		log_error("Could not create program");
		return false;
	}

	vertexShader.id = glCreateShader(GL_VERTEX_SHADER);
	if (vertexShader.id == 0) {
		log_error("Could not create vertex shader");
		return false;
	}

	fragmentShader.id = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragmentShader.id == 0) {
		log_error("Could not create fragment shader");
		return false;
	}

	if (geometryShaderAvailable) {
		geometryShader.id = glCreateShader(GL_GEOMETRY_SHADER);
		if (geometryShader.id == 0) {
			log_error("Could not create geometry shader");
			return false;
		}
	}

	const GLchar* temp = static_cast<const GLchar*> (vertexShader.source.c_str());
	glShaderSource(vertexShader.id, 1, (const GLchar**) &temp, NULL);

	temp = static_cast<const GLchar*> (fragmentShader.source.c_str());
	glShaderSource(fragmentShader.id, 1, (const GLchar**) &temp, NULL);

	if (geometryShaderAvailable) {
		temp = static_cast<const GLchar*> (geometryShader.source.c_str());
		glShaderSource(geometryShader.id, 1, (const GLchar**) &temp, NULL);
	}

	if (!CompileShader(vertexShader) || !CompileShader(fragmentShader)) {
		log_error("Could not compile the shaders, they are invalid");
		return false;
	}

	if (geometryShaderAvailable && !CompileShader(geometryShader)) {
		log_error("Could not compile geometry shader");
		return false;
	}

	glAttachShader(programId, vertexShader.id);
	glAttachShader(programId, fragmentShader.id);
	if (geometryShaderAvailable) {
		glAttachShader(programId, geometryShader.id);
	}

	if (bindAttribs)
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

void GLSLProgram::sendUniform(const string& name, const int id) {
	GLuint location = GetUniformLocation(name);
	glUniform1i(location, id);
}

void GLSLProgram::sendUniform(const string& name, const unsigned int id) {
	GLuint location = GetUniformLocation(name);
	glUniform1i(location, id);
}

void GLSLProgram::sendUniform(const string& name, const float red, const float green, const float blue, const float alpha) {
	GLuint location = GetUniformLocation(name);
	glUniform4f(location, red, green, blue, alpha);
}

void GLSLProgram::sendUniform(const string& name, const NGE::Math::vec4f& vec) {
	sendUniform(name, vec.x, vec.y, vec.z, vec.w);
}

void GLSLProgram::sendUniform(const string& name, const float x, const float y, const float z) {
	GLuint location = GetUniformLocation(name);
	glUniform3f(location, x, y, z);
}

void GLSLProgram::sendUniform(const string& name, const NGE::Math::vec3f& vec) {
	GLuint location = GetUniformLocation(name);
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void GLSLProgram::sendUniform(const string& name, const float scalar) {
	GLuint location = GetUniformLocation(name);
	glUniform1f(location, scalar);
}

void GLSLProgram::sendUniformArray4(const string& name, const int size, const float* array) {
	GLuint location = GetUniformLocation(name);
	glUniform4fv(location, size, array);
}

void GLSLProgram::sendUniformArray2(const string& name, const int size, const float* array) {
	GLuint location = GetUniformLocation(name);
	glUniform2fv(location, size, array);
}

void GLSLProgram::sendUniform3x3(const string& name, const float* matrix, bool transpose) {
	GLuint location = GetUniformLocation(name);
	glUniformMatrix3fv(location, 1, transpose, matrix);
}

void GLSLProgram::sendUniform4x4(const string& name, const float* matrix, bool transpose) {
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
		log_error("Could not load shader source from: '{}'", filename);
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
		log_error("Could not compile shader: '{}'", shader.id);
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
	ss << "Shader " << shaderId << " contains errors, please validate this shader!" << std::endl;
	glGetShaderInfoLog(shaderId, infoLog.size(), &infoLen, &infoLog[0]);
	ss << string(infoLog.begin(), infoLog.end()) << std::endl;

	log_error("{}", ss.str());
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
	ss << "Shader " << programId << " contains errors, please validate this shader!" << std::endl;
	glGetProgramInfoLog(programId, infoLog.size(), &infoLen, &infoLog[0]);
	ss << string(infoLog.begin(), infoLog.end()) << std::endl;

	log_error("{}", ss.str());
}

void GLSLProgram::DetachShader(unsigned int programId, unsigned int shaderId) {
	if (programId != 0 && shaderId != 0) {
		glDetachShader(programId, shaderId);
	}
}

void GLSLProgram::DeleteShader(unsigned int shaderId) {
	if (shaderId != 0) {
		glDeleteShader(shaderId);
	}
}