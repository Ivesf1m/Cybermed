#include "cybShader.h"
#include <iostream>

using std::cout;
using std::endl;

CybShader::CybShader(ifstream& shaderFile, ShaderType t, GLuint& p)
	: handle(0), type(t), prog(p)
{
	readSourceFile(shaderFile);
}

CybShader::CybShader(string& s, ShaderType t, GLuint& p)
	: handle(0), src(s), type(t), prog(p)
{

}

CybShader::~CybShader()
{

}

void CybShader::bindInVariable(GLuint location, string& name)
{
	glBindAttribLocation(handle, location, name.c_str());
}

bool CybShader::compile()
{
	//creation of empty shader
	switch (type) {
	case VERTEX:
		handle = glCreateShader(GL_VERTEX_SHADER);
		break;
	case FRAGMENT:
		handle = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case GEOMETRY:
		handle = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case TESS_CONTROL:
		handle = glCreateShader(GL_TESS_CONTROL_SHADER);
		break;
	case TESS_EVAL:
		handle = glCreateShader(GL_TESS_EVALUATION_SHADER);
		break;
	case COMPUTE:
		handle = glCreateShader(GL_COMPUTE_SHADER);
	default:
		return false;
	}

	//source code and compilation
	const char* cCode = src.c_str();
	GLint tam = src.size();
	glShaderSource(handle, 1, &cCode, &tam);
	glCompileShader(handle);
	
	//compilation verification
	int result;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length = 0;
		log = "";
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			char* cLog = new char[length];
			int written = 0;
			glGetShaderInfoLog(handle, length, &written, cLog);
			log = cLog;
			delete[] cLog;
			glDeleteShader(handle);
		}
		return false;
	}
	else {
		glAttachShader(prog, handle);
		return true;
	}
}

int CybShader::getHandle()
{
	return handle;
}

string& CybShader::getLog()
{
	return log;
}

string& CybShader::getSourceCode()
{
	return src;
}

void CybShader::readSourceFile(ifstream& shaderFile)
{
	if (!shaderFile)
		return;
	src = "";
	string next;
	while (shaderFile) {
		shaderFile >> next;
		src += next;
	}
	shaderFile.close();
}

void CybShader::printDescription()
{
	cout << "Shader description" << endl;
	cout << "Associated GL program: " << prog << endl;
	cout << "Handle: " << handle << endl;
	cout << "Type: " << type << endl;
	cout << "Source code: \n" << src << endl;
}