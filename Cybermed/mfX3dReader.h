#pragma once

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "mfMesh.h"
#include "mfReader.h"

using std::cerr;
using std::endl;
using std::ifstream;

namespace mf
{
	template<class Traits>
	class mfX3dReader : public mfReader<Traits>
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfX3dReader();

		/** Destructor */
		virtual ~mfX3dReader();

		virtual bool read(sMesh& mesh, string& filename);
		bool readColor(float rgba[], string& filename);

	private:
		bool reader(ifstream& pf, sMesh& mesh);
	};

	template<class Traits>
	mfX3dReader<Traits>::mfX3dReader()
	{

	}

	template<class Traits>
	mfX3dReader<Traits>::~mfX3dReader()
	{

	}

	template<class Traits>
	bool mfX3dReader<Traits>::read(sMesh& mesh, string& filename)
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
	bool mfX3dReader<Traits>::reader(ifstream& pf, sMesh& mesh)
	{
		space coords[3];
		int auxvtx[3];
		int test;
		bool isFaceSet = false;

		string auxstr;
		string triangleSet("<IndexedTriangleSet");
		string faceSet("<IndexedFaceSet");
		string index("index=");
		string coordIndex("coordIndex");
		string point("point");
		string stop;

		char c;

		pf >> auxstr;

		while ((!pf.eof()) && (auxstr.compare(triangleSet) != 0) &&
			(auxstr.compare(faceSet) != 0))
			pf >> auxstr;

		if (pf.eof()) {
			cerr << "end of file while reading the model" << endl;
			exit(1);
		}

		if (auxstr.compare(faceSet) == 0) {
			isFaceSet = true;
			stop = coordIndex;
		}
		else
			stop = index;

		while ((!pf.eof()) && (auxstr.compare(stop) != 0))
			pf >> auxstr;

		if (pf.eof()) {
			cerr << "end of file while reading the model" << endl;
			exit(1);
		}

		size_t numPos = auxstr.find("\"");
		string num = auxstr.substr(numPos + 1, auxstr.size() - 1);
		auxvtx[0] = atoi(num.c_str());
		pf >> auxvtx[1];
		pf >> auxvtx[2];
		if (isFaceSet) {
			pf >> test;
			if (test != -1) {
				cerr << "The model must have triangles!" << endl;
				exit(1);
			}
		}
		mesh.addCell(auxvtx);
		pf >> c;

		while (isdigit(c)) {
			pf.putback(c);
			pf >> auxvtx[0] >> auxvtx[1] >> auxvtx[2];

			if (isFaceSet) {
				pf >> test;
				if (test != -1) {
					cerr << "The model must have triangles!" << endl;
					exit(1);
				}
			}
			mesh.addCell(auxvtx);
			pf >> c;
		}

		while ((!pf.eof()) && (auxstr.compare(point) != 0))
			pf >> auxstr;

		if (pf.eof()) {
			cerr << "end of file while reading the model" << endl;
			exit(1);
		}

		numPos = auxstr.find("\"");
		num = auxstr.substr(numPos + 1, auxstr.size() - 1);
		coords[0] = strtod(num.c_str(), NULL);
		pf >> coords[1] >> coords[2];

		mesh.addVertex(coords);
		pf >> c;

		while (isdigit(c) || c == '-') {
			pf.putback(c);
			pf >> coords[0] >> coords[1] >> coords[2];
			mesh.addVertex(coords);
			pf >> c;
		}

		return true;
	}

	template<class Traits>
	bool mfX3dReader<Traits>::readColor(float rgba[], string& filename)
	{
		rgba[0] = rgba[3] = 1;
		rgba[1] = rgba[2] = 0;
		return true;
	}
}
