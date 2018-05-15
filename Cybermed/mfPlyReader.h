#pragma once

#include <fstream>
#include "mfMesh.h"
#include "mfReader.h"

using std::ifstream;

namespace mf
{
	template<class Traits>
	class mfPlyReader : public mfReader<Traits>
	{
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfPlyReader();

		/** Destructor */
		~mfPlyReader();

		virtual bool read(sMesh& mesh, string& filename);
		virtual bool read(sMesh& mesh, string& filename, int cellDimension);

	private:
		bool reader(ifstream& pf, sMesh& mesh);
		bool reader(ifstream& pf, sMesh& mesh, int cellDimension);
	};

	template<class Traits>
	mfPlyReader<Traits>::mfPlyReader()
	{

	}

	template<class Traits>
	mfPlyReader<Traits>::~mfPlyReader()
	{

	}

	template<class Traits>
	bool mfPlyReader<Traits>::read(sMesh& mesh, string& filename)
	{
		ifstream pf(filename);

		if (pf) {
			if (!reader(pf, malha))
				return false;
			pf.close();
		}
		else
			return false;
		return true;
	}

	template<class Traits>
	bool mfPlyReader<Traits>::reader(ifstream& pf, sMesh& mesh)
	{
		string auxstr("");
		space coord[3];
		space bin;
		sCell cell;
		int i, j;
		int numVertex, numFace;
		int auxvtx[4];
		int vPropertyJump = -3;
		int idnv;
		char c;

		//number of vertices
		while ((!pf.eof()) && (auxstr.compare("vertex") != 0))
			pf >> auxstr;
		pf >> numVertex;

		while ((!pf.eof()) && (auxstr.compare("property") != 0))
			pf >> auxstr;

		while ((!pf.eof()) && (auxstr.compare("vertex") != 0)) {
			vPropertyJump++;
			pf >> auxstr;
			pf >> auxstr;
			pf >> auxstr;
		}

		//number of faces
		while ((!pf.eof()) && (auxstr.compare("face") != 0))
			pf >> auxstr;
		pf >> numFace;

		while ((!pf.eof()) && (auxstr.compare("end_header") != 0))
			pf >> auxstr;

		if (!pf.eof()) {
			//coords
			for (i = 0; i < numVertex; ++i) {
				for (j = 0; j < 3; ++j)
					pf >> coords[j];
				for (j = 0; j < vPropertyJump; ++j)
					pf >> bin;
				mesh.addVertex(coords);
			}

			//faces
			for (i = 0; i < numFace; ++i) {
				pf >> idnv;
				assert(idnv);
				for (j = 0; j < idnv; ++j)
					pf >> auxvtx[j];

				for (j = idnv; j < 4; ++j)
					auxvtx[j] = -1;

				mesh.addCell(auxvtx);
			}
		}
		else
			return false;

		return true;
	}

	template<class Traits>
	bool mfPlyReader<Traits>::read(sMesh& mesh, string& filename, int cellDimension)
	{
		ifstream pf(filename);

		if (pf) {
			if (!reader(pf, malha, cellDimension))
				return false;
			pf.close();
		}
		else
			return false;
		return true;
	}

	template<class Traits>
	bool mfPlyReader<Traits>::reader(ifstream& pf, sMesh& mesh, int cellDimension)
	{
		string auxstr("");
		space coord[3];
		space bin;
		sCell cell;
		int i, j;
		int numVertex, numFace;
		int auxvtx[cellDimension];
		int vPropertyJump = -3;
		int faceType;
		char c;

		//number of vertices
		while ((!pf.eof()) && (auxstr.compare("vertex") != 0))
			pf >> auxstr;
		pf >> numVertex;

		while ((!pf.eof()) && (auxstr.compare("property") != 0))
			pf >> auxstr;

		while ((!pf.eof()) && (auxstr.compare("vertex") != 0)) {
			vPropertyJump++;
			pf >> auxstr;
			pf >> auxstr;
			pf >> auxstr;
		}

		//number of faces
		while ((!pf.eof()) && (auxstr.compare("face") != 0))
			pf >> auxstr;
		pf >> numFace;

		while ((!pf.eof()) && (auxstr.compare("end_header") != 0))
			pf >> auxstr;

		if (!pf.eof()) {
			//coords
			for (i = 0; i < numVertex; ++i) {
				for (j = 0; j < 3; ++j)
					pf >> coords[j];
				for (j = 0; j < vPropertyJump; ++j)
					pf >> bin;
				mesh.addVertex(coords);
			}

			//faces
			for (i = 0; i < numFace; ++i) {
				pf >> faceType;
				for (j = 0; j < idnv; ++j)
					pf >> auxvtx[j];

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
