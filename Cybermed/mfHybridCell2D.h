#pragma once

#include "mfCell.h"

namespace mf
{
#define IDS typename mfHybridCell2D<Traits>::ids

	template<class Traits>
	class mfHybridCell2D : public mfCell<4, Traits>
	{
		public:
			/* Constructor */
			mfHybridCell2D();

			/* Destructor */
			~mfHybridCell2D();

			/**
			*	Returns the edge id of the specified index
			*	@param index: position of edge
			*/
			ids getEdgeId(int index);

			/**
			*	Defines the edge id of the specified index
			*	@param index: position of edge
			*	@param edge: the edge id
			*/
			void setEdgeId(int index, ids edge);

			/**
			*	Returns the number of vertices in this cell
			*/
			int getNumberVerticesInCell();

			/**
			*	Returns the number of edges in this cell
			*/
			int getNumberEdgesInCell();

			static inline int getDimension() { return 2; };

		private:
			array<ids, 4> edges;
			array<ids, 4> mates;
	};

	template<class Traits>
	mfHybridCell2D<Traits>::mfHybridCell2D()
		: edges(-1), mates(-1)
	{

	}

	template<class Traits>
	mfHybridCell2D<Traits>::~mfHybridCell2D()
	{

	}

	template<class Traits>
	int mfHybridCell2D<Traits>::getNumberEdgesInCell()
	{
		if (edges[3] == -1)
			return 3;
		else
			return 4;
	}

	template<class Traits>
	int mfHybridCell2D<Traits>::getNumberVerticesInCell()
	{
		if (vertices[3] == -1)
			return 3;
		else
			return 4;
	}

	template<class Traits>
	IDS mfHybridCell2D<Traits>::getEdgeId(int index)
	{
		assert((index >= 0) && (index < getNumberEdgesInCell()));
		mtx.lock();
		ids temp = edges[index];
		mtx.unlock();
		return temp;
	}

	template<class Traits>
	void mfHybridCell2D<Traits>::setEdgeId(int index, ids edge)
	{
		assert((index >= 0) && (index < getNumberEdgesInCell()));
		mtx.lock();
		edges[index] = edge;
		mtx.unlock();
	}

#undef IDS
}