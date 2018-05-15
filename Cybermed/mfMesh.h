#pragma once

#include <iostream>
#include "mfBase.h"
#include <vector>

using std::vector;

namespace mf
{
#define IDS typename mfMesh<Traits>::ids
#define SVERTEX typename mfMesh<Traits>::sVertex
#define SCELL typename mfMesh<Traits>::sCell
#define SEDGE typename mfMesh<Traits>::sEdge

	/**
	*	Mesh class.
	*	This class must be initialized with elements types (vertices and cells) and the operation class.
	*	Operation class depends of the vertices and cells dimensions.
	*	_Traits must have: space, ids, sVertex, sCell, sEdge , sOper
	*/
	template <class Traits>
	class mfMesh
	{
		public:
			typedef typename Traits::space space;
			typedef typename Traits::ids ids;
			typedef typename Traits::sVertex sVertex;
			typedef typename Traits::sCell sCell;
			typedef typename Traits::sEdge sEdge;
			typedef typename Traits::sOper sOper;
			typedef typename Traits::sTopology sTopology;

			/**
			*	Constructor
			*/
			mfMesh();

			/**
			*	Destructor
			*/
			~mfMesh();

			/**
			*	Adds a vertex to mesh.
			*/
			ids addVertex(vector<space>& coords);

			/**
			*	Deletes a vertex to mesh
			*/
			bool delVertex(ids idVertex);

			/**
			*	Returns the number of vertices in mesh.
			*/
			ids getNumberOfVertices();

			/**
			*	Returns the maximum vertex index.
			*/
			ids getVertexMaxId();

			/**
			*	Returns a reference to the vertex.
			*/
			sVertex& getVertex(ids idVertex);

			/**
			*	Returns true if the specified vertex id is a vertex in mesh.
			*/
			bool isVertex(ids idVertex);

			/**
			*	Adds a cell in mesh
			*/
			ids addCell(vector<ids>& idVertices);

			/**
			*	Deletes a cell in mesh
			*/
			bool delCell(ids idCell);

			/**
			*	Returns the number of cells in mesh.
			*/
			ids getNumberOfCells();

			/**
			*	Returns the maximum cell index.
			*/
			ids getCellMaxId();

			/**
			*	Returns a reference to the cell.
			*/
			sCell& getCell(ids idCell);

			/**
			*	Returns true if the specified cell is a cell in mesh.
			*/
			bool isCell(ids idCell);

			/**
			*	Adds a vertex in the specified position in mesh.
			*/
			void addVertex(ids idVertex, vector<space>& coords);

			/**
			*	Adds a cell in the specified position in mesh.
			*/
			void addCell(ids idCell, vector<ids>& idVertices);
			ids addCell(vector<ids>& idVertices, vector<ids>& idOps);

			/**
			*	Adds an edge to mesh.
			*/
			ids addEdge();

			/**
			*	Returns the number of edges in mesh.
			*/
			ids getNumberOfEdges();

			/**
			*	Returns the maximum edge index. 
			*/
			ids getEdgeMaxId();

			/**
			*	Returns a reference to the edge.
			*/
			sEdge& getEdge(ids idEdge);

			/**
			*	Returns true if the specified edge id is an edge in mesh.
			*/
			bool isEdge(ids idEdge);


		protected:
			sOper oper;
			vector<sVertex> vertices;
			vector<sCell> cells;
			vector<sEdge> edges;

			//Mutexes
			mutex verticesMutex;
			mutex cellsMutex;
			mutex edgesMutex;
	};

	template<class Traits>
	mfMesh<Traits>::mfMesh()
	{

	}

	template<class Traits>
	mfMesh<Traits>::~mfMesh()
	{

	}

	/* Vertex functions */

	template<class Traits>
	IDS mfMesh<Traits>::addVertex(vector<space>& coords)
	{
		ids idvertex;
		verticesMutex.lock();
		idvertex = vertices.size();
		verticesMutex.unlock();
		addVertex(idvertex, coords);
		return idvertex;
	}

	template<class Traits>
	void mfMesh<Traits>::addVertex(ids idVertex, vector<space>& coords)
	{
		vertices.lock();
		if (idvertex >= vertices.size())
			while (vertices.size() < idvertex)
				vertices.push_back(sVertex());

		for (int i = 0; i < sVertex::getDimension(); ++i)
			vertices[idvertex].setCoord(i, coords[i]);

		vertices.unlock();
	}

	template<class Traits>
	bool mfMesh<Traits>::delVertex(ids idVertex)
	{
		assert(idVertex >= 0 && idVertex < vertices.size());

		bool temp;

		if (vertices[idVertex].getNumberOfSings() == 0) {
			verticesMutex.lock();
			vertices[idvertex](sVertex());
			verticesMutex.unlock();
			temp = true;
		}
		else
			temp = false;
		return temp;
	}

	template<class Traits>
	IDS mfMesh<Traits>::getNumberOfVertices()
	{
		ids temp;
		verticesMutex.lock();
		temp = vertices.size();
		verticesMutex.unlock();
		return temp;
	}

	template<class Traits>
	IDS mfMesh<Traits>::getVertexMaxId()
	{
		ids temp;
		verticesMutex.lock();
		temp = vertices.size() - 1;
		verticesMutex.unlock();
		return temp;
	}

	template<class Traits>
	SVERTEX& mfMesh<Traits>::getVertex(ids idVertex)
	{
		assert(idVertex >= 0 && idVertex < getVertexMaxId());
		return vertices[idVertex];
	}

	template<class Traits>
	bool mfMesh<Traits>::isVertex(ids idVertex)
	{
		assert(idVertex >= 0 && idVertex < getVertexMaxId());
		return vertices[idVertex].inMesh();
	}

	/* Cells */
	template<class Traits>
	IDS mfMesh<Traits>::addCell(vector<ids>& idVertices)
	{
		ids temp;
		cellsMutex.lock();
		temp = cells.size();
		oper.addCell(temp, idVertices);
		cellsMutex.unlock();
		return temp;
	}

	template<class Traits>
	void mfMesh<Traits>::addCell(ids idCell, vector<ids>& idVertices)
	{
		assert(idCell > 0);
		oper.addCell(idCell, idVertices);
	}

	template<class Traits>
	bool mfMesh<Traits>::delCell(ids idCell)
	{
		assert(idCell >= 0 && idCell <= getCellMaxId());
		oper.delCell(idCell);
		cellsMutex.lock();
		cells.erase(cells.begin() + idCell);
		cellsMutex.unlock();
		return true;
	}

	template<class Traits>
	IDS mfMesh<Traits>::getNumberOfCells()
	{
		ids temp;
		cellsMutex.lock();
		temp = cells.size();
		cellsMutex.unlock();
		return temp;
	}

	template<class Traits>
	IDS mfMesh<Traits>::getCellMaxId()
	{
		return getNumberOfCells() - 1;
	}

	template<class Traits>
	SCELL& mfMesh<Traits>::getCell(ids idCell)
	{
		assert(idCell >= 0 && idCell <= getCellMaxId());
		return cells[idCell];
	}

	template<class Traits>
	bool mfMesh<Traits>::isCell(ids idCell) 
	{
		assert(idCell >= 0 && idCell <= getCellMaxId());
		return cells[idCell].inMesh();
	}

	template<class Traits>
	IDS mfMesh<Traits>::addCell(vector<ids>& idVertices, vector<ids>& idOps)
	{
		ids temp = getNumberOfCells()
		oper.addCell(temp, idVertices, idOps);
		return temp;
	}

	/* Edges */
	template<class Traits>
	IDS mfMesh<Traits>::addEdge()
	{
		ids idEdge;
		edgesMutex.lock();
		idEdge = edges.getNumberOfEdges();
		edgesMutex.unlock();
		return idEdge;
	}

	template<class Traits>
	IDS mfMesh<Traits>::getNumberOfEdges()
	{
		ids temp;
		edgesMutex.lock();
		temp = edges.size();
		edgesMutex.unlock();
		return temp;
	}

	template<class Traits>
	IDS mfMesh<Traits>::getEdgeMaxId()
	{
		return getNumberOfEdges() - 1;
	}

	template<class Traits>
	SEDGE& mfMesh<Traits>::getEdge(ids idEdge)
	{
		assert(idEdge >= 0 && idEdge < getNumberOfEdges());
		return edges[idEdge];
	}

	template<class Traits>
	bool mfMesh<Traits>::isEdge(ids idEdge)
	{
		assert(idEdge >= 0 && idEdge < getNumberOfEdges());
		return edges[idEdge].inMesh();
	}

#undef IDS
#undef SVERTEX
#undef SCELL
#undef SEDGE

}