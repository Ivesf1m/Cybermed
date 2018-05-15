#pragma once

#include <fstream>
#include <iomanip>
#include "mfCellsIterator.h"
#include "mfMesh.h"
#include "mfVerticesIterator.h"
#include "mfWriter.h"

using std::ofstream;

namespace mf
{
	template<class Traits>
	class mfVtkWriter : public mfWriter<Traits>
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfVtkWriter();

		/** Destructor */
		virtual ~mfVtkWriter();

		virtual bool write(sMesh& mesh, string& filename);

	private:
		bool writer(ofstream& pf, sMesh& mesh);
	};

	template<class Traits>
	mfVtkWriter<Traits>::mfVtkWriter()
	{

	}

	template<class Traits>
	mfVtkWriter<Traits>::~mfVtkWriter()
	{

	}

	template<class Traits>
	bool mfVtkWriter<Traits>::write(sMesh& mesh, string& filename)
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
	bool mfVtkWriter<Traits>::writer(ofstream& pf, sMesh& mesh)
	{
		int i = 0;
		int j;
		sCell cell;

		pf << "# vtk DataFile Version 1.0" << endl;
		pf << "Mesh from mf" << endl;
		pf << "ASCII" << endl;
		pf << "DATASET UNSTRUCTURED_GRID" << endl;
		pf << "POINTS " << mesh.getNumberOfVertices() << " float" << endl;

		mfVerticesIterator<Traits> itv(mesh);
		vector<ids> nvids(mesh.getNumberOfVertices());

		for (itv.initialize(); itv.notFinished(); ++itv) {
			pf << setprecision(15) << itv->getCoord(0) << " ";
			pf << setprecision(15) << itv->getCoord(1);
			if (sVertex::getDimension() == 2)
				pf << " 0.0" << endl;
			else
				pf << setprecision(15) << itv->getCoord(2) << endl;

			nvids[&itv] = i++;
		}

		pf << endl << "CELLS " << mesh.getNumberOfCell();
		pf << " " << (cell.getNumberVerticesInCell() + 1) *
			mesh.getNumberOfCells() << endl;

		mfCellsIterator<Traits> itc(mesh);

		for (itc.initialize(); itc.notFinished(); ++itc) {
			pf << itc->getNumberVerticesInCell();
			for (j = 0; j < itc->getNumberVerticesInCell(); ++j)
				pf << " " << nvids[itc->getVertexId(j)];
			pf << endl;
		}

		pf << endl << "CELL_TYPES " << mesh.getNumberOfCells() << endl;

		for (i = 0; i < mesh.getNumberOfCells(); ++i)
			pf << "5 ";

		return true;
	}
}