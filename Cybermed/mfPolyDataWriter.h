#pragma once

#include <fstream>
#include "mfCellsIterator.h"
#include "mfMesh.h"
#include "mfVerticesIterator.h"
#include "mfWriter.h"

using std::ofstream;

namespace mf
{
	template<class Traits>
	class mfPolyDataWriter : public mfWriter<Traits>
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfPolyDataWriter();

		/** Destructor */
		virtual ~mfPolyDataWriter();

		virtual bool write(sMesh& mesh, string& filename);

	private:
		bool writer(ofstream& pf, sMesh& mesh);
	};

	template<class Traits>
	mfPolyDataWriter<Traits>::mfPolyDataWriter()
	{

	}

	template<class Traits>
	mfPolyDataWriter<Traits>::~mfPolyDataWriter()
	{

	}

	template<class Traits>
	bool mfPolyDataWriter<Traits>::write(sMesh& mesh, string& filename)
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
	bool mfPolyDataWriter<Traits>::writer(ofstream& pf, sMesh& mesh)
	{
		int j;
		sCell cell;

		pf << "# vtk DataFile Version 1.0" << endl;
		pf << "Mesh from mf" << endl;
		pf << "ASCII" << endl;
		pf << "DATASET UNSTRUCTURED_GRID" << endl;
		pf << "POINTS " << malha->getNumberOfVertices() << " float" << endl;

		mfVerticesIterator<Traits> itv(mesh);

		for (itv.initialize(); itv.notFinished(); ++itv) {
			if (sVertex::getDimension() == 2) {
				pf << setprecision(15) << itv->getCoord(0) << " ";
				pf << setprecision(15) << itv->getCoord(1) << "0.0" << endl;
			}
			else if (sVertex::getDimension() == 3) {
				pf << setprecision(15) << itv->getCoord(0) << " ";
				pf << setprecision(15) << itv->getCoord(1) << " ";
				pf << setprecision(15) << itv->getCoord(2) << endl;
			}
		}

		pf << endl << "POLYGONS" << mesh.getNumberOfCells() << " ";
		pf << (cell.getNumberVerticesInCell() + 1) * mesh.getNumberOfCells();
		pf << endl;

		mfCellsIterator<Traits> itc(mesh);

		for (itc.initialize(); itc.notFinished(); ++itc) {
			pf << itc->getNumberVerticesInCell();

			for (j = 0; j < itc->getNumberVerticesInCell(); ++j)
				pf << " " << itc->getVertexId(j);
			pf << endl;
		}

		return true;
	}
}
