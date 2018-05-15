#pragma once

#include "mfVertex.h"
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;

namespace mf
{
	template<class Traits>
	class CybVertex : public mfVertex<3, Traits>
	{
	public:
		CybVertex(float x = 0.0f, float y = 0.0f, float z = 0.0f)
		{
			coords[0] = x;
			coords[1] = y;
			coords[2] = z;
		}

		CybVertex(vec3& _coords)
		{
			coords[0] = _coords[0];
			coords[1] = _coords[1];
			coords[2] = _coords[2];
		}

		~CybVertex()
		{

		}

		/**
		* Returns the normal vector's value in the position pos
		* @param int pos: the position.
		* @return float: the coordinate.
		*/
		inline float getNormalCoord(int pos);

		/**
		* Sets the value of the normal vector in the position pos.
		*
		* @param int pos: the position.
		* @param float value: the value of coordinate.
		* @return void
		*
		*/
		inline void setNormalCoord(int pos, float value);

		inline void setNormal(vec3& n);
		inline vec3& getNormal();

		//Texture coordinates
		inline float getTexCoord(int pos);
		inline void setTexCoord(int pos, float value);
		inline void setTexCoords(vec2& tc);
		inline vec2& getTexCoords();

		//Color
		inline float getColorComponent(int pos);
		inline void setColorComponent(int pos, float value);
		inline void setColor(vec4& tc);
		inline vec4& getColor();

		//auxiliar functions for coordinates
		vec3 getCoords();
		void setCoords(vec3& _coords);

	private:
		typedef typename Traits::sMesh sMesh;
		typedef typename Traits::ids ids;
		typedef typename Traits::space space;

		vec3 normal;
		vec2 texCoords;
		vec4 color;
	};

	template<class Traits>
	vec4& CybVertex<Traits>::getColor()
	{
		return color;
	}

	template<class Traits>
	float CybVertex<Traits>::getColorComponent(int pos)
	{
		assert(pos >= 0 && pos < 4);
		return color[pos];
	}

	template<class Traits>
	vec3& CybVertex<Traits>::getNormal()
	{
		return normal;
	}

	template<class Traits>
	float CybVertex<Traits>::getNormalCoord(int pos)
	{
		assert(pos >= 0 && pos < 3);
		return normal[pos];
	}

	template<class Traits>
	float CybVertex<Traits>::getTexCoord(int pos)
	{
		assert(pos >= 0 && pos < 2);
		return texCoords[pos];
	}

	template<class Traits>
	vec2& CybVertex<Traits>::getTexCoords()
	{
		return texCoords;
	}

	template<class Traits>
	void CybVertex<Traits>::setColor(vec4& c)
	{
		color = c;
	}

	template<class Traits>
	void CybVertex<Traits>::setColorComponent(int pos, float value)
	{
		assert(pos >= 0 && pos < 4);
		color[pos] = value;
	}

	template<class Traits>
	void CybVertex<Traits>::setNormal(vec3& n)
	{
		normal = n;
	}

	template<class Traits>
	void CybVertex<Traits>::setNormalCoord(int pos, float value)
	{
		assert(pos >= 0 && pos < 3);
		normal[pos] = value;
	}

	template<class Traits>
	void CybVertex<Traits>::setTexCoord(int pos, float value)
	{
		assert(pos >= 0 && pos < 2);
		texCoords[pos] = value;
	}

	template<class Traits>
	void CybVertex<Traits>::setTexCoords(vec2& tc)
	{
		texCoords = tc;
	}

	template<class Traits>
	vec3 CybVertex<Traits>::getCoords()
	{
		return vec3(coords[0], coords[1], coords[2]);
	}

	template<class Traits>
	void setCoords(vec3& _coords)
	{
		coords[0] = _coords[0];
		coords[1] = _coords[1];
		coords[2] = _coords[2];
	}
}