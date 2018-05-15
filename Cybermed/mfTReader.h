#pragma once

#include "mfMesh.h"
#include <string>

using std::string;

namespace mf
{
	template<class Traits>
	class mfTReader
	{
	public:
		typedef typename Traits::ids ids;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfTReader(int vThreads, int cThreads);

		/** Destructor */
		virtual ~mfTReader();

		void setNumberOfThreads(int vThreads, int cThreads);

		virtual bool read(sMesh& mesh, string& filename) = 0;

	protected:

		struct mfTReaderData {
			string str;
			ids id;
			ids start;
			ids end;
			sMesh *mesh;
			mfXmlParser* config;
		};

		int vThreads; //number of vertex threads
		int cThreads; //number of cell threads
	};

	template<class Traits>
	mfTReader<Traits>::mfTReader(int vThreads, int cThreads)
	{
		setNumberOfThreads(vThreads, cThreads);
	}

	template<class Traits>
	mfTReader<Traits>::~mfTReader()
	{

	}

	template<class Traits>
	void mfTReader<Traits>::setNumberOfThreads(int vThreads, int cThreads)
	{
		assert(vThreads > 0);
		assert(cThreads > 0);

		this->vThreads = vThreads;
		this->cThreads = cThreads;
	}


}
