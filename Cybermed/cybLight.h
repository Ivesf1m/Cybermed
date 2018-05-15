#pragma once
#include <glm.hpp>

using glm::vec3;

class CybLight
{
public:
	CybLight(vec3& _position = vec3(0.0f, 0.0f, -10.0f),
		vec3& _ambient = vec3(0.1f, 0.1f, 0.1f),
		vec3& _diffuse = vec3(0.5f, 0.5f, 0.5f),
		vec3& _specular = vec3(1.0f, 1.0f, 1.0f));

	~CybLight();

	vec3& getPosition();
	void setPosition(vec3& pos);

	vec3& getAmbientComponent();
	void setAmbientComponent(vec3& ambient);

	vec3& getDiffuseComponent();
	void setDiffuseComponent(vec3& diffuse);

	vec3& getSpecularComponent();
	void setSpecularComponent(vec3& specular);

private:
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

