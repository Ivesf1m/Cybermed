#pragma once

#include <forward_list>

#include "mfIterator.h"
#include "mfMesh.h"
#include "mfSing.h"

using std::forward_list;

namespace mf
{
#define SCELL typename mfEdgeStarIterator3D<Traits>::sCell
#define IDS typename mfEdgeStarIterator3D<Traits>::ids

	template<class Traits>
	class mfEdgeStarIterator3D : public mfIterator<Traits>
	{
	public:
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::ids ids;
		typedef mfSing<Traits> sSing;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfEdgeStarIterator3D(sMesh *mesh);

		/** Destructor */
		virtual ~mfEdgeStarIterator3D();

		bool initialize(ids idcell, int index1, int index2);
		virtual bool finished();
		virtual bool notFinished();
		virtual bool operator++();
		sCell* operator->();
		sCell* operator*();
		virtual ids operator&();

	private:
		ids initialIndex;
		ids iv;
		ids idv1;
		ids idv2;
		sVertex* v;
		forward_list<ids> list;
		forward_list<ids> flags;
		int ie1;
		int ie2;
		int iEdge1;
		int iEdge2;
		int orientationStatus;
		int is;

		void addNeighbor();
	};

	template<class Traits>
	mfEdgeStarIterator3D<Traits>::mfEdgeStarIterator3D(sMesh* mesh)
		: mfIterator<Traits>(mesh), is(-1), currentIndex(-1), initialIndex(-1),
		iv(-1), ie1(-1), ie2(-1), status(0), v(NULL)
	{

	}

	template<class Traits>
	mfEdgeStarIterator3D<Traits>::~mfEdgeStarIterator3D()
	{

	}

	template<class Traits>
	bool mfEdgeStarIterator3D<Traits>::initialize(ids idcell, int index1,
		int index2)
	{
		assert(idcell >= 0);
		assert((index1 >= 0) && (index1 <= 3));
		assert((index2 >= 0) && (index2 <= 3));

		ie1 = index1;
		iEdge1 = ie1;
		ie2 = index2;
		iEdge2 = ie2;
		initialIndex = idcell;
		orientationStatus = 1;
		status = 1;
		currentIndex = -1;
		current = NULL;
		list.clear();
		return true;
	}

	template<class Traits>
	bool mfEdgeStarIterator3D<Traits>::operator++()
	{
		if (status == 1) {
			if (currentIndex < 0) {
				if (is < 0) {
					is = 0;
					currentIndex = initialIndex;
					assert(currentIndex >= 0);
					current = mesh->getCell(currentIndex);
					assert((current) && (current->inMesh()));
					idv1 = current->getVertexId(ie1);
					idv2 = current->getVertexId(ie2);
					addNeighbor();
				}
			}
			else {
				if (list.empty()) {
					is++;
					current = NULL;
					currentIndex = -1;
					status = 3;
					return false;
				}
				currentIndex = list.front();
				list.pop_front();
				current = mesh->getCell(currentIndex);
				assert((current) && (current->inMesh()));
				iEdge1 = current->getVertexId(idv1);
				iEdge2 = current->getVertexId(idv2);
				addNeighbor();
			}
			return true;
		}
		else return false;
	}

	template<class Traits>
	void mfEdgeStarIterator3D<Traits>::addNeighbor()
	{
		ids temp;
		if (orientationStatus) {
			temp = current->getMateId(current->getLeftFaceIndex(iEdge1, iEdge2));
			if (temp >= 0 && temp != initialIndex)
				list.insert_after(list.end(), temp);
			else if (temp != initialIndex || initialIndex == currentIndex) {
				orientationStatus = 0;
				sCell* aux;
				aux = mesh->getCell(initialIndex);
				temp = aux->getMateId(aux->getRightFaceIndex(ie1, ie2));
				if(temp >= 0)
					list.insert_after(list.end()), temp);
			}
		}
		else {
			temp = current->getMateId(current->getRightFaceIndex(iEdge1, iEdge2));
			if(temp >= 0 && temp != initialIndex)
				list.insert_after(list.end()), temp);
		}

		assert(current->getVertexIndex(iv) >= 0);
	}

	template<class Traits>
	SCELL* mfEdgeStarIterator3D<Traits>::operator->()
	{
		return current;
	}

	template<class Traits>
	SCELL* mfEdgeStarIterator3D<Traits>::operator*()
	{
		return current;
	}

	template<class Traits>
	IDS mfEdgeStarIterator3D<Traits>::operator&()
	{
		return currentIndex;
	}

	template<class Traits>
	bool mfEdgeStarIterator3D<Traits>::finished()
	{
		return ((status == 2) || (status == 3));
	}

	template<class Traits>
	bool mfEdgeStarIterator3D<Traits>::notFinished()
	{
		return ((status == 0) || (status == 1));
	}

#undef SCELL
#undef IDS

}