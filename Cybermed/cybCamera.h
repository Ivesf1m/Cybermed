#pragma once

#include <mat4x4.hpp>
#include <vec3.hpp>

using glm::mat4;
using glm::vec3;

class CybCamera
{
public:
	CybCamera(vec3& position = vec3(0.0f, 0.0f, 10.0f),
		vec3& center = vec3(0.0f, 0.0f, 0.0f),
		vec3& up = vec3(0.0f, 1.0f, 0.0f));
	~CybCamera();

	void calculateViewMatrix();

	//getters and setters
	vec3& getCenter();
	void setCenter(vec3& center);

	vec3& getPosition();
	void setPosition(vec3& pos);

	vec3& getUpVector();
	void setUpVector(vec3& up);

	mat4& getViewMatrix();
	void setViewMatrix(mat4& v);

private:
	vec3 position;
	vec3 up;
	vec3 center;
	mat4 viewMatrix;
};

