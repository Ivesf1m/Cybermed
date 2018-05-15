#include "cybVertexArrayObject.h"

CybVertexArrayObject::CybVertexArrayObject()
	: created(false)
{

}

CybVertexArrayObject::~CybVertexArrayObject()
{
	destroyVAO();
}

void CybVertexArrayObject::bindVAO()
{
	if(created)
		glBindVertexArray(index);
}

void CybVertexArrayObject::createVAO()
{
	if (!created) {
		glGenVertexArrays(1, &index);
		created = true;
	}
}

void CybVertexArrayObject::destroyVAO()
{
	if (created) {
		glDeleteVertexArrays(1, &index);
		created = false;
	}
}

bool CybVertexArrayObject::isCreated()
{
	return created;
}