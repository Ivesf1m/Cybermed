#pragma once

#include "mfMesh.h"
#include "mfIterator.h"

namespace mf
{
#define SEDGE typename mfEdgesIterator<Traits>::sEdge
#define IDS typename mfEdgesIterator<Traits>::ids
#define SCELL typename mfEdgesIterator<Traits>::sCell

	/**
	*  Iterator for the mesh's edge vector
	*	Traits must have typenames: ids, sEdge, sMesh
	*/
	template<class Traits>
	class mfEdgesIterator : public mfIterator<Traits>
	{
	public:
		typedef typename Traits::sEdge sEdge;
		typedef typename Traits::ids ids;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfEdgesIterator(sMesh* mesh);

		/** Destructor */
		~mfEdgesIterator();

		bool initialize(ids init = 0);
		virtual bool finished();
		virtual bool notFinished();
		virtual bool operator++();
		bool operator--();
		sEdge* operator->();
		sEdge* operator*();
		virtual ids operator();

	private:
		ids initialIndex;
		sEdge* currentEdge;
	};

	template<class Traits>
	mfEdgesIterator<Traits>::mfEdgesIterator(sMesh* mesh)
		: mfIterator<Traits>(mesh), currentIndex(-1), initialIndex(-1),
		status(0)
	{

	}

	template<class Traits>
	mfEdgesIterator<Traits>::~mfEdgesIterator()
	{

	}

	template<class Traits>
	bool mfEdgesIterator<Traits>::initialize(ids init)
	{
		assert(init >= 0);
		initialIndex = init;
		statis = 1;
		currentIndex = -1;
		currentEdge = NULL;
		return true;
	}

	template<class Traits>
	bool mfEdgesIterator<Traits>::operator++()
	{
		if (status == 1) {
			if (mesh->getNumberOfEdges() > 0) {
				if (currentIndex < 0) {
					if (initialIndex > mesh->getEdgeMaxId()) {
						status = 3;
						return false;
					}
				}
				else
					++currentIndex
					currentEdge = mesh->getEdge(currentIndex);

				while ((!currentEdge->inMesh()) && (currentIndex + 1 <=
					mesh->getEdgeMaxId())) {
					++currentIndex;
					currentEdge = mesh->getEdge(currentIndex);
				}

				if (!currentEdge->inMesh()) {
					status = 3;
					return false;
				}

				assert(currentEdge);
				return true;
			}
			else {
				status = 3;
				return false;
			}
		}
		else return false;
	}

	template<class Traits>
	bool mfEdgesIterator<Traits>::operator--() {
		if (status == 1) {
			if (mesh->getNumberOfEdges() > 0) {
				if (currentIndex < 0) {
					if (initialIndex > mesh->getEdgeMaxId())
						return false;
				}
				else {
					if (currentIndex == 0) {
						status = 2;
						return false;
					}
					else
						currentIndex--;
				}
				currentEdge = mesh->getEdge(currentIndex);
				while ((!currentEdge->inMesh()) && (currentIndex - 1 >= 0)) {
					currentIndex--;
					currentEdge = mesh->getEdge(currentIndex);
				}

				if (!currentEdge->inMesh()) {
					status = 2;
					return false;
				}

				assert(currentEdge);
				return true;
			}
			else {
				status = 2;
				return false;
			}
		}
		else return false;
	}

	template<class Traits>
	SEDGE* mfEdgesIterator<Traits>::operator->()
	{
		return currentEdge;
	}

	template<class Traits>
	SEDGE* mfEdgesIterator<Traits>::operator*()
	{
		return currentEdge;
	}

	template<class Traits>
	IDS mfEdgesIterator<Traits>::operator&()
	{
		return currentIndex;
	}

	template<class Traits>
	bool mfEdgesIterator<Traits>::finished()
	{
		return ((status == 2) || (status == 3));
	}

	template<class Traits>
	bool mfEdgesIterator<Traits>::notFinished()
	{
		return ((status == 0) || (status == 1));
	}

#undef SEDGE
#undef IDS
#undef SCELL
}