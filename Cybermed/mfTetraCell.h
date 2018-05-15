#pragma once

#include "mfCell.h"

namespace mf 
{
#define IDS typename mfTetraCell<Traits>::ids

	/**
	*	Base class for tetrahedric cells
	*  Tetrahedron cell default format with vertex indices:
	*  2------3
	*  |     /|
	*  | \  / |
	*  |   /  |
	*  |  /   |
	*  | /  \ |
	*  |/     |
	*  0------1
	*	
	*	Traits must have ids
	*/
	template<class Traits>
	class mfTetraCell : public mfCell<4, Traits>
	{
	public:
		typedef typename Traits::ids ids;

		/** Constructor */
		mfTetraCell();

		/** Destructor */
		virtual ~mfTetraCell();

		/**
		*	Returns the edge id of the specified index
		*	@param index: position of edge
		*/
		ids getEdgeId(int index);

		/**
		*	Returns the mate cell id of the specified index
		*	@param index: position of mate cell
		*/
		ids getMateId(int index);

		/** 
		*	Defines the edge id of the specified index
		*	@param index: position of edge
		*	@param edge: the edge id
		*/
		void setEdgeId(int index, ids edge);

		/**
		*	Defines the mate cell id of the specified index
		*	@param index: position of mate cell
		*	@param cell: the mate cell id
		*/
		void setMateId(int index, ids cell);

		/**
		*	Return the face index to the right of the edge defined be the
		*	2 vertices.
		*	@param index1: index of the first vertex
		*	@param index2: index of the second vertex
		*	@return index of the face
		*/
		int getRightFaceIndex(ids index1, ids index2);

		/**
		*	Return the face index to the left of the edge defined be the
		*	2 vertices.
		*	@param index1: index of the first vertex
		*	@param index2: index of the second vertex
		*	@return index of the face
		*/
		int getLeftFaceIndex(ids index1, ids index2);

		/**
		*	Resets the mate cells ids, setting them all to -1
		*/
		void clearMates();

		/**
		*	Returns the position of the specified mate cell id
		*	@param cell: the mate cell id
		*/
		int getMateIndex(ids cell);

		/**
		*	Returns the opposite cell id of the specified vertex id
		*	@param cell: the vertex id
		*/
		ids getMateVertexId(ids vertex);

		/**
		*	Returns the opposite vertex id of the specified mate cell id
		*	@param cell: the mate cell id
		*/
		ids getVertexMateId(ids cell);

		/**
		*	Returns the dimension of this cell
		*/
		static inline int getDimension() { return 3; };

		/**
		*	Returns the number of vertices in this cell.
		*/
		static inline int getNumberVerticesInCell() { return 4; };

		/**
		*	Returns the number of edges in ths cell.
		*/
		static inline int getNumberEdgesInCell() { return 6; };

		/**
		*	Retuns the number of faces of this cell.
		*/
		static inline int getNumberFacesInCell() { return 4; };

	private:
		array<ids, 6> edges;
	};

	template<class Traits>
	mfTetraCell<Traits>::mfTetraCell()
	{
		edges.fill(-1);
		mates.fill(-1);
	}

	template<class Traits>
	mfTetraCell<Traits>::~mfTetraCell()
	{

	}

	template<class Traits>
	IDS mfTetraCell<Traits>::getEdgeId(int index)
	{
		assert((index >= 0) && (index < 6));

		mtx.lock();
		ids temp = edges[index];
		mtx.unlock();
		return temp;
	}

	template<class Traits>
	IDS mfTetraCell<Traits>::getMateId(int index)
	{
		assert((index >= 0) && (index < 4));

		mtx.lock();
		ids temp = mates[index];
		mtx.unlock();
		return temp;
	}

	template<class Traits>
	void mfTetraCell<Traits>::setEdgeId(int index, ids edge)
	{
		assert((index >= 0) && (index < 6));
		mtx.lock();
		edges[index] = edge;
		mtx.unlock();
	}

	template<class Traits>
	void mfTetraCell<Traits>::setMateId(int index, ids cell)
	{
		assert((index >= 0) && (index < 4));
		mtx.lock();
		mates[index] = cell;
		mtx.unlock();
	}

	template<class Traits>
	IDS mfTetraCell<Traits>::getVertexMateId(ids cell)
	{
		for (int i = 0; i < 4; ++i)
			if (getMateId(i) == cell)
				return getVertexId(i);
		return -1;
	}

	template<class Traits>
	int mfTetraCell<Traits>::getMateIndex(ids cell)
	{
		for (int i = 0; i < 4; ++i)
			if (getMateId(i) == cell)
				return i;
		return -1;
	}

	template<class Traits>
	void mfTetraCell<Traits>::clearMates()
	{
		for (int i = 0; i < 4; ++i)
			setMateId(i, -1);
	}

	template<class Traits>
	int mfTetraCell<Traits>::getRightFaceIndex(ids index1, ids index2)
	{
		assert((index1 >= 0) && (index1 <= 3));
		assert((index2 >= 0) && (index2 <= 3));
		assert(index1 != index2);

		switch (index1) {
		case 0:
			if (index2 == 1)
				return 3;
			if (index2 == 2)
				return 1;
			if (index2 == 3)
				return 2;
			break;
		case 1:
			if (index2 == 0)
				return 2;
			if (index2 == 2)
				return 3;
			if (index2 == 3)
				return 0;
			break;
		case 2:
			if (index2 == 0)
				return 3;
			if (index2 == 1)
				return 0;
			if (index2 == 3)
				return 1;
			break;
		case 3:
			if (index2 == 0)
				return 1;
			if (index2 == 1)
				return 2;
			if (index2 == 2)
				return 0;
			break;
		}

		return -1;
	}

	template<class Traits>
	int mfTetraCell<Traits>::getLeftFaceIndex(ids index1, ids index2)
	{
		assert((index1 >= 0) && (index1 <= 3));
		assert((index2 >= 0) && (index2 <= 3));
		assert(index1 != index2);

		switch (index1)
		{
		case 0:
			if (index2 == 1)
				return 2;
			if (index2 == 2)
				return 3;
			if (index2 == 3)
				return 1;
			break;
		case 1:
			if (index2 == 0)
				return 3;
			if (index2 == 2)
				return 0;
			if (index2 == 3)
				return 2;
			break;
		case 2:
			if (index2 == 0)
				return 1;
			if (index2 == 1)
				return 3;
			if (index2 == 3)
				return 0;
			break;
		case 3:
			if (index2 == 0)
				return 2;
			if (index2 == 1)
				return 0;
			if (index2 == 2)
				return 1;
			break;
		}
		return -1;
	}

#undef IDS
}


