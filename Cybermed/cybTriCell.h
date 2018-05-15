#pragma once

#include <vec3.hpp>
#include "mfTriCell.h"
#include "mfVertexStarIteratorTriSurf.h"

using glm::vec3;

namespace mf
{
	template<class Traits>
	class CybTriCell : public mfTriCell<Traits>
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sMesh sMesh;
		typedef typename Traits::sVertex sVertex;

		/** Constructor */
		CybTriCell() {};

		/** Destrcutor */
		virtual ~CybTriCell() {};

		/**
		* Sets the value of the normal vector.
		* @param vec3& n: the new normal
		* @return void
		*/
		void setNormal(vec3& n);

		/**
		* Returns the normal vector's value
		* @return vec3: the normal.
		*/
		vec3& getNormal();

		/**
		* Returns the normal vector's value from on its components.
		* @param int pos: the component of normal.
		* @return float: the value of the component.
		*/
		float getCellNormalComponent(int pos);

		/**
		* Sets the value of the normal vector for one its components.
		* @param int pos: the component of the normal.
		* @param float value: the new component value.
		* @return void
		*/
		void setCellNormalComponent(int pos, float value);

		/**
		* Updates the cell and vertex normal vector.
		* @param sMesh& mesh: the mesh associated.
		* @return void
		*/
		void updateVerticesNormalVector(sMesh& mesh);

		/**
		* Updates the cell and vertex normal vector.
		* @param sMesh& mesh: the mesh associated.
		* @return void
		*/
		void updateNormalVector(sMesh& mesh);

	private:
		vec3 cNormal; //cell normal vector
	};

	template<class Traits>
	float CybTriCell<Traits>::getCellNormalComponent(int pos)
	{
		assert(pos >= 0 && pos < 3);
		return cNormal[pos];
	}

	template<class Traits>
	void CybTriCell<Traits>::setCellNormalComponent(int pos, float value)
	{
		assert(pos >= 0 && pos < 3);
		cNormal[pos] = value;
	}

	template<class Traits>
	void CybTriCell<Traits>::updateNormalVector(sMesh& mesh)
	{
		vec3 nCell;
		float norm;
		float aux1x, aux1y, aux1z, aux2x, aux2y, aux2z;
		float x[3], y[3], z[3];

		sVertex* v[3];

		v[0] = &(mesh.getVertex(getVertexId(0)));
		v[1] = &(mesh.getVertex(getVertexId(1)));
		v[2] = &(mesh.getVertex(getVertexId(2)));

		for (int j = 0; j < 3; ++j) {
			x[j] = v[j]->getCoord(0);
			y[j] = v[j]->getCoord(1);
			z[j] = v[j]->getCoord(2);
		}

		aux1x = x[1] - x[0];
		aux1y = y[1] - y[0];
		aux1z = z[1] - z[0];
		aux2x = x[2] - x[0];
		aux2y = y[2] - y[0];
		aux2z = z[2] - z[0];

		nCell[0] = (aux1y * aux2z) - (aux2y * aux1z);
		nCell[1] = (aux1z * aux2x) - (aux2z * aux1x);
		nCell[2] = (aux1x * aux2y) - (aux2x * aux1y);

		norm = sqrt(nCell[0] * nCell[0] +
					nCell[1] * nCell[1] +
					nCell[2] * nCell[2]);

		if (norm) {
			cNormal[0] = nCell[0] / norm;
			cNormal[1] = nCell[1] / norm;
			cNormal[2] = nCell[2] / norm;
		}
		else {
			cNormal[0] = cNormal[1] = cNormal[2] = 1.0f;
		}
	}

	template<class Traits>
	void CybTriCell<Traits>::updateVerticesNormalVector(sMesh& mesh)
	{
		mfVertexStarIteratorTriSurf<Traits> its(&mesh);
		vec3 nCell;
		float norm;
		float aux1x, aux1y, aux1z, aux2x, aux2y, aux2z;
		float x[3], y[3], z[3];
		float vNormal[3];
		int count;
		ids idVertex;
		sVertex* v[3];

		v[0] = &(mesh.getVertex(getVertexId(0)));
		v[1] = &(mesh.getVertex(getVertexId(1)));
		v[2] = &(mesh.getVertex(getVertexId(2)));

		for (int j = 0; j < 3; ++j) {
			x[j] = v[j]->getCoord(0);
			y[j] = v[j]->getCoord(1);
			z[j] = v[j]->getCoord(2);
		}

		aux1x = x[1] - x[0];
		aux1y = y[1] - y[0];
		aux1z = z[1] - z[0];
		aux2x = x[2] - x[0];
		aux2y = y[2] - y[0];
		aux2z = z[2] - z[0];

		nCell[0] = (aux1y * aux2z) - (aux2y * aux1z);
		nCell[1] = (aux1z * aux2x) - (aux2z * aux1x);
		nCell[2] = (aux1x * aux2y) - (aux2x * aux1y);

		norm = sqrt(nCell[0] * nCell[0] +
			nCell[1] * nCell[1] +
			nCell[2] * nCell[2]);

		if (norm) {
			cNormal[0] = nCell[0] / norm;
			cNormal[1] = nCell[1] / norm;
			cNormal[2] = nCell[2] / norm;
		}
		else {
			cNormal[0] = cNormal[1] = cNormal[2] = 1.0f;
		}

		//vertex norm update
		for (int i = 0; i < 3; ++i) {
			vNormal[0] = vNormal[1] = vNormal[2] = 0.0f;
			count = 0;
			idVertex = getVertexId(i);

			for (its.initialize(idVertex); its.notFinished(); ++its) {
				nCell = its->getNormal();
				for (int k = 0; k < 3; ++k)
					vNormal[k] += nCell[k];
				count++;
			}

			for (int l = 0; l < 3; ++l)
				vNormal[l] /= count;
			mesh.getVertex(getVertexId(i)).setNormal(vNormal);
		}
	}

	template<class Traits>
	void CybTriCell<Traits>::setNormal(vec3& n)
	{
		cNormal = n;
	}

	template<class Traits>
	vec3& CybTriCell<Traits>::getNormal()
	{
		return cNormal;
	}
}
