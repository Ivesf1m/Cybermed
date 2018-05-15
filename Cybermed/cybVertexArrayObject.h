#pragma once

#include "cybTraits.h"
#include <gl\glew.h>
#include <gl\freeglut.h>

class CybVertexArrayObject
{
public:
	CybVertexArrayObject();
	~CybVertexArrayObject();

	void bindVAO();
	void createVAO();
	void destroyVAO();
	bool isCreated();

private:
	GLuint index;
	bool created;
};

