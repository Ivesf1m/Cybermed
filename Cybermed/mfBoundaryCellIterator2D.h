#pragma once

#include "mfIterator.h"

namespace mf
{
#define SCELL typename mfBoundaryCellCIterator2D<Traits>::sCell
#define IDS typename mfBoundaryCellCIterator2D<Traits>::ids

	template<class Traits>
	class mfBoundaryCellIterator2D : public mfIterator<Traits>
	{
		public:
			typedef typename Traits::sCell sCell;
			typedef typename Traits::ids ids;
			typedef typename Traits::sMesh sMesh;

			/**
			*	Constructor
			*/
			mfBoundaryCellIterator2D(sMesh* mesh);

			/**
			*	Destructor
			*/
			virtual ~mfBoundaryCellIterator2D();

			bool initialize(ids init, int edge = -1);
			virtual bool finished();
			virtual bool notFinished();
			virtual bool operator++();
			bool operator--();

			sCell* operator->();
			sCell* operator*();

			virtual ids operator&();

			int getEdge();

		private:
			ids mark;
			int mark2;
	};	

	template<class Traits>
	mfBoundaryCellIterator2D<Traits>::mfBoundaryCellIterator2D(sMesh* mesh)
		: mfIterator<Traits>(mesh), status(0), currentIndex(-1), current(-1),
		mark(-1), mark2(-1), currentCell(NULL)
	{

	}

	template<class Traits>
	mfBoundaryCellIterator2D<Traits>::~mfBoundaryCellIterator2D()
	{

	}

	template<class Traits>
	bool mfBoundaryCellIterator2D<Traits>::initialize(ids init, int edge)
	{
		assert(init >= 0);

		currentIndex = init;
		mark = currentIndex;

		currentCell = this->mesh->getCell(currentIndex);

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

		mark2 = current;
		status = 1;
		return true;
	}

	template<class Traits>
	bool mfBoundaryCellIterator2D<Traits>::operator++()
	{
		sCell* temp = currentCell;
		int tempEdge = current;
		int tempIndex = currentIndex;
		int prevIndex;

		if ((status == 1) || (status == 2)) {
			while (temp->getMateId((tempEdge + 1) % 3) >= 0) {
				prevIndex = tempIndex;
				tempIndex = temp->getMateId((tempEdge + 1) % 3);
				temp = this->mesh->getCell(tempIndex);
				tempEdge = temp->getMateIndex(prevIndex);
			}

			current = (tempEdge + 1) % 3;
			currentCell = tempCell;
			currentIndex = tempIndex;

			if ((currentIndex == mark) && (current == mark2)) {
				status = 3;
				return false;
			}
		}
		else
			return false;

		return true;
	}

	template<class Traits>
	bool mfBoundaryCellIterator2D<Traits>::operator--()
	{
		sCell* temp = currentCell;
		int tempEdge = current;
		int tempIndex = currentIndex;
		int prevIndex;

		if ((status == 1) || (status == 3)) {
			while (temp->getMateId((tempEdge + 2) % 3) >= 0) {
				prevIndex = tempIndex;
				tempIndex = temp->getMateId((tempEdge + 2) % 3);
				temp = this->mesh->getCell(tempIndex);
				tempEdge = temp->getMateIndex(prevIndex);
			}

			current = (tempEdge + 2) % 3;
			currentCell = tempCell;
			currentIndex = tempIndex;

			if ((currentIndex == mark) && (current == mark2)) {
				status = 2;
				return false;
			}
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
