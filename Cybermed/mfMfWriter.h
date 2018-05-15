#pragma once

#include <fstream>
#include <iomanip>
#include "mfBinaryIO.h"
#include "mfCellsIterator.h"
#include "mfVerticesIterator.h"
#include "mfWriter.h"

using std::ofstream;

namespace mf
{
	template<class Traits>
	class mfMfWriter : public mfWriter<Traits>
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;
		typedef mfBinaryIO<Traits> sBinaryIO;

		/** Constructor */
		mfMfWriter();

		/** Destructor */
		~mfMfWriter();

		virtual bool write(sMesh* mesh, string& filename);

	private:
		sBinaryIO io;
		void writeVersion(ofstream& pf);
		void writeHeader(ofstream& pf, ids nv, ids nc);
		bool writer(ofstream& pf, sMesh* mesh);
	};

	template<class Traits>
	mfMfWriter<Traits>::mfMfWriter()
	{

	}

	template<class Traits>
	mfMfWriter<Traits>::~mfMfWriter()
	{

	}

	template<class Traits>
	mfMfWriter<Traits>::write(sMesh* mesh, string& filename)
	{
		assert(mesh);

		ofstream pf(filename, ios_base::out | ios_base::binary | ios_base::trunc);

		if (pf.is_open()) {
			if (!writer(pf, mesh))
				return false;
			pf.close();
		}
		else
			return false;
		return true;
	}

	template<class Traits>
	void mfMfWriter<Traits>::writeVersion(ofstream& pf)
	{
		pf << "MF-BINARY-V1.001";
	}

	template<class Traits>
	void mfMfWriter<Traits>::writeHeader(ofstream& pf, ids nv, ids nc)
	{
		sCell cell;
		io.idsStore(pf, sizeof(space));
		io.idsStore(pf, sizeof(ids));
		io.idsStore(pf, sVertex::getDimension());
		io.idsStore(pf, cell.getNumberVerticesInCell());
		io.idsStore(pf, nv);
		io.idsStore(pf, nc);
	}

	template<class Traits>
	bool mfMfWriter<Traits>::writer(ofstream& pf, sMesh* mesh)
	{
		int i, nv, j, nc;

		nv = mesh->getNumberOfVertices();
		nc = mesh->getNumberOfCells();
		vector<ids> nvids(nv);
		vector<ids> ncids(nc);

		writeVersion(pf);

		writeHeader(pf, nv, nc);

		mfVerticesIterator<Traits> itv(mesh);
		i = 0;
		for (itv.initialize(); itv.notFinished(); ++itv) {
			for (j = 0; j < sVertex::getDimension(); ++j)
				io.spaceStore(pf, itv->getCoord(j));
			nvids[&itv] = i++;
		}

		mfCellsIterator<Traits> itc(mesh);
		i = 0;
		for (itc.initialize(); itc.notFinished(); ++itc)
			nvids[&itv] = i++;

		for (itc.initialize(); itc.notFinished(); ++itc) {
			for (j = 0; j < itc->getNumberVerticesInCell(); ++j)
				io.idsStore(pf, nvids[itc->getVertexId(j)]);

			for (j = 0; j < itc->getNumberVerticesInCell(); ++j)
				if (itc->getMateId(j) >= 0)
					io.idsStore(pf, ncids[itc->getMateId(j)]);
				else
					io.idsStore(pf, -1);
		}

		for (itv.initialize(); itv.notFinished(); ++itv) {
			io.idsStore(pf, itv->getNumberofSings());
			for (j = 0; j < itv->getNumberOfSings(); ++j)
				io.idsStore(pf, itv->getSingCell(j));
		}

		return true;
	}
}