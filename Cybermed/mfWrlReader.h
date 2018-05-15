#pragma once

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>

#include "mfMesh.h"
#include "mfReader.h"

using std::ifstream;

namespace mf
{
	template<class Traits>
	class mfWrlReader : public mfReader<Traits>
	{
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfWrlReader();

		/** Destructor */
		virtual ~mfWrlReader();

		virtual bool read(sMesh& mesh, string& filename);

		bool read(sMesh& mesh, string& filename, int cellDimension);
		bool readColor(float rgba[], string& filename);

	private:
		bool reader(ifstream& pf, sMesh& mesh);
		bool reader(ifstream& pf, sMesh& mesh, int cellDimension);
	};

	template<class Traits>
	mfWrlReader<Traits>::mfWrlReader()
	{

	}

	template<class Traits>
	mfWrlReader<Traits>::~mfWrlReader()
	{

	}

	template<class Traits>
	bool mfWrlReader<Traits>::read(sMesh& mesh, string& filename)
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
	bool mfWrlReader<Traits>::reader(ifstream& pf, sMesh& mesh)
	{
		string auxstr;
		space coords[3];
		int auxvtx[3];
		char c;

		pf >> auxstr;

		while ((!pf.eof()) && (auxstr.compare("point") != 0))
			pf >> auxstr;

		if (pf.eof())
			return false;

		pf >> c;

		if (c == '[') {
			pf >> c;
			if (c == ']')
				return false;

			do {
				pf.putback(c);
				pf >> coords[0] >> coords[1] >> coords[2];
				mesh.addVertex(coords);
				pf >> c;
				if (c == ',')
					pf >> c;
			} while (c == '-' || isdigit(c));

			if (c != ']')
				return false;
		}
		else
			return false;

		while ((!pf.eof()) && (auxstr.compare("coordIndex") != 0))
			pf >> auxstr;

		if (pf.eof())
			return false;
		pf >> c;

		if (c == '[') {
			pf >> c;
			if (c == ']')
				return false;

			do {
				pf.putback(c);
				pf >> auxvtx[0] >> auxvtx[1] >> auxvtx[2];
				mesh.addCell(auxvtx);

				pf >> c;
				if (c == ',')
					pf >> c;
				if (c != '-')
					break;

				pf >> c;
				pf >> c;
				if (c == ',')
					pf >> c;
			} while (c == '-' || isdigit(c));

			if (c != ']')
				return false;
		}
		else
			return false;

		return true;
	}

	template<class Traits>
	bool mfWrlReader<Traits>::read(sMesh& mesh, string& filename, 
		int cellDimension)
	{
		ifstream pf(filename);

		if (pf) {
			if (!reader(pf, mesh, cellDimension))
				return false;
			pf.close();
		}
		else
			return false;

		return true;
	}

	template<class Traits>
	bool mfWrlReader<Traits>::reader(ifstream& pf, sMesh& mesh, int cellDimension)
	{
		string auxstr;
		space coords[3];
		int j;
		int auxvtx[cellDimension];
		char c;

		pf >> auxstr;

		while ((!pf.eof()) && (auxstr.compare("point") != 0))
			pf >> auxstr;

		if (pf.eof())
			return false;

		pf >> c;

		if (c == '[') {
			pf >> c;
			if (c == ']')
				return false;

			do {
				pf.putback(c);
				pf >> coords[0] >> coords[1] >> coords[2];
				mesh.addVertex(coords);
				pf >> c;
				if (c == ',')
					pf >> c;
			} while (c == '-' || isdigit(c));

			if (c != ']')
				return false;
		}
		else
			return false;

		while ((!pf.eof()) && (auxstr.compare("coordIndex") != 0))
			pf >> auxstr;

		if (pf.eof())
			return false;
		pf >> c;

		if (c == '[') {
			pf >> c;
			if (c == ']')
				return false;

			do {
				pf.putback(c);

				for (j = 0; j < cellDimension; ++j)
					pf >> auxvtx[j];

				mesh.addCell(auxvtx);

				pf >> c;
				if (c == ',')
					pf >> c;
				if (c != '-')
					break;

				pf >> c;
				pf >> c;
				if (c == ',')
					pf >> c;
			} while (c == '-' || isdigit(c));

			if (c != ']')
				return false;
		}
		else
			return false;

		return true;
	}

	template<class Traits>
	bool mfWrlReader<Traits>::readColor(float rgba[], string& filename)
	{
		ifstream inputFile(filename);
		string word;
		int value;
		bool foundColor = false;

		while (1) {
			inputFile >> word;

			if (word.compare("diffuseColor") == 0) {
				inputFile >> rgba[0] >> rgba[1] >> rgba[2];
				rgba[3] = 1;
				foundColor = true;
			}

			if (word.compare("IndexedFaceSet") == 0) {
				if (!foundColor) {
					rgba[0] = 1;
					rgba[1] = 1;
					rgba[2] = 1;
					rgba[3] = 1;
				}
				break;
			}
		}

		inputFile.close();
		return true;
	}
}
