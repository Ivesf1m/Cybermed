#pragma once

#include "mfMesh.h"
#include <string>

using std::string;

namespace mf
{
	/**
	*	Interface for file readers.
	*/

	template<class Traits>
	class mfReader
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfReader();

		/** Destructor */
		virtual ~mfReader();

		/**
		*	Executes file reading.
		*	@param mesh: pointer to the alocated mesh
		*	@param filename: name of the mesh's file
		*/
		virtual bool read(sMesh& malha, string& filename) = 0;
		virtual bool readColor(array<float, 4>& rgba, string& filename);
	};

	template<class Traits>
	mfReader<Traits>::mfReader()
	{

	}

	template<class Traits>
	mfReader<Traits>::~mfReader()
	{

	}
}