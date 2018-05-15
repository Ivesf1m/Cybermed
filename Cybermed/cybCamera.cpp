#include "cybCamera.h"
#include <gtc\matrix_transform.hpp>

CybCamera::CybCamera(vec3& position, vec3& center, vec3& up)
	: position(position), center(center), up(up)
{
	calculateViewMatrix();
}


CybCamera::~CybCamera()
{

}

void CybCamera::calculateViewMatrix()
{
	viewMatrix = glm::lookAt(position, center, up);
}

vec3& CybCamera::getCenter()
{
	return center;
}

void CybCamera::setCenter(vec3& center)
{
	this->center = center;
	calculateViewMatrix();
}

vec3& CybCamera::getPosition()
{
	return position;
}

void CybCamera::setPosition(vec3& pos)
{
	this->position = pos;
	calculateViewMatrix();
}

vec3& CybCamera::getUpVector()
{
	return up;
}

void CybCamera::setUpVector(vec3& up)
{
	this->up = up;
	calculateViewMatrix();
}

mat4& CybCamera::getViewMatrix()
{
	return viewMatrix;
}

void CybCamera::setViewMatrix(mat4& v)
{
	this->viewMatrix = v;
}
