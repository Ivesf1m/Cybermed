#include "cybLight.h"



CybLight::CybLight(vec3& _position, vec3& _ambient, vec3& _diffuse, 
	vec3& _specular)
	: position(_position), ambient(_ambient), diffuse(_diffuse), 
	specular(_specular)
{

}


CybLight::~CybLight()
{

}

vec3& CybLight::getPosition()
{
	return position;
}

void CybLight::setPosition(vec3& pos)
{
	this->position = pos;
}

vec3& CybLight::getAmbientComponent()
{
	return ambient;
}

void CybLight::setAmbientComponent(vec3& ambient)
{
	this->ambient = ambient;
}

vec3& CybLight::getDiffuseComponent()
{
	return diffuse;
}

void CybLight::setDiffuseComponent(vec3& diffuse)
{
	this->diffuse = diffuse;
}

vec3& CybLight::getSpecularComponent()
{
	return specular;
}

void CybLight::setSpecularComponent(vec3& specular)
{
	this->specular = specular;
}
