#pragma once

#include "mfMesh.h"
#include <string>

using std::string;

namespace mf
{
	/**
	*	Interface for file writers.
	*/
	template<class Traits>
	class mfWriter
	{
	public:
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfWriter();

		/** Destructor */
		~mfWriter();

		/**
		*	Writes the file
		*	@param mesh: addres of the mesh to be written
		*	@param filename: name of the file of destination
		*/
		virtual bool wrtie(sMesh& mesh, string& filename) = 0;
	};

	template<class Traits>
	mfWriter<Traits>::mfWriter()
	{

	}

	template<class Traits>
	mfWriter<Traits>::~mfWriter()
	{

	}
}