#pragma once

#include <cmath>
#include "mfMesh.h"
#include "mfVertexStarIteratorTriSurf.h"
#include <vec3.hpp>

using glm::vec3;

namespace mf
{
#define IDS typename CybMesh<Traits>::ids
#define SPACE typename CybMesh<Traits>::space

	template<class Traits>
	class CybMesh : public mfMesh<Traits>
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sOper sOper;

		/** Constructor */
		CybMesh()
			: maxIncidEdges(0)
		{

		}

		/** Destructor */
		virtual ~CybMesh()
		{

		}

		/**
		* Adds a cell in mesh.
		*
		* @param ids idvertices: vector with vertices ids of the cell
		* @return ids: the cell id in mesh
		*/
		ids addCell(vector<ids>& idvertices);

		/**
		* Updates the vertices normal vector of one cell
		*
		* @param int idCell: the cell id
		* @return void
		*/
		void updateCellVerticesNormal(int idCell);

		/**
		* Updates the cell normal vector
		*
		* @param int idCell the cell id
		* @return void
		*/
		void updateCellNormalVector(int idCell);

		/**
		* Updates the normal vector of one vertex
		*
		* @param ids idVertex: the vertex id
		* @return void
		*/
		void updateVertexNormalVector(ids idVertex);

		/**
		* Returns the maximum number of incident edges in one vertex
		*
		* @param void
		* @return int: the mesh maximum number of incident edges in one vertex
		*/
		int getMaxIncidEdges();

		/**
		* Returns the length of one edge
		*
		* @param sCell& cell: the cell which contains the edge
		* @param int coordinate: 0 for x, 1 for y and 2 for z
		* @param int edge: one of the three edges
		* @return int: the maximum number of incident edges in one vertex
		*/
		space getEdgeLength(sCell& cell, int coordinate, int edge);

	private:
		int maxIncidEdges;
	};

	template<class Traits>
	int CybMesh<Traits>::getMaxIncidEdges()
	{
		return maxIncidEdges;
	}

	template<class Traits>
	IDS CybMesh<Traits>::addCell(vector<ids>& idvertices)
	{
		int idcell, i;
		space aux1x, aux1y, aux1z, aux2x, aux2y, aux2z;
		space x1, x2, x3, y1, y2, y3, z1, z2, z3;
		vec3 vnormal;
		float norm;

		idcell = mfMesh<Traits>::addCell(idvertices);

		x1 = getVertex(idvertices[0]).getCoord(0);
		y1 = getVertex(idvertices[0]).getCoord(1);
		z1 = getVertex(idvertices[0]).getCoord(2);
		x2 = getVertex(idvertices[1]).getCoord(0);
		y2 = getVertex(idvertices[1]).getCoord(1);
		z2 = getVertex(idvertices[1]).getCoord(2);
		x3 = getVertex(idvertices[2]).getCoord(0);
		y3 = getVertex(idvertices[2]).getCoord(1);
		z3 = getVertex(idvertices[2]).getCoord(2);

		aux1x = x2 - x1;
		aux1y = y2 - y1;
		aux1z = z2 - z1;
		aux2x = x3 - x1;
		aux2y = y3 - y1;
		aux2z = z3 - z1;

		vnormal[0] = (aux1y * aux2z) - (aux2y * aux1z);
		vnormal[1] = (aux1z * aux2x) - (aux2z * aux1x);
		vnormal[2] = (aux1x * aux2y) - (aux2x * aux1y);

		norm = sqrt(vnormal[0] * vnormal[0] +
			vnormal[1] * vnormal[1] +
			vnormal[2] * vnormal[2]);

		if (norm != 0)
			for (i = 0; i < 3; ++i)
				vnormal[i] /= norm;
		else
			for (i = 0; i < 3; ++i)
				vnormal[i] = 1;

		getCell(idcell).setNormal(vnormal);
		return idcell;		
	}

	template<class Traits>
	void CybMesh<Traits>::updateCellNormalVector(int idcell)
	{
		sCell& cell = getCell(idcell);
		int count, i;
		vec3 normal, ncell;
		float norm;
		space aux1x, aux1y, aux1z, aux2x, aux2y, aux2z;
		space x1, x2, x3, y1, y2, y3, z1, z2, z3;

		x1 = getVertex(cell.getVertexId(0)).getCoord(0);
		y1 = getVertex(cell.getVertexId(0)).getCoord(1);
		z1 = getVertex(cell.getVertexId(0)).getCoord(2);
		x2 = getVertex(cell.getVertexId(1)).getCoord(0);
		y2 = getVertex(cell.getVertexId(1)).getCoord(1);
		z2 = getVertex(cell.getVertexId(1)).getCoord(2);
		x3 = getVertex(cell.getVertexId(2)).getCoord(0);
		y3 = getVertex(cell.getVertexId(2)).getCoord(1);
		z3 = getVertex(cell.getVertexId(2)).getCoord(2);

		aux1x = x2 - x1;
		aux1y = y2 - y1;
		aux1z = z2 - z1;
		aux2x = x3 - x1;
		aux2y = y3 - y1;
		aux2z = z3 - z1;

		ncell[0] = (aux1y * aux2z) - (aux2y * aux1z);
		ncell[1] = (aux1z * aux2x) - (aux2z * aux1x);
		ncell[2] = (aux1x * aux2y) - (aux2x * aux1y);

		norm = sqrt(vnormal[0] * vnormal[0] +
			vnormal[1] * vnormal[1] +
			vnormal[2] * vnormal[2]);

		if (norm != 0)
			for (i = 0; i < 3; ++i)
				vnormal[i] /= norm;
		else
			for (i = 0; i < 3; ++i)
				vnormal[i] = 1;

		getCell(idcell).setNormal(ncell);
	}

	template<class Traits>
	void CybMesh<Traits>::updateCellVerticesNormal(int idcell)
	{
		sCell& cell = getCell(idcell);
		mfVertexStarIteratorTriSurf<Traits> its(this);
		int count, i;
		ids idvertex;
		vec3 normal, ncell;
		float norm;

		space aux1x, aux1y, aux1z, aux2x, aux2y, aux2z;
		space x1, x2, x3, y1, y2, y3, z1, z2, z3;

		x1 = getVertex(cell.getVertexId(0)).getCoord(0);
		y1 = getVertex(cell.getVertexId(0)).getCoord(1);
		z1 = getVertex(cell.getVertexId(0)).getCoord(2);
		x2 = getVertex(cell.getVertexId(1)).getCoord(0);
		y2 = getVertex(cell.getVertexId(1)).getCoord(1);
		z2 = getVertex(cell.getVertexId(1)).getCoord(2);
		x3 = getVertex(cell.getVertexId(2)).getCoord(0);
		y3 = getVertex(cell.getVertexId(2)).getCoord(1);
		z3 = getVertex(cell.getVertexId(2)).getCoord(2);

		aux1x = x2 - x1;
		aux1y = y2 - y1;
		aux1z = z2 - z1;
		aux2x = x3 - x1;
		aux2y = y3 - y1;
		aux2z = z3 - z1;

		ncell[0] = (aux1y * aux2z) - (aux2y * aux1z);
		ncell[1] = (aux1z * aux2x) - (aux2z * aux1x);
		ncell[2] = (aux1x * aux2y) - (aux2x * aux1y);

		norm = sqrt(vnormal[0] * vnormal[0] +
			vnormal[1] * vnormal[1] +
			vnormal[2] * vnormal[2]);

		if (norm != 0)
			for (i = 0; i < 3; ++i)
				vnormal[i] /= norm;
		else
			for (i = 0; i < 3; ++i)
				vnormal[i] = 1;

		getCell(idcell).setNormal(ncell);

		for (i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j)
				normal[j] = 0;
			count = 0;

			idvertex = cell.getVertexId(i);

			for (its.initialize(idvertex); its.notFinished(); ++its) {
				ncell = its->getNormal();
				for (int k = 0; k < 3; ++k)
					normal[k] += ncell[k];
				count++;
			}

			for (int l = 0; l < 3; ++l)
				normal[l] /= count;

			getVertex(cell.getVertexId(i)).setNormal(normal);
		}
	}

	template<class Traits>
	void CybMesh<Traits>::updateVertexNormalVector(ids idvertex)
	{
		mfVertexStarIteratorTriSurf<Traits> its(this);
		vec3 ncell, normalv, cell;
		int count = 0;
		int vindex;
		ids vid;

		for (its.initialize(idvertex); its.notFinished(); ++its) {
			its->updateNormalVector(*this);
			cell = its->getNormal();
			ncell[0] = cell[0];
			ncell[1] = cell[1];
			ncell[2] = cell[2];
			normalv += ncell;
			count++;
		}
		normalv /= count;
		getVertex(idvertex).setNormal(normalv);
	}

	template<class Traits>
	SPACE CybMesh<Traits>::getEdgeLength(sCell& cell, int coordinate, int edge)
	{
		if (edge == 0)
			return getVertex(cell.getVertexId(0)).getCoord(coordinate) -
			getVertex(cell.getVertexId(1)).getCoord(coordinate);
		if (edge == 1)
			return getVertex(cell.getVertexId(0)).getCoord(coordinate) -
			getVertex(cell.getVertexId(2)).getCoord(coordinate);
		if (edge == 2)
			return getVertex(cell.getVertexId(1)).getCoord(coordinate) -
			getVertex(cell.getVertexId(2)).getCoord(coordinate);
		else
			return 0;
	}

#undef IDS
#undef SPACE
}