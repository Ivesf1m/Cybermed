#pragma once

#include <fstream>
#include <iostream>
#include "mfCellsIterator.h"
#include "mfMesh.h"
#include "mfVerticesIterator.h"
#include "mfWriter.h"

using std::ofstream;

namespace mf
{
	template<class Traits>
	class mfOffWriter : public mfWriter<Traits>
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfOffWriter();

		/** Destructor */
		~mfOffWriter();

		virtual bool write(sMesh& mesh, string& filename);

	private:
		bool writer(ofstream& pf, sMesh& mesh);
	};

	template<class Traits>
	mfOffWriter<Traits>::mfOffWriter()
	{

	}

	template<class Traits>
	mfOffWriter<Traits>::~mfOffWriter()
	{

	}

	template<class Traits>
	bool mfOffWriter<Traits>::write(sMesh* mesh, string& filename)
	{
		ofstream pf(filename);

		if (pf) {
			if (!writer(pf, mesh))
				return false;
			pf.close();
		}
		else
			return false;
		return true;
	}

	template<class Traits>
	bool mfOffWriter<Traits>::writer(ofstream& pf, sMesh& mesh)
	{
		int i = 0;
		int j;
		sCell = cell;

		pf << "OFF" << endl;
		pf << mesh.getNumberOfVertices() << " ";
		pf << mesh.getNumberOfCells() << " ";
		pf << "0" << endl;

		mfVerticesIterator<Traits> itv(&mesh);
		vector<ids> nvids(mesh.getNumberOfVertices());

		for (itv.initialize(); itv.notFinished(); ++itv) {
			if (sVertex::getDimension() == 2) {
				pf << setprecision(15) << itv->getCoord(0) << " ";
				pf << setprecision(15) << itv->getCoord(1) << " 0.0" << endl;
			}
			else if (sVertex::getDimension() == 3) {
				pf << setprecision(15) << itv->getCoord(0) << " ";
				pf << setprecision(15) << itv->getCoord(1) << " ";
				pf << setprecision(15) << itv->getCoord(2) << endl;
			}

			nvids[&itv] = i++;
		}

		mfCellsIterator<Traits> itc(&mesh);

		for (itc.initialize(); itc.notFinished(); ++itc) {
			pf << itc->getNumberVerticesInCell();
			for (j = 0; j < itc->getNumberVerticesInCell(); ++j)
				pf << " " << nvids[itc->getVertexId(j)];
			pf << endl;
		}

		return true;
	}
}