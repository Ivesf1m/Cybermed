#pragma once

#include "mfIterator.h"

namespace mf
{
#define SCELL typename mfBoundaryCellCIterator2D<Traits>::sCell
#define IDS typename mfBoundaryCellCIterator2D<Traits>::ids

	template<class Traits>
	class mfCellsIterator : public mfIterator<Traits>
	{
		public:
			typedef typename Traits::sCell sCell;
			typedef typename Traits::ids ids;
			typedef typename Traits::sMesh sMesh;

			/**
			*	Constructor
			*/
			mfCellsIterator(sMesh* mesh);

			/**
			*	Destructor
			*/
			virtual ~mfCellsIterator();

			bool initialize(ids init = 0, int edge = -1);
			virtual bool finished();
			virtual bool notFinished();
			virtual bool operator++();
			bool operator--();

			sCell* operator->();
			sCell* operator*();

			virtual ids operator&();

		private:
			ids initialIndex;
	};

	template<class Traits>
	mfCellsIterator<Traits>::mfCellsIterator(sMesh* mesh)
		: mfIterator<Traits>(mesh), currentIndex(-1), initialIndex(-1),
		currentCell(NULL), status(0)
	{

	}

	template<class Traits>
	mfCellsIterator<Traits>::~mfCellsIterator()
	{

	}

	template<class Traits>
	bool mfCellsIterator<Traits>::initialize(ids init, int edge)
	{
		assert(init >= 0);

		initialIndex = init;
		status = 1;
		currentIndex = -1;
		currentCell = NULL;
		return ++(*this);
	}

	template<class Traits>
	bool mfCellsIterator<Traits>::operator++()
	{
		if (status == 1) {
			if (mesh->getNumberOfCells() > 0) {
				if (currentIndex < 0) {
					if (initialIndex > mesh->getCellMaxId()) {
						status = 3;
						return false;
					}
					currentIndex = initialIndex;
				}
				else
					currentIndex++;

				currentCell = mesh->getCell(currentIndex);
				while ((!currentCell->inMesh()) &&
					(currentIndex + 1 <= mesh->getCellMaxId())) {
					currentIndex++;
					currentCell = mesh->getCell(currentIndex);
				}

				if (!currentCell->inMesh()) {
					status = 3;
					return false;
				}

				assert(currentCell);
				return true;
			}
			else {
				status = 3;
				return false;
			}
		}
		else
			return false;
	}

	template<class Traits>
	bool mfCellsIterator<Traits>::operator--()
	{
		if (status == 1) {
			if (mesh->getNumberOfCells() > 0) {
				if (currentIndex < 0) {
					if (initialIndex > mesh->getCellMaxId())
						return false;
					currentIndex = initialIndex;
				}
				else {
					if (currentIndex == 0) {
						status = 2;
						return false;
					}
					else
						currentIndex--;
				}

				currentCell = mesh->getCell(currentIndex);
				while ((!currentCell->inMesh()) && (currentIndex - 1 >= 0)) {
					currentIndex--;
					currentCell = mesh->getCell(currentIndex);
				}

				if (!currentCell->inMesh()) {
					status = 2;
					return false;
				}

				assert(currentCell);
				return true;
			}
			else {
				status = 2;
				return false;
			}
		}
		else
			return false;
	}

	template<class Traits>
	SCELL* mfCellsIterator<Traits>::operator->()
	{
		return currentCell();
	}

	template<class Traits>
	SCELL* mfCellsIterator<Traits>::operator*()
	{
		return currentCell();
	}

	template<class Traits>
	IDS mfCellsIterator<Traits>::operator&()
	{
		return currentIndex;
	}

	template<class Traits>
	bool mfCellsIterator<Traits>::finished()
	{
		return ((status == 2) || (status == 3));
	}

	template<class Traits>
	bool mfCellsIterator<Traits>::notFinished()
	{
		return ((status == 0) || (status == 1));
	}

#undef SCELL
#undef IDS
}