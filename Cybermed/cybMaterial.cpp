#include "cybMaterial.h"

CybMaterial::CybMaterial()
	: ambientReflectance(1.0f), diffuseReflectance(1.0f),
	specularReflectance(1.0f), shininess(1.0f)
{

}


CybMaterial::~CybMaterial()
{

}

float CybMaterial::getAmbientReflectance()
{
	return ambientReflectance;
}

void CybMaterial::setAmbientReflectance(float ra)
{
	ambientReflectance = ra;
}

float CybMaterial::getDiffuseReflectance()
{
	return diffuseReflectance;
}

void CybMaterial::setDiffuseReflectance(float rd)
{
	diffuseReflectance = rd;
}

float CybMaterial::getSpecularReflectance()
{
	return specularReflectance;
}

void CybMaterial::setSpecularReflectance(float rs)
{
	specularReflectance = rs;
}

float CybMaterial::getShininess()
{
	return shininess;
}

void CybMaterial::setShininess(float s)
{
	shininess = s;
}
