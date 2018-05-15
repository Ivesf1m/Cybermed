#pragma once

#include <algorithm>
#include <forward_list>
#include "mfIterator.h"
#include "mfMesh.h"
#include "mfSing.h"

using std::find;
using std::forward_list;

namespace mf
{
#define SCELL typename mfVertexStarIteratorTriSurf<Traits>::sCell
#define IDS typename mfVertexStarIteratorTriSurf<Traits>::ids

	template<class Traits>
	class mfVertexStarIteratorTriSurf : public mfIterator<Traits>
	{
	public:
		typedef typename Traits::sVertex sVertex;
		typedef mfSing<Traits> sSing;

		/** Constructor */
		mfVertexStarIteratorTriSurf(sMesh* _mesh);

		/** Destructor */
		virtual ~mfVertexStarIteratorTriSurf();

		bool initialize(ids init);

		virtual bool finished();
		virtual bool notFinished();
		virtual bool operator++();
		sCell* operator->();
		sCell* operator*();
		virtual ids operator&();

	private:
		ids initialIndex; /* Initial cell id */
		ids iv; /* Id of vertex center to the star iteration */
		sVertex* v; /* Instance of vertex center to the star iteration */
		int values[10];
		int counter;

		/* List of cells to which the center vertex belongs to */
		forward_list<ids> list;

		/* List of flag cells to which the center vertex belongs to */
		forward_list<ids> flags;

		int is; /* Current singular id */
		int orientationStatus; /* 0 - cw, 1 - ccw */

		/**
		*	Adds neighbor of the current cell that have the center vertex of
		*	the iteration.
		*/
		void addNeighbor();
	};

	template<class Traits>
	mfVertexStarIteratorTriSurf<Traits>::mfVertexStarIteratorTriSurf(sMesh* _mesh)
		: mfIterator<Traits>(_mesh), is(-1), currentIndex(-1), initialIndex(-1),
		iv(-1), status(0), v(NULL)
	{

	}

	template<class Traits>
	mfVertexStarIteratorTriSurf<Traits>::~mfVertexStarIteratorTriSurf()
	{

	}

	template<class Traits>
	bool mfVertexStarIteratorTriSurf<Traits>::initialize(ids init)
	{
		assert(init >= 0);
		iv = init;
		v = mesh->getVertex(iv);
		status = 1;
		assert(v->inMesh());
		currentIndex = initialIndex = -1;
		currentCell = NULL;
		is = -1;
		list.clear();
		flags.clear();
		return true;
	}

	template<class Traits>
	bool mfVertexStarIteratorTriSurf<Traits>::operator++()
	{
		if (status == 1) {
			if (currentIndex < 0) {
				if (is < 0) {
					is = 0;
					currentIndex = v->getSingCell(is);
					assert(currentIndex >= 0);
					currentCell = mesh->getCell(currentIndex);
					assert(currentCell && currentCell->inMesh());
					flags.insert_after(flags.end(), currentIndex);
					addNeighbor();
					initialIndex = currentIndex;
				}
			}
			else {
				assert(currentCell->getVertexIndex(iv) >= 0);

				if (list.empty()) {
					is++;
					currentIndex = v->getSingCell(is);

					if (currentIndex >= 0) {
						list.insert_after(list.end(), currentIndex);
						flags.insert_after(flags.end(), currentIndex);
					}
					else {
						currentCell = NULL;
						currentIndex = -1;
						status = 3;
						return false;
					}
				}
				
				currentIndex = list.front();
				list.pop_front();
				currentCell = mesh->getCell(currentIndex);
				assert(currentCell && currentCell->inMesh());
				addNeighbor();
			}
			return true;
		}
		else
			return false;
	}

	template<class Traits>
	void mfVertexStarIteratorTriSurf<Traits>::addNeighbor()
	{
		ids temp;

		temp = currentCell->getMateId((currentCell->getVertexIndex(iv) + 1) % 3);
		if ((temp >= 0) && (find(flags.begin(), flags.end(), temp) == flags.end())) {
			flags.insert_after(flags.end(), temp);
			list.insert_after(list.end(), temp);
		}

		temp = currentCell->getMateId((currentCell->getVertexIndex(iv) + 2) % 3);
		if ((temp >= 0) && (find(flags.begin(), flags.end(), temp) == flags.end())) {
			flags.insert_after(flags.end(), temp);
			list.insert_after(list.end(), temp);
		}

		assert(currentCell->getVertexIndex(iv) >= 0);
	}

	template<class Traits>
	SCELL* mfVertexStarIteratorTriSurf<Traits>::operator->()
	{
		return currentCell;
	}

	template<class Traits>
	SCELL* mfVertexStarIteratorTriSurf<Traits>::operator*()
	{
		return currentCell;
	}

	template<class Traits>
	IDS mfVertexStarIteratorTriSurf<Traits>::operator&()
	{
		return currentIndex;
	}

	template<class Traits>
	bool mfVertexStarIteratorTriSurf<Traits>::finished()
	{
		return ((status == 2) || (status == 3));
	}

	template<class Traits>
	bool mfVertexStarIteratorTriSurf<Traits>::notFinished()
	{
		return ((status == 0) || (status == 1));
	}

#undef SCELL
#undef IDS
}
