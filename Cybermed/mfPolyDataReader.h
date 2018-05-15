#pragma once

#include <fstream>
#include "mfKdTree.h"
#include "mfMesh.h"
#include "mfReader.h"
#include "mfVertexStarIteratorTriSurf.h"

using std::ifstream;

namespace mf
{
	template<class Traits>
	class mfPolyDataReader : public mfReader<Traits>
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;
		typedef typename Traits::sGeometric sGeometric;

		/** Constructor */
		mfPolyDataReader();

		/** Destructor */
		virtual ~mfPolyDataReader();

		void checkPoints(bool value);

		virtual bool read(sMesh& mesh, string& filename);

	private:

		class sObject {
		public:
			sVertex* v;
			ids ids;
		};

		class sObjectCompare {
		public:
			bool greater(sObject v1, sObject v2, int dim) {
				assert((dim == 0) || (dim == 1));
				return v1.v->getCoord(dim) > v2.v->getCoord(dim);
			}
		};

		bool cpoints;
		mfKdTree<sObject, sObjectCompare> kd;
		vector<ids> vtx;
		sVertex temp;
		sObject obj;
		sObject result;
		sGeometric geom;

		bool reader(ifstream& pf, sMesh& mesh);
		int sameVertices(sCell& cell, vector<int>& vtx)
	};

	template<class Traits>
	mfPolyDataReader<Traits>::mfPolyDataReader()
		: kd(sVertex::getDimension()), cpoints(false)
	{

	}

	template<class Traits>
	mfPolyDataReader<Traits>::~mfPolyDataReader()
	{

	}

	template<class Traits>
	void mfPolyDataReader<Traits>::checkPoints(bool value)
	{
		cpoints = value;
	}

	template<class Traits>
	bool mfPolyDataReader<Traits>::read(sMesh& mesh, string& filename)
	{
		geom.setMesh(mesh);
		
		ifstream pf(filename);
		bool ret = true;

		if (pf) {
			if (!reader(pf, mesh))
				ret = false;
			pf.close();
		}
		else
			ret = false;

		return ret;
	}

	template<class Traits>
	bool mfPolyDataReader<Traits>::reader(ifstream& pf, sMesh& mesh)
	{
		string auxstr("");
		space coords[3];
		sCell cell;
		int i, nv, j, nc, k, idnv;
		vector<int> auxvtx(cell.getNumberVerticesInCell());
		bool test;

		while ((!pf.eof()) && (auxstr.compare("POINTS") != 0))
			pf >> auxstr;

		if (!pf.eof()) {
			pf >> nv;
			pf >> auxstr;

			assert(nv > 0);
			vtx.resize(nv);

			if (cpoints) {
				for (i = 0; i < nv; ++i) {
					for (j = 0; j < 3; ++j)
						pf >> coords[j];

					if (cpoints) {
						obj.v = &temp;
						obj.v->setCoords(coords);
						test = false;
						if (kd.size() > 0) {
							result = kd.nearest(obj);
							test = geom.sameVertices(result.v, obj.v);
						}
						if (test)
							vtx[i] = result.id;
						else {
							vtx[i] = mesh.addVertex(coords);
							obj.v = &(mesh.getVertex(vtx[i]));
							obj.id = vtx[i];
						}
					}
					else
						mesh.addVertex(coords);
				}
			}
		}
		else
			return false;

		while ((!pf.eof()) && (auxstr.compare("POLYGONS") != 0))
			pf >> auxstr;

		if (!pf.eof()) {
			pf >> nc;
			pf >> auxstr;

			assert(nc >= 0);
			
			mfVertexStarIteratorTriSurf<Traits> itvs(mesh);
			bool repeated;

			for (i = 0; i < nc; ++i) {
				pf >> idnv;

				assert(idnv <= cell.getNumberVerticesInCell());

				for (j = 0; j < cell.idnv; ++j) {
					pf >> k;
					assert((k > 0) && (k < nv));
					if (cpoints)
						auxvtx[j] = vtx[k];
					else
						auxvtx[j] = k;
				}

				for (j = idnv; j < cell.getNumberVerticesInCell(); ++j)
					auxvtx[j] = -1;

				repeated = false;
				if (mesh.getVertex(auxvtx[0]).getNumberOfSings() > 0) {
					for (itvs.initialize(auxvtx[0]); itvs.notFinished(); ++itvs) {
						if (sameVertices(*itvs, auxvtx) == 3) {
							repeated = true;
							break;
						}
					}
				}

				if (!repeated)
					mesh.addCell(auxvtx);
			}
		}
		else {
			return false;
		}

		return true;
	}

	template<class Traits>
	int mfPolyDataReader<Traits>::sameVertices(sCell& cell, vector<ids>& vtx)
	{
		int i, j, v1, v2, nv = 0;

		for (i = 0; i < 3; ++i) {
			v1 = cell.getVertexId(i);
			for (j = 0; j < 3; ++j) {
				v2 = vtx[j];
				if (v1 == v2) {
					nv++;
					break;
				}
			}
		}

		return nv;
	}
}