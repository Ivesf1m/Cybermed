#pragma once

#include "mfCell.h"

namespace mf
{
#define IDS typename mfQuadCell<Traits>::ids

	/**
	*	Base class of the quadrilateral cell
	*
	*	Traits must have typename ids
	*/
	template<class Traits>
	class mfQuadCell : public mfCell<4, Traits>
	{
		public:
			typedef typename Traits::ids ids;

			/** Constructor */
			mfQuadCell();

			/** Destructor */
			virtual ~mfQuadCell();

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
			*	Returns the dimension of this cell
			*/
			static inline int getDimension() { return 2 };

		private:
			array<ids, 4> edges;
			//array<ids, 3> mates;
	};

	template<class Traits>
	mfQuadCell<Traits>::mfQuadCell()
		: edges(-1), mates(-1)
	{

	}

	template<class Traits>
	mfQuadCell<Traits>::~mfQuadCell()
	{

	}

	template<class Traits>
	IDS mfQuadCell<Traits>::getEdgeId(int index)
	{
		assert((index >= 0) && (index < 4));
		mtx.lock();
		ids temp = edges[index];
		mtx.unlock();
		return temp;
	}

	template<class Traits>
	void mfQuadCell<Traits>::setEdgeId(int index, ids edge)
	{
		assert((index >= 0) && (index < 4));
		mtx.lock();
		edges[index] = edge;
		mtx.unlock();
	}

#undef IDS
}