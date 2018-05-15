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
#define SCELL typename mfVertexStarIterator3D<Traits>::sCell
#define IDS typename mfVertexStarIterator3D<Traits>::ids

	template<class Traits>
	class mfVertexStarIterator3D : public mfIterator<Traits>
	{
	public:
		typedef typename Traits::sVertex sVertex;
		typedef mfSing<Traits> sSing;

		/** Constructor */
		mfVertexStarIterator3D(sMesh* _mesh);

		/** Destructor */
		virtual ~mfVertexStarIterator3D();

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
		forward_list<ids> list;
		forward_list<ids> flags;
		int is;

		void addNeighbor();
	};

	template<class Traits>
	mfVertexStarIterator3D<Traits>::mfVertexStarIterator3D(sMesh* _mesh)
		: mfIterator<Traits>(_mesh), is(-1), currentIndex(-1), initialIndex(-1),
		iv(-1), status(0), v(NULL)
	{

	}

	template<class Traits>
	mfVertexStarIterator3D<Traits>::~mfVertexStarIterator3D()
	{

	}

	template<class Traits>
	bool mfVertexStarIterator3D<Traits>::initialize(ids init)
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
	bool mfVertexStarIterator3D<Traits>::operator++()
	{
		if (status == 1) {
			if (currentIndex < 0) {
				if (is < 0) {
					is = 0;
					currentIndex = v->getSingCell(is);
					assert(currentIndex >= 0);
					currentCell = mesh->getCell(currentIndex);
					assert(currentCell && (currentCell->inMesh()));
					flags.insert_after(flags.end(), initialIndex);
					addNeighbor();
					initialIndex = currentIndex;
				}
			}
			else {
				assert(currenCell->getVertexIndex(iv) >= 0);
				if (list.empty()) {
					is++;
					currentIndex = v->getSingCell(is);

					if (currentIndex >= 0) {
						assert(find(list.begin(), list.end(), currentIndex)
							== list.end());
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
				assert((currentCell) && (currentCell->inMesh()));
				addNeighbor();
			}
			return true;
		}
		else
			return false;
	}

	template<class Traits>
	void mfVertexStarIterator3D<Traits>::addNeighbor()
	{
		ids temp;
		temp = currentCell->getMateId((currentCell->getVertexIndex(iv) + 1) % 4);
		if ((temp >= 0) && (find(flags.begin(), flags.end(), temp) == flags.end())) {
			flags.insert_after(flags.end(), temp);
			list.insert_after(list.end(), temp);
		}

		temp = currentCell->getMateId((currentCell->getVertexIndex(iv) + 2) % 4);
		if ((temp >= 0) && (find(flags.begin(), flags.end(), temp) == flags.end())) {
			flags.insert_after(flags.end(), temp);
			list.insert_after(list.end(), temp);
		}

		temp = currentCell->getMateId((currentCell->getVertexIndex(iv) + 3) % 4);
		if ((temp >= 0) && (find(flags.begin(), flags.end(), temp) == flags.end())) {
			flags.insert_after(flags.end(), temp);
			list.insert_after(list.end(), temp);
		}

		assert(currentCell->getVertexIndex(iv) >= 0);
	}

	template<class Traits>
	SCELL* mfVertexStarIterator3D<Traits>::operator->()
	{
		return currentCell;
	}

	template<class Traits>
	SCELL* mfVertexStarIterator3D<Traits>::operator*()
	{
		return currentCell;
	}

	template<class Traits>
	IDS mfVertexStarIterator3D<Traits>::operator&()
	{
		return currentIndex;
	}

	template<class Traits>
	bool mfVertexStarIterator3D<Traits>::finished()
	{
		return ((status == 2) || (status == 3));
	}

	template<class Traits>
	bool mfVertexStarIterator3D<Traits>::notFinished()
	{
		return ((status == 0) || (status == 1));
	}

#undef SCELL
#undef IDS
}
