#pragma once

#include <gl\glew.h>
#include <fstream>
#include <string>

using std::ifstream;
using std::string;

enum ShaderType
{
	VERTEX,
	FRAGMENT,
	GEOMETRY,
	TESS_CONTROL,
	TESS_EVAL,
	COMPUTE
};

enum ShaderAttribute
{
	VERTEX_ATTRIB,
	NORMAL_ATTRIB,
	COLOR_ATTRIB,
	TEXTURE0_ATTRIB,
	TEXTURE1_ATTRIB,
	TEXTURE2_ATTRIB,
	TEXTURE3_ATTRIB
};

class CybShader
{
public:
	CybShader(ifstream&, ShaderType, GLuint&);
	CybShader(string&, ShaderType, GLuint&);

	~CybShader();

	void bindInVariable(GLuint location, string& name);
	bool compile();
	int getHandle();
	string& getLog();
	string& getSourceCode();
	void printDescription();

private:
	GLuint handle;
	GLuint prog;
	string log;
	string src;
	ShaderType type;

	void readSourceFile(ifstream& shaderFile);
};

