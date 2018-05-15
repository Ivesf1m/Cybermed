#pragma once

#include "mfMesh.h"

namespace mf
{
	/**
	*	Iterator base class.
	*/

	template<class Traits>
	class mfIterator
	{
		public:
			typedef typename Traits::sCell sCell;
			typedef typename Traits::ids ids;
			typedef typename Traits::sMesh sMesh;

			/**
			*	Sets the mesh that will be manipulated by an object of this class.
			*/
			void setMesh(sMesh* _mesh);

		protected:
			/**
			*	Constructor
			*/
			mfIterator(sMesh* _mesh);

			/**
			*	Destructor
			*/
			virtual ~mfIterator();


			virtual bool finished() = 0;
			virtual bool notFinished() = 0;
			virtual bool operator++() = 0;
			virtual ids operator&() = 0;

			sMesh* mesh;
			ids currentIndex;
			sCell* currentCell;
			int current;

			/**< Iterator status */
			// 0 - iterator with no position
			// 1 - iterator in normal position
			// 2 - iterator in lower bound
			// 3 - iterator in upper bound
			int status;
	};

	template<class Traits>
	mfIterator<Traits>::mfIterator(sMesh* _mesh)
	{
		assert(_mesh);
		this->mesh = mesh;
	}

	template<class Traits>
	mfIterator<Traits>::~mfIterator()
	{

	}

	template<class Traits>
	void mfIterator<Traits>::setMesh(sMesh* _mesh)
	{
		assert(_mesh);
		this->mesh = _mesh;
	}
}
