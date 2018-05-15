#pragma once

#include <fstream>
#include "mfMesh.h"
#include "mfReader.h"

using std::ifstream;

namespace mf
{
	template<class Traits>
	class mfVtkReader : public mfReader<Traits>
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfVtkReader();

		/** Destructor */
		virtual ~mfVtkReader();

		virtual bool read(sMesh& mesh, string& filename);

	private:
		bool reader(ifstream& pf, sMesh& mesh);
	};

	template<class Traits>
	mfVtkReader<Traits>::mfVtkReader()
	{

	}

	template<class Traits>
	mfVtkReader<Traits>::~mfVtkReader()
	{

	}

	template<class Traits>
	bool mfVtkReader<Traits>::read(sMesh& mesh, string& filename)
	{
		ifstream pf(filename);

		if (pf) {
			if (!reader(pf, mesh))
				return false;
			pf.close();
		}
		else
			return false;

		return true;
	}

	template<class Traits>
	bool mfVtkReader<Traits>::reader(ifstream& pf, sMesh& mesh)
	{
		string auxstr("");
		space coords[3];
		sCell cell;
		int i, nv, j, nc, k, idnv;
		int auxvtx[cell.getNumberVerticesInCell()];

		while ((!pf.eof()) && (auxstr.compare("POINTS") != 0)
			pf >> auxstr;

		if (!pf.eof()) {
			pf >> nv;
			pf >> auxstr;

			assert(nv > 0);

			for (i = 0; i < nv; ++i) {
				for (j = 0; j < 3; ++j)
					pf >> coords[j];
				mesh.addVertex(coords);
			}
		}
		else
			return false;

		while ((!pf.eof()) && (auxstr.compare("CELLS") != 0)
			pf >> auxstr;

		if (!pf.eof()) {
			pf >> nc;
			pf >> auxstr;

			assert(nc >= 0);

			for (i = 0; i < nc; ++i) {
				pf >> idnv;
				assert(idnv <= cell.getNumberVerticesInCell());

				for (j = 0; j < idnv; j++) {
					pf >> k;
					assert((k >= 0) && (k < nv));
					auxvtx[j] = k;
				}

				for (j = idnv; j < cell.getNumberVerticesInCell(); ++j)
					auxvtx[j] = -1;

				mesh.addCell(auxvtx);
			}
		}
		else
			return false;

		return true;
	}


}