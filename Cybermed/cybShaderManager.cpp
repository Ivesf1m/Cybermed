#include "cybShaderManager.h"
#include <gtc\type_ptr.hpp>
#include <iostream>

using glm::value_ptr;
using std::cerr;
using std::cout;
using std::endl;

CybShaderManager::CybShaderManager(ifstream& vShaderFile, ifstream&
	fShaderFile)
	: linked(false), handle(glCreateProgram()), 
	vShader(vShaderFile, VERTEX, handle),
	fShader(fShaderFile, FRAGMENT, handle)
{
	if (!handle) {
		cerr << "Error: shader program creation failure." << endl;
		exit(1);
	}
}

CybShaderManager::CybShaderManager(string& vShaderSrcCode, string&
	fShaderSrcCode)
	: linked(false), handle(glCreateProgram()),
	vShader(vShaderSrcCode, VERTEX, handle),
	fShader(fShaderSrcCode, FRAGMENT, handle)
{
	if (!handle) {
		cerr << "Error: shader program creation failure." << endl;
		exit(1);
	}
}

CybShaderManager::~CybShaderManager()
{

}

bool CybShaderManager::compile()
{
	bool status = vShader.compile();
	if (!status) {
		log = vShader.getLog();
		cerr << log << endl;
		return false;
	}

	status = fShader.compile();
	if (!status) {
		log = vShader.getLog();
		cerr << log << endl;
		return false;
	}
	return true;
}

CybShader& CybShaderManager::getFragmentShader()
{
	return fShader;
}

int CybShaderManager::getHandle()
{
	return handle;
}

string& CybShaderManager::getLog()
{
	return log;
}

GLint CybShaderManager::getUniformLocation(string& name)
{
	return glGetUniformLocation(handle, name.c_str());
}

CybShader& CybShaderManager::getVertexShader()
{
	return vShader;
}

bool CybShaderManager::isLinked()
{
	return linked;
}

bool CybShaderManager::link()
{
	if (linked)
		return true;

	if (handle <= 0)
		return false;

	glLinkProgram(handle);

	int status = 0;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		int length = 0;
		log = "";
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			char* cLog = new char[length];
			int written = 0;
			glGetProgramInfoLog(handle, length, &written, cLog);
			log = cLog;
			delete[] cLog;
		}
		return false;
	}
	else {
		linked = true;
		return true;
	}
}

void CybShaderManager::printActiveAttributes()
{
	GLint written, size, location, maxLength, nAttribs;
	GLchar* name;
	GLenum type;

	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &nAttribs);
	name = new GLchar[maxLength];
	cout << " Index | Name" << endl;
	cout << "-----------------------------------------" << endl;
	for (int i = 0; i < nAttribs; ++i) {
		glGetActiveAttrib(handle, i, maxLength, &written, &size, &type, name);
		location = glGetAttribLocation(handle, name);
		cout << location << " | " << name << endl;
	}
	delete[] name;
}

void CybShaderManager::printActiveUniforms()
{
	GLint written, size, location, maxLength, nUniforms;
	GLchar* name;
	GLenum type;

	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
	glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &nUniforms);
	name = new GLchar[maxLength];
	cout << " Location | Name" << endl;
	cout << "-----------------------------------------" << endl;
	for (int i = 0; i < nUniforms; ++i) {
		glGetActiveUniform(handle, i, maxLength, &written, &size, &type, name);
		location = glGetUniformLocation(handle, name);
		cout << location << " | " << name << endl;
	}
	delete[] name;
}

void CybShaderManager::setUniform(string& name, float x, float y, float z)
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0)
		glUniform3f(loc, x, y, z);
	else
		cerr << "The uniform variable " << name << " was not found" << endl;
}

void CybShaderManager::setUniform(string& name, const vec2& v)
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0)
		glUniform2f(loc, v[0], v[1]);
	else
		cerr << "The uniform variable " << name << " was not found" << endl;
}

void CybShaderManager::setUniform(string& name, const vec3& v)
{
	setUniform(name, v[0], v[1], v[2]);
}

void CybShaderManager::setUniform(string& name, const vec4& v)
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0)
		glUniform4f(loc, v[0], v[1], v[2], v[3]);
	else
		cerr << "The uniform variable " << name << " was not found" << endl;
}

void CybShaderManager::setUniform(string& name, const mat3& m)
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0)
		glUniformMatrix3fv(loc, 1, GL_FALSE, value_ptr(m));
	else
		cerr << "The uniform variable " << name << " was not found" << endl;
}

void CybShaderManager::setUniform(string& name, const mat4& m)
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0)
		glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(m));
	else
		cerr << "The uniform variable " << name << " was not found" << endl;
}

void CybShaderManager::setUniform(string& name, float val)
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0)
		glUniform1f(loc, val);
	else
		cerr << "The uniform variable " << name << " was not found" << endl;
}

void CybShaderManager::setUniform(string& name, int val)
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0)
		glUniform1i(loc, val);
	else
		cerr << "The uniform variable " << name << " was not found" << endl;
}

void CybShaderManager::setUniform(string& name, bool val)
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0)
		glUniform1i(loc, val);
	else
		cerr << "The uniform variable " << name << " was not found" << endl;
}

void CybShaderManager::use()
{
	if (handle <= 0 || !linked)
		return;
	glUseProgram(handle);
}

bool CybShaderManager::validate()
{
	if (!linked)
		return false;
	GLint status;
	glValidateProgram(handle);
	glGetProgramiv(handle, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE) {
		int length = 0;
		log = "";
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			char* cLog = new char[length];
			int written = 0;
			glGetProgramInfoLog(handle, length, &written, cLog);
			log = cLog;
			delete[] cLog;
		}
		return false;
	}
	else {
		cout << "Program successfully validated." << endl;
		return true;
	}
}

