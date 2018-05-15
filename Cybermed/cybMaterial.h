#pragma once

class CybMaterial
{
public:
	CybMaterial();
	~CybMaterial();

	float getAmbientReflectance();
	void setAmbientReflectance(float ra);

	float getDiffuseReflectance();
	void setDiffuseReflectance(float rd);

	float getSpecularReflectance();
	void setSpecularReflectance(float rs);

	float getShininess();
	void setShininess(float s);

private:
	float ambientReflectance;
	float diffuseReflectance;
	float specularReflectance;
	float shininess;
};

