#pragma once

#include "mfMesh.h"

namespace
{
	/**
	*	Base operation class for meshes
	*/
	template<class Traits>
	class mfMeshOper {
	public:
		typedef typename Traits::ids ids;
		typedef typename Traits::sMesh sMesh;

	protected:
		/** Constructor */
		mfMeshOper(sMesh* _mesh);

		/** Destructor */
		virtual ~mfMeshOper();

		sMesh* mesh;
	};

	template<class Traits>
	mfMeshOper<Traits>::mfMeshOper(sMesh* _mesh)
		: mesh(_mesh)
	{

	}

	template<class Traits>
	mfMeshOper<Traits>::~mfMeshOper()
	{

	}
}
