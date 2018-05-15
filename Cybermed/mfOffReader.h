#pragma once

#include <fstream>
#include "mfMesh.h"
#include "mfReader.h"

using std::ifstream;

namespace mf
{
	template <class Traits>
	class mfOffReader : public mfReader<Traits>
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfOffReader();

		/** Destructor */
		virtual ~mfOffReader();

		virtual bool read(sMesh& mesh, string& filename);

	private:
		bool reader(ifstream& pf, sMesh& mesh);
	};

	template<class Traits>
	mfOffReader<Traits>::mfOffReader()
	{

	}

	template<class Traits>
	mfOffReader<Traits>::~mfOffReader()
	{

	}

	template<class Traits>
	mfOffReader<Traits>::read(sMesh& mesh, string& filename)
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
	template mfOffReader<Traits>::reader(ifstream& pf, sMesh& malha)
	{
		string auxstr("");
		space coords[3];
		sCell cell;
		int i, nv, j, nc, k, idnv;
		ids auxvtx[cell.getNumberVerticesInCell()];

		while ((!pf.eof()) && (auxstr.compare("OFF") != 0))
			pf >> auxstr;

		if (!pf.eof()) {
			pf >> nv;
			pf >> nc;
			pf >> k;

			assert(nv > 0);

			for (i = 0; i < nv; ++i) {
				for (j = 0; j < 3; ++j)
					pf >> coords[j];
				mesh.addVertex(coords);
			}

			assert(nc >= 0);

			for (i = 0; i < nc; ++i) {
				pf >> idnv;
				assert(idnv < cell.getNumberVerticesInCell());

				for (j = 0; j < idnv; ++j) {
					pf >> k;
					assert((k >= 0) && (k < nv));
					auxvtx[j] = k;
				}

				for (j = idnv; j < 4; ++j)
					auxvtx[j] = -1;

				mesh.addCell(auxvtx);
			}
		}
		else
			return false;

		return true;
	}
}
