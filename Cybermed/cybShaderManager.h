#pragma once

#include "cybShader.h"
#include <glm.hpp>

using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

class CybShaderManager
{
public:
	CybShaderManager(ifstream& vShaderFile, ifstream& fShaderFile);
	CybShaderManager(string& vShaderSrcCode, string& fShaderSrcCode);

	~CybShaderManager();

	bool compile();
	CybShader& getFragmentShader();
	int getHandle();
	string& getLog();
	CybShader& getVertexShader();
	bool isLinked();
	bool link();
	void printActiveAttributes();
	void printActiveUniforms();
	void setUniform(string& name, float x, float y, float z);
	void setUniform(string& name, const vec2& v);
	void setUniform(string& name, const vec3& v);
	void setUniform(string& name, const vec4& v);
	void setUniform(string& name, const mat3& m);
	void setUniform(string& name, const mat4& m);
	void setUniform(string& name, float val);
	void setUniform(string& name, int val);
	void setUniform(string& name, bool val);
	void use();
	bool validate();

private:
	GLuint handle;
	bool linked;
	CybShader vShader;
	CybShader fShader;
	string log;

	GLint getUniformLocation(string& name);
};

