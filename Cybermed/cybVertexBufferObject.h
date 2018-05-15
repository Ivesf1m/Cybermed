#pragma once

#include "cybVertexArrayObject.h"

template<class Traits>
class CybVertexBufferObject
{
public:
	CybVertexBufferObject();
	~CybVertexBufferObject();

	unsigned int createBuffer(Traits::sMesh* mesh, CybVertexArrayObject& vao);
	void destroyBuffer();

private:
	GLuint index;
	bool created;
};

template<class Traits>
CybVertexBufferObject<Traits>::CybVertexBufferObject()
	: created(false)
{

}

template<class Traits>
CybVertexBufferObject<Traits>::~CybVertexBufferObject()
{
	destroyBuffer();
}

template<class Traits>
unsigned int CybVertexBufferObject<Traits>::createBuffer(Traits::sMesh* mesh,
	CybVertexArrayObject& vao)
{
	if (!mesh) {
		cerr << "Invalid mesh" << endl;
		return;
	}

	if (!vao.isCreated())
		vao.createVAO();
	vao.bindVAO();
}

template<class Traits>
void CybVertexBufferObject<Traits>::destroyBuffer()
{
	if (created) {
		glDeleteBuffers(1, &index);
		created = false;
	}
}

