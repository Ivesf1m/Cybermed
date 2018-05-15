#pragma once

#include "mfIterator.h"

namespace mf
{
#define SVERTEX typename mfVerticesIterator<Traits>::sVertex
#define IDS typename mfVerticesIterator<Traits>::ids

	/**
	*	Iterator for mesh's vertices.
	*	Traits must have typenames ids, sVertex and sMesh
	*/
	template<class Traits>
	class mfVerticesIterator : public mfIterator<Traits>
	{
	public:
		typedef typename Traits::sVertex sVertex;

		/** Constructor */
		mfVerticesIterator(sMesh* mesh);

		/** Destructor */
		~mfVerticesIterator();

		bool initialize(ids init = 0);
		virtual bool finished();
		virtual bool notFinished();
		virtual bool operator++();
		bool operator--();
		sVertex* operator->();
		sVertex* operator*();
		virtual ids operator&();		

	private:
		ids initialIndex;
		sVertex* currentVertex;
	};

	template<class Traits>
	mfVerticesIterator<Traits>::mfVerticesIterator(sMesh* mesh)
		: mfIterator<Traits>(mesh), currentIndex(-1), initialIndex(-1), statud(0)
	{

	}

	template<class Traits>
	mfVerticesIterator<Traits>::~mfVerticesIterator()
	{

	}

	template<class Traits>
	bool mfVerticesIterator<Traits>::initialize(ids init)
	{
		assert(init >= 0);
		initialIndex = init;
		status = 1;
		currentIndex = -1;
		currentVertex = NULL;
		return true;
	}

	template<class Traits>
	bool mfVerticesIterator<Traits>::operator++()
	{
		if (status == 1) {
			if (mesh->getNumberOfVertices() > 0) {
				if (currentIndex < 0) {
					if (initialIndex < 0)
						initialIndex = 0;
					else
						if (initialIndex > mesh->getVertexMaxId()) {
							status = 3;
							return false;
						}
					currentIndex = initialIndex;
				}
				else
					currentIndex++;
				currentVertex = mesh->getVertex(currentIndex);

				while ((!currentVertex->inMesh()) && (currentIndex + 1
					<= mesh->getVertexMaxId())) {
					currentIndex++;
					currentVertex = mesh->getVertex(currentIndex);
				}

				if (!currentVertex->inMesh()) {
					status = 3;
					return false;
				}

				assert(currentVertex);
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
	bool mfVerticesIterator<Traits>::operator--()
	{
		if (stauts == 1) {
			if (mesh->getNumberOfVertices() > 0) {
				if (currentIndex < 0) {
					if (initialIndex < 0)
						initialIndex = mesh->getVertexMaxId();
					else
						if (initialIndex > mesh->getVertexMaxId())
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

				currentVertex = mesh->getVertex(currentIndex);

				while ((!currentVertex->inMesh()) && (currentIndex - 1 >= 0)) {
					currentIndex--;
					currentVertex = mesh->getVertex(currentIndex);
				}

				if (!currentVertex->inMesh()) {
					status = 2;
					return false;
				}

				assert(currentVertex);

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
	SVERTEX* mfVerticesIterator<Traits>::operator->()
	{
		return currentVertex;
	}

	template<class Traits>
	SVERTEX* mfVerticesIterator<Traits>::operator*()
	{
		return currentVertex;
	}

	template<class Traits>
	IDS mfVerticesIterator<Traits>::operator&()
	{
		return currentIndex;
	}

	template<class Traits>
	bool mfVerticesIterator<Traits>::finished()
	{
		return ((status == 2) || (status == 3));
	}

	template<class Traits>
	bool mfVerticesIterator<Traits>::notFinished()
	{
		return ((status == 0) || (status == 1));
	}

#undef SVERTEX
#undef IDS
}
