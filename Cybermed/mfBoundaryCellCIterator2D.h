#pragma once

#include "mfIterator.h"

namespace mf
{

#define SCELL typename mfBoundaryCellCIterator2D<Traits>::sCell
#define IDS typename mfBoundaryCellCIterator2D<Traits>::ids

	template<class Traits>
	class mfBoundaryCellCIterator2D : public mfIterator<Traits>
	{
		public:
			typedef typename Traits::sCell sCell;
			typedef typename Traits::ids ids;
			typedef typename Traits::sMesh sMesh;

			/**
			*	Constructor
			*/
			mfBoundaryCellCIterator2D(sMesh* mesh);

			/**
			*	Destructor
			*/
			virtual ~mfBoundaryCellCIterator2D();

			bool initialize(ids init, int edge = -1);
			virtual bool finished();
			virtual bool notFinished();
			virtual bool operator++();
			bool operator--();

			sCell* operator->();
			sCell* operator*();

			virtual ids operator&();

			int getEdge();
	};

	template<class Traits>
	mfBoundaryCellCIterator2D<Traits>::mfBoundaryCellCIterator2D(sMesh* mesh) 
		: mfIterator<Traits>(mesh), status(0), currentId(-1), current(-1),
		currentCell(NULL)
	{

	}

	template<class Traits>
	mfBoundaryCellCIterator2D<Traits>::~mfBoundaryCellCIterator2D()
	{

	}

	template<class Traits>
	bool mfBoundaryCellCIterator2D<Traits>::initialize(ids init, int edge)
	{
		assert(init >= 0);
		currentId = init;
		currentCell = mesh->getCell(currentId);

		assert(currentCell->inMesh());

		if (edge < 0) {
			if (currentCell->getMateId(0) < 0)
				current = 0;
			else if (currentCell->getMateId(1) < 0)
				current = 1;
			else if (currentCell->getMateId(2) < 0)
				current = 2;
		}
		else
			current = edge;

		status = 1;
		return true;
	}

	template<class Traits>
	bool mfBoundaryCellCIterator2D<Traits>::operator++()
	{
		sCell* tempCell = currentCell;
		int tempCurrent = current;
		int iant;
		int tempCellId = currentId;

		if (status == 1) {
			while (tempCell->getMateId((tempCurrent + 1) % 3) >= 0) {
				iant = tempCellId;
				tempCellId = tempCell->getMateId((tempCurrent + 1) % 3);
				tempCell = mesh->getCell(tempCellId);
				tempCurrent = tempCell->getMateIndex(iant);
			}
			current = (tempCurrent + 1) % 3;
			currentCell = tempCell;
			currentId = tempCellId
		}
		else
			return false;
		return true;
	}

	template<class Traits>
	bool mfBoundaryCellCIterator2D<Traits>::operator--()
	{
		sCell* tempCell = currentCell;
		int tempCurrent = current;
		int iant;
		int tempCellId = currentId;

		if (status == 1) {
			while (tempCell->getMateId((tempCurrent + 1) % 3) >= 0) {
				iant = tempCellId;
				tempCellId = tempCell->getMateId((tempCurrent + 2) % 3);
				tempCell = mesh->getCell(tempCellId);
				tempCurrent = tempCell->getMateIndex(iant);
			}

			current = (tempCurrent + 2) % 3;
			currentCell = tempCell;
			currentId = tempCellId;
		}
		else
			return false;
		return true;
	}

	template<class Traits>
	SCELL* mfBoundaryCellCIterator2D<Traits>::operator->()
	{
		return currentCell;
	}

	template<class Traits>
	SCELL* mfBoundaryCellCIterator2D<Traits>::operator*()
	{
		return currentCell;
	}

	template<class Traits>
	IDS mfBoundaryCellCIterator2D<Traits>::operator&()
	{
		return currentId;
	}

	template<class Traits>
	bool mfBoundaryCellCIterator2D<Traits>::finished()
	{
		return ((status == 2) || (status == 3));
	}

	template<class Traits>
	bool mfBoundaryCellCIterator2D<Traits>::notFinished()
	{
		return ((status == 0) || (status == 1));
	}

	template<class Traits>
	int mfBoundaryCellCIterator2D<Traits>::getEdge()
	{
		if (status != 0)
			return current;
		return -1;
	}

#undef SCELL
#undef IDS
}