#pragma once

#include <forward_list>

#include "mfIterator.h"
#include "mfMesh.h"
#include "mfSing.h"

using std::forward_list;

namespace mf
{
#define SCELL typename mfEdgeStarIteratorTetra<Traits>::sCell
#define IDS typename mfEdgeStarIteratorTetra<Traits>::ids

	template<class Traits>
	class mfEdgeStarIteratorTetra : public mfIterator<Traits>
	{
	public:
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::ids ids;
		typedef mfSing<Traits> sSing;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfEdgeStarIteratorTetra(sMesh* mesh);

		/** Destructor */
		~mfEdgeStarIteratorTetra();

		bool initialize(ids idcell, int index1, int index2);
		virtual bool finished();
		virtual bool notFinished();
		virtual bool operator++();
		sCell* operator->();
		sCell* operator*();
		virtual ids operator&();

		int getV1Index();
		int getV2Index();

	private:
		ids initialIndex;
		ids iv; //id of vertex center to the star iteration
		sVertex* v; //instance of vertex center to the star iteration
		forward_list<ids> list; //list of cells to which the center vertex belongs to
		forward_list<ids> flags; //list of flag cells to which the center vertex belongs to
		int is; //current singular id
		ids idv1;
		ids idv2;
		int ie1;
		int ie2;
		int iEdge1;
		int iEdge2;
		int orientationStatus;

		void addNeighbor();
	};

	template<class Traits>
	mfEdgeStarIteratorTetra<Traits>::mfEdgeStarIteratorTetra(sMesh* mesh)
		: mfIterator<Traits>(mesh), is(-1), currentIndex(-1), initialIndex(-1),
		iv(-1), ie1(-1), ie2(-1), status(0), v(NULL)
	{

	}

	template<class Traits>
	mfEdgeStarIteratorTetra<Traits>::~mfEdgeStarIteratorTetra()
	{

	}

	template<class Traits>
	bool mfEdgeStarIteratorTetra<Traits>::initialize(ids idcell, int index1,
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
	bool mfEdgeStarIteratorTetra<Traits>::operator++()
	{
		if (status == 1) {
			if (currentIndex < 0) {
				if (is < 0) {
					is = 0;
					currentIndex = initialIndex;
					assert(currentIndex >= 0);
					current = mesh->getCell(currentIndex);
					assert(current && (current->inMesh()));
					idv1 = current->getVertexId(ie1);
					iEdge1 = current->getVertexIndex(idv1);
					idv2 = current->getVertexId(ie2);
					iEdge2 = current->getVertexIndex(idv2);
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
				assert(current && (current->inMesh()));
				iEdge1 = current->getVertexIndex(idv1);
				iEdge2 = current->getVertexIndex(idv2);
				addNeighbor();
			}
			return true;
		}
		else return false;
	}

	template<class Traits>
	int mfEdgeStarIteratorTetra<Traits>::getV1Index()
	{
		return iEdge1;
	}

	template<class Traits>
	int mfEdgeStarIteratorTetra<Traits>::getV2Index()
	{
		return iEdge2;
	}

	template<class Traits>
	void mfEdgeStarIteratorTetra<Traits>::addNeighbor()
	{
		ids temp;
		if (orientationStatus) { //ccw
			temp = current->getMateId(current->getLeftFaceIndex(iEdge1, iEdge2));

			if (temp >= 0 && temp != initialIndex)
				list.insert_after(list.end(), temp);
			else {
				if (temp != initialIndex || initialIndex == currentIndex) {
					orientationStatus = 0;
					sCell* aux;
					aux = mesh->getCell(initialIndex);
					temp = aux->getMateId(aux->getRightFaceIndex(ie1, ie2));
					if (temp >= 0)
						list.insert_after(list.end()), temp;
				}
			}
		}
		else { //cw
			temp = current->getMateId(current->getRightFaceIndex(iEdge1, iEdge2));
			if(temp >= 0 && temp != initialIndex)
				list.insert_after(list.end()), temp);
		}

		assert(current->getVertexIndex(iv) >= 0);
	}

	template<class Traits>
	SCELL* mfEdgeStarIteratorTetra<Traits>::operator->()
	{
		return current;
	}

	template<class Traits>
	SCELL* mfEdgeStarIteratorTetra<Traits>::operator*()
	{
		return current;
	}

	template<class Traits>
	IDS mfEdgeStarIteratorTetra<Traits>::operator&()
	{
		return currentIndex;
	}

	template<class Traits>
	bool mfEdgeStarIteratorTetra<Traits>::finished()
	{
		return ((status == 2) || (status == 3));
	}

	template<class Traits>
	bool mfEdgeStarIteratorTetra<Traits>::notFinished()
	{
		return ((status == 0) || (status == 1));
	}

#undef SCELL
#undef IDS
}