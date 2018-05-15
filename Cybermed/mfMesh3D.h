#pragma once

#include "mfBase.h"
#include <vector>

using std::vector;

namespace mf
{
#define IDS typename mfMesh3D<Traits>::ids
#define SVERTEX typename mfMesh3D<Traits>::sVertex
#define SCELL typename mfMesh3D<Traits>::sCell
#define SEDGE typename mfMesh3D<Traits>::sEdge
#define SFACE typename mfMesh3D<Traits>::sFace

	/**
	*	Mesh Class
	*	This class must be initialized with element types (vertices and cells)
	*	and the operation class.
	*	Operation class depends on the vertices and cells dimensions.
	*	Traits must have: space, ids, sVertex, sCell, sEdge , sOper
	*/
	template<class Traits>
	class mfMesh3D
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sEdge sEdge;
		typedef typename Traits::sFace sFace;
		typedef typename Traits::sOper sOper;
		typedef typename Traits::sTopology sTopology;

		/** Constructor */
		mfMesh3D();

		/** Destructor */
		~mfMesh3D();

		/**
		*	Adds a vertex in mesh
		*	@param coords: vector with the vertex coordinates
		*	@return: the vertex id in mesh
		*/
		ids addVertex(vector<space>& coords);

		/**
		*	Deletes a vertex from mesh
		*	@param idvertex: the id of the vertex to be deleted
		*	@return: true if the vertex was deleted
		*/
		bool delVertex(ids idvertex);

		/**
		*	Returns the number of vertices in mesh
		*/
		ids getNumberOfVertices();

		/**
		*	Returns the greater vertex id
		*/
		ids getVertexMaxId();

		/**
		*	Returns a reference to a vertex
		*	@param idvertex: the vertex id
		*/
		sVertex& getVertex(ids idvertex);

		/**
		*	Return true if the specified vertex id in mesh
		*/
		bool isVertex(ids idvertex);

		/**
		*	Adds a cell in mesh
		*	@param idvertices: vector with vertices ids of the cell
		*	@return: the cell id in mesh
		*/
		ids addCell(vector<ids>& idvertices);

		/**
		*	Deletes a cell from mesh
		*	@param idcell: the id of the cell to be deleted
		*	@return: true if the cell was deleted
		*/
		bool delCell(ids idcell);

		/**
		*	Returns the number of cells in mesh
		*/
		ids getNumberOfCells();

		/**
		*	Returns the greater cell id
		*/
		ids getCellMaxId();

		/**
		*	Returns a reference to a cell
		*	@param idcell: the cell id
		*/
		sCell& getCell(ids idcell);

		/**
		*	Returns true if cell is in mesh
		*	@param idcell: the cell id
		*/
		bool isCell(ids idcell);

		/**
		*	Adds a vertex in a specified position in mesh
		*	@param idvertex: position of vertex
		*	@param coords: vector with the vertex coordinates
		*	@return: the vertex id in mesh
		*/
		void addVertex(ids idvertex, vector<space>& coords);

		/**
		*	Adds a cell in a specified position in mesh
		*	@param idcell: position of cell
		*	@param idvertices: vector with the vertex ids
		*	@return: the cell id in mesh
		*/
		void addCell(ids idcell, vector<ids>& idvertices);
		ids addCell(vector<ids>& idvertices, vector<ids>& idops);

		/**
		*	Adds an edge in mesh
		*	@return: the edge id
		*/
		ids addEdge();

		/**
		*	Returns the number of edges in mesh
		*/
		ids getNumberOfEdges();

		/**
		*	Returns the greater edge id
		*/
		ids getEdgeMaxId();

		/**
		*	Returns a reference to an edge
		*	@param idedge: the edge id
		*/
		sEdge& getEdge(ids idedge);

		/**
		*	Returns true if the edge is in mesh
		*	@param idedge: the edge id
		*/
		bool isEdge(ids idedge);

		void freeCell(ids idcell);
		void freeVertex(ids idvertex);
		void freeEdge(ids idedge);

	private:
		sOper oper;
		vector<sVertex> vertices;
		vector<sCell> cells;
		vector<sEdge> edges;
		vector<sFace> faces;

		//Mutexes
		mutex verticesMutex;
		mutex cellsMutex;
		mutex edgesMutex;
	};

	template<class Traits>
	mfMesh3D<Traits>::mfMesh3D()
	{

	}

	template<class Traits>
	mfMesh3D<Traits>::~mfMesh3D()
	{

	}

	/* Vertex functions */
	template<class Traits>
	IDS mfMesh3D<Traits>::addVertex(vector<space>& coords)
	{
		ids idvertex;
		verticesMutex.lock();
		idvertex = vertices.size();
		verticesMutex.unlock();
		addVertex(idvertex, coords);
		return idvertex;
	}

	template<class Traits>
	void mfMesh3D<Traits>::addVertex(ids idvertex, vector<space>& coords)
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
	bool mfMesh3D<Traits>::delVertex(ids idvertex)
	{
		assert(idvertex >= 0);

		if (idvertex >= vertices.size())
			return false;
		else if (vertices[idvertex].getNumberOfSings() == 0) {
			verticesMutex.lock();
			vertices[idvertex](sVertex());
			verticesMutex.unlock();
			return true;
		}
		else
			return false;
	}

	template<class Traits>
	IDS mfMesh3D<Traits>::getNumberOfVertices()
	{
		verticesMutex.lock();
		ids temp = vertices.size();
		verticesMutex.unlock();
		return temp;
	}

	template<class Traits>
	IDS mfMesh3D<Traits>::getVertexMaxId()
	{
		return getNumberOfVertices() - 1;
	}

	template<class Traits>
	SVERTEX& mfMesh3D<Traits>::getVertex(ids idvertex)
	{
		assert((idvertex >= 0) && (idvertex < vertices.size()));
		return vertices[idvertex];
	}

	template<class Traits>
	bool mfMesh3D<Traits>::isVertex(ids idvertex)
	{
		assert((idvertex >= 0) && (idvertex < vertices.size()));
		return vertices[idvertex].inMesh();
	}

	/** Cell functions */
	template<class Traits>
	IDS mfMesh3D<Traits>::addCell(vector<ids>& idvertices)
	{
		cellsMutex.lock();
		int idcell = cells.size();
		cellsMutex.unlock();
		oper.addCell(idcell, idvertices);
		return idcell;
	}

	template<class Traits>
	void mfMesh3D<Traits>::addCell(ids idcell, vector<ids>& idvertices)
	{
		assert(idCell > 0);
		oper.addCell(idcell, idvertices);
	}

	template<class Traits>
	bool mfMesh3D<Traits>::delCell(ids idcell)
	{
		assert((idcell >= 0) && (idcell <= getCellMaxId()));
		oper.delCell(idcell);
		cellsMutex.lock();
		cells[idcell](sCell());
		cellsMutex.unlock();
		return true;
	}

	template<class Traits>
	IDS mfMesh3D<Traits>::getNumberOfCells()
	{
		cellsMutex.lock();
		ids temp = cells.size();
		cellsMutex.unlock();
		return temp;
	}

	template<class Traits>
	IDS mfMesh3D<Traits>::getCellMaxId()
	{
		return getNumberOfCells() - 1;
	}

	template<class Traits>
	SCELL& mfMesh3D<Traits>::getCell(ids idcell)
	{
		assert(idcell >= 0 && idcell < getNumberOfCells());
		return cells[idcell];
	}

	template<class Traits>
	bool mfMesh3D<Traits>::isCell(ids idcell)
	{
		assert(idcell >= 0 && idcell < getNumberOfCells());
		return cells[idcell].inMesh();
	}

	template<class Traits>
	IDS mfMesh3D<Traits>::addCell(vector<ids>& idvertices, vector<ids>& idops)
	{
		ids idcell;

		cellsMutex.lock();
		idcell = cells.size();
		cellsMutex.unlock();
		oper.addCell(idcell, idvertices, idops);
		return idcell;
	}

	/** Edge functions */
	template<class Traits>
	IDS mfMesh3D<Traits>::addEdge()
	{
		ids idedge;
		edgesMutex.lock();
		idedge = edges.size();
		edgesMutex.unlock();
		return idedge;
	}

	template<class Traits>
	IDS mfMesh3D<Traits>::getNumberOfEdges()
	{
		ids temp;
		edgesMutex.lock();
		temp = edges.size();
		edgesMutex.unlock();
		return temp;
	}

	template<class Traits>
	IDS mfMesh3D<Traits>::getEdgeMaxId()
	{
		return getNumberOfEdges() - 1;
	}

	template<class Traits>
	SEDGE& mfMesh3D<Traits>::getEdge(ids idedge)
	{
		assert(idedge >= 0 && idedge < getNumberOfEdges());
		return edges[idedge];
	}

	template<class Traits>
	bool mfMesh3D<Traits>::isEdge(ids idedge)
	{
		assert(idedge >= 0 && idedge < getNumberOfEdges());
		return edges[idedge].inMesh();
	}

#undef IDS
#undef SVERTEX
#undef SCELL
#undef SEDGE
}