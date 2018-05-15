#pragma once

#include "mfCell.h"

namespace mf
{
#define IDS typename mfTriCell<Traits>::ids

	/**
	*	Base class for triangular cells
	*/
	template<class Traits>
	class mfTriCell : public mfCell<3, Traits>
	{
	public:
		typedef typename Traits::ids ids;

		/** Constructor */
		mfTriCell();

		/** Destructor */
		virtual ~mfTriCell();

		/**
		*	Returns the edge id of the specified index
		*	@param index: position of edge
		*/
		ids getEdgeId(int index);

		/**
		*	Defines the edge id of the specified index
		*	@param index: position of edge
		*	@param vertex: the edge id
		*/
		void setEdgeId(int index, ids edge);

	protected:
		array<ids, 3> edges;
	};

	template<class Traits>
	mfTriCell<Traits>::mfTriCell()
	{
		mtx.lock();
		for (int i = 0; i < 3; ++i)
			edges[i] = -1;
		mtx.unlock();
	}

	template<class Traits>
	mfTriCell<Traits>::~mfTriCell()
	{

	}

	template<class Traits>
	IDS mfTriCell<Traits>::getEdgeId(int index)
	{
		assert((index >= 0) && (index < 3));

		mtx.lock();
		ids temp = edges[index];
		mtx.unlock();
		return temp;
	}

	template<class Traits>
	void mfTriCell<Traits>::setEdgeId(int index, ids edge)
	{
		assert((index >= 0) && (index < 3));

		mtx.lock();
		edges[index] = edge;
		mtx.unlock();
	}

#undef IDS
}
