#pragma once

#include "mfIterator.h"
#include "mfMesh.h"
#include "mfSing.h"

namespace mf
{
#define SCELL typename mfVertexStarIterator2D<Traits>::sCell
#define IDS typename mfVertexStarIterator2D<Traits>::ids

	template<class Traits>
	class mfVertexStarIterator2D : public mfIterator<Traits>
	{
	public:
		typedef typename Traits::sVertex sVertex;
		typedef mfSing<Traits> sSing;

		/** Constructor */
		mfVertexStarIterator2D(sMesh* _mesh);

		/** Destructor */
		virtual ~mfVertexStarIterator2D();

		bool initialize(ids init);

		virtual bool finished();
		virtual bool notFinished();
		virtual bool operator++();
		sCell* operator->();
		sCell* operator*();
		virtual ids operator&();

	private:
		ids initialIndex;
		ids iv;
		sVertex* v;
		int is;
	};

	template<class Traits>
	mfVertexStarIterator2D<Traits>::mfVertexStarIterator2D(sMesh* _mesh)
		: mfIterator<Traits>(_mesh), is(-1), currentIndex(-1), initialIndex(-1),
		iv(-1), status(0), v(NULL)
	{

	}

	template<class Traits>
	mfVertexStarIterator2D<Traits>::~mfVertexStarIterator2D()
	{

	}

	template<class Traits>
	bool mfVertexStarIterator2D<Traits>::initialize(ids init)
	{
		assert(init >= 0);
		iv = init;
		v = mesh->getVertex(iv);
		status = 1;
		assert(v->inMesh());
		currentIndex = initialIndex = -1;
		currentCell = NULL;
		is = -1;
		return true;
	}

	template<class Traits>
	bool mfVertexStarIterator2D<Traits>::operator++()
	{
		if (status == 1) {
			if (currentIndex < 0) {
				if (is < 0) {
					is = 0;
					currentIndex = v->getSingCell(is);
					currentCell = mesh->getCell(currentIndex);
					assert((currentCell) && (currentCell->inMesh()));
					initialIndex = currentIndex;
				}
			}
			else {
				assert(currentCell->getVertexIndex(iv) >= 0);
				currentIndex = currentCell->getMateId((currentCell->
					getVertexIndex(iv) + 2) % 3);

				if (currentIndex >= 0) {
					currentCell = mesh->getCell(currentIndex);
					assert(currentCell->getVertexIndex(iv) >= 0);

					if (currentIndex == initialIndex) {
						currentCell = NULL;
						currentIndex = -1;
						status = 3;
						return false;
					}
				}
				else {
					is++;
					currentIndex = v->getSingCell(is);
					if (currentIndex >= 0) {
						currentCell = mesh->getCell(currentIndex);
						assert((currentCell) && (currentCell->inMesh()));
					}
					else {
						currentCell = NULL;
						currentIndex = -1;
						status = 3;
						return false;
					}
				}
			}
			return true;
		}
		else
			return false;
	}

	template<class Traits>
	SCELL* mfVertexStarIterator2D<Traits>::operator->()
	{
		return currentCell;
	}

	template<class Traits>
	SCELL* mfVertexStarIterator2D<Traits>::operator*()
	{
		return currentCell;
	}

	template<class Traits>
	IDS mfVertexStarIterator2D<Traits>::operator&()
	{
		return currentIndex;
	}

	template<class Traits>
	bool mfVertexStarIterator2D<Traits>::finished()
	{
		return ((status == 2) || (status == 3));
	}

	template<class Traits>
	bool mfVertexStarIterator2D<Traits>::notFinished()
	{
		return ((status == 0) || (status == 1));
	}

#undef SCELL
#undef IDS
}