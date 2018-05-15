#pragma once

#include <fstream>
#include "mfBinaryIO.h"
#include "mfMesh.h"
#include "mfReader.h"

using std::ifstream;

namespace mf
{
	template<class Traits>
	class mfMfReader : public mfReader<Traits>
	{
	public:
		typedef mfBinaryIO<Traits> sBinaryIO;

		/** Constructor */
		mfMfReader();

		/** Destrcutor */
		~mfMfReader();

		virtual bool read(sMesh* malha, string& filename);

	private:
		sBinaryIO io;

		bool checkVersion(ifstream& pf);
		bool readHeader(ifstream& pf, ids& nv, ids& nc);
		bool reader(ifstream& pf, sMesh* mesh);
	};

	template<class Traits>
	mfMfReader<Traits>::mfMfReader()
	{

	}

	template<class Traits>
	mfMfReader<Traits>::~mfMfReader()
	{

	}

	template<class Traits>
	bool mfMfReader<Traits>::read(sMesh* mesh, string& filename)
	{
		assert(mesh);

		ifstream pf(filename, ios_base::in | ios_base::binary);

		if (pf.is_open()) {
			if (!reader(pf, mesh))
				return false;
			pf.close();
		}
		else
			return false;
		return true;
	}

	template<class Traits>
	bool mfMfReader<Traits>::checkVersion(ifstream& pf)
	{
		string header;
		pf >> header;
		if (header.compare("MF-BINARY-V1.001") == 0)
			return true;
		else
			return false;
	}

	template<class Traits>
	bool mfMfReader<Traits>::readHeader(ifstream& pf, ids& nv, ids& nc)
	{
		if((io.idsLoad(pf) <= static_cast<int>(sizeof(space))) &&
			(io.idsLoad(pf) <= static_cast<int>(sizeof(ids)) {
			if ((io.idsLoad(pf) == sVertex::getDimension()) &&
				(io.idsLoad(pf) == sCell::getDimension())) {
				nv = io.idsLoad(pf);
				nc = io.idsLoad(pf);
			}
			else
				return false;			
		}
		else
			return false;
		return true;
	}

	template<class Traits>
	bool mfMfReader<Traits>::reader(ifstream& pf, sMesh* mesh)
	{
		sCell cell;
		space coords[3];
		int i, nv, j, nc, k;
		int auxvtx[cell.getNumberVerticesInCell()];
		int auxop[cell.getNumberVerticesInCell()];

		if (!checkVersion(pf))
			return false;

		if (!readHeader(pf, nv, nc))
			return false;

		assert(nv > 0);

		for (i = 0; i < nv; ++i) {
			for (j = 0; j < sVertex::getDimension(); ++j)
				coords[j] = io.spaceLoad(pf);
			mesh->addVertex(coords);
		}

		assert(nc > 0);

		for (i = 0; i < nc; ++i) {
			for (j = 0; j < cell.getNumberVerticesInCell(); ++j) {
				k = io.idsLoad(pf);
				assert((k >= 0) && (k < nv));
				auxvtx[j] = k;
			}

			for (j = 0; j < cell.getNumberVerticesInCell(); ++j) {
				k = io.idsLoad(pf);
				auxop[j] = k;
			}
			mesh->addCell(auxvtx, auxop);
		}

		sVertex* v;
		ids n;

		for (i = 0; i < nv; ++i) {
			k = io.idsLoad(pf);
			v = mesh->getVertex(i);
			for (j = 0; j < k; ++k) {
				n = io.idsLoad(pf);
				v->addSing(n);
			}
		}

		return true;
	}
}
