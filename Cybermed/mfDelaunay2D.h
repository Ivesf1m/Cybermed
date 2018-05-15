#pragma once

#include <algorithm>
#include <forward_list>
#include "mfBoundaryCellCIterator2D.h"
#include "mfKdTree.h"
#include "mfMesh.h"
#include "mfSearchDIDO.h"
#include "mfVerticesIterator.h"

using std::find;
using std::forward_list;

namespace mf
{
#define SPACE typename mfDelaunay2D<Traits>::space
#define IDS typename mfDelaunay2D<Traits>::ids
#define SMESH typename mfDelaunay2D<Traits>::sMesh

	template<class Traits>
	class mfDelaunay2D
	{
	public:
		typedef typename Traits::space space;
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sGeometric sGeometric;
		typedef typename Traits::sTopology sTopology;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfDelaunay2D(bool kdtree = true);

		/** Constructor */
		mfDelaunay2D(sMesh* _mesh, bool kdtree = true);

		/** Destructor */
		~mfDelaunay2D();

		ids addVertex(vector<space>& coords, ids inicell = -1);

		ids searchVertex(vector<space>& coords);

		sMesh* getMesh();

	protected:
		sMesh* mesh;

		void add(ids idvertice, ids inicell);

		class sObject {
		public:
			sObject(sVertex* _v, ids _id) {
				v = _v;
				id = _id;
			};

			sVertex* v;
			ids id;
		};

		class sObjectCompare {
		public:
			bool greater(sObject* v1, sObject* v2, int dim) {
				assert((dim == 0) || (dim == 1));
				return v1->v->getCoord(dim) > v2->v->getCoord(dim);
			}
		};

		mfKdTree<sObject*, sObjectCompare> kd;

	private:
		bool free;
		sGeometric geom;
		sTopology top;
		mfSearchDIDO<Traots> search;

		void legalizeEdge(ids cell, int edge);
		void insertIn(ids idcell, ids idvertex);
		void insertOut(ids idcell, ids idvertex, int edge);
		void insertOn(ids idcell, ids idvertex, int edge);
		void lockCells(ids id1, ids id2, sCell& c1, sCell& c2);
	};

	template<class Traits>
	mfDelaunay2D<Traits>::mfDelaunay2D(bool kdtree)
		: search(NULL), free(true), kd(2)
	{
		mesh = new sMesh();
		geom.setMesh(mesh);
		top.setMesh(mesh);
		search.setMesh(mesh);
	}

	template<class Traits>
	mfDelaunay2D<Traits>::mfDelaunay2D(sMesh* _mesh, bool kdtree)
		: search(_mesh), free(false), mesh(_mesh), kd(2)
	{
		geom.setMesh(mesh);
		top.setMesh(mesh);
	}

	template<class Traits>
	mfDelaunay2D<Traits>::~mfDelaunay2D()
	{
		if (free)
			delete mesh;
	}

	template<class Traits>
	void mfDelaunay2D<Traits>::searchVertex(vector<space>& coords)
	{
		sVertex v;
		v.setCoords(coords);
		sObject o(&v, -1);
		sObject near;

		near = kd.nearest(o);

		if (geom.sameVertices(&v, near->v)
			return near->id;
		else
			return -1;
	}

	template<class Traits>
	void mfDelaunay2D<Traits>::add(ids idvertex, ids inicell)
	{
		assert(idvertex >= 0);

		int edge = -1;
		int pos;

		sObject novo(&(mesh->getVertex(idvertex)), idvertex);
		if (inicell < 0) {
			sObject o = kd.nearestAndInsert(novo);
			inicell = o.v->getSingCell();
		}
		else
			kd.insert(novo);

		pos = search.dido(mesh->getVertex(mesh->getCell(inicell)->getVertexId(0)),
			mesh->getVertex(idvertex)->getCoords(), inicell, edge);
		assert(pos != 3);

		if (pos == 0)
			insertIn(inicell, idvertex);
		else if (pos == 1)
			insertOut(inicell, idvertex, edge);
		else if (pos == 2)
			insertOn(inicell, idvertex, edge);
	}

	template<class Traits>
	IDS mfDelaunay2D<Traits>::addVertex(vector<space>& coords, ids inicell)
	{
		ids idvertex = searchVertex(coords);
		if (idvertex < 0) {
			idvertex = mesh->addVertex(coords);

			if (mesh->getNumberOfVertices() > 3)
				add(idvertex, inicell);
			else {
				sObject novo(&(mesh->getVertex(idvertex)), idvertex);
				kd.insert(novo);

				if (mesh->getNumberOfVertices() == 3) {
					vector<ids> newcell(3);
					mfVerticesIterator<Traits> it(mesh);
					it.initialize();
					newcell[0] = &it;
					++it;
					newcell[1] = &it;
					++it;
					newcell[2] = &it;

					mesh->addCell(newcell);
				}
			}
		}
		return idvertex;
	}

	template<class Traits>
	sMesh* mfDelaunay2D<Traits>::getMesh()
	{
		return mesh;
	}

	template<class Traits>
	void mfDelaunay2D<Traits>::legalizeEdge(ids idcell, int edge)
	{
		sCell& cell = mesh->getCell(idcell);
		if (cell.getMateId(edge) >= 0) {
			ids idopposite = cell.getMateId(edge);
			sCell& opposite = mesh->getCell(idopposite);

			lockCells(idcell, idopposite, cell, opposite);
			ids idv = cell.getVertexId(edge);
			assert(opposite.getVertexMateId(idcell) >= 0);

			if (geom.inCircle(mesh->getVertex(cell.getVertexId((edge + 2) % 3)),
				mesh->getVertex(cell.getVertexId(edge)),
				mesh->getVertex(cell.getVertexId((edge + 1) % 3)),
				mesh->getVertex(opposite.getVertexMateId(idcell)))) {
				top.flip2D(idcell, idopposite);

				assert(opposite.getVertexId(0) != opposite.getVertexId(1));
				assert(opposite.getVertexId(0) != opposite.getVertexId(2));
				assert(opposite.getVertexId(2) != opposite.getVertexId(2));

				assert(cell.getVertexId(0) != cell.getVertexId(1));
				assert(cell.getVertexId(0) != cell.getVertexId(2));
				assert(cell.getVertexId(2) != cell.getVertexId(2));

				lockCell(idcell, idopposite, cell, opposite);

				legalizeEdge(idcell, cell.getVertexIndex(idv));
				legalizeEdge(idopposite, opposite.getVertexIndex(idv));
			}
			else
				lockCells(idcell, idopposite, cell, opposite);
		}
	}

	template<class Traits>
	void mfDelaunay2D<Traits>::lockCells(ids id1, ids id2, sCell& c1, sCell& c2)
	{
		if (id1 < id2) {
			c1.lock();
			c2.lock();
		}
		else {
			c2.lock();
			c1.lock();
		}
	}

	template<class Traits>
	void mfDelaunay2D<Traits>::insertIn(ids idcell, ids idvertex)
	{
		sCell& c = mesh->getCell(idcell);
		ids vids[3] = { c.getVertex(0), c.getVertex(1), c.getVertex(2) };
		ids newcell[3], cellsids[3];
		mesh->delCell(idcell);

		newcell[0] = idvertex;
		newcell[1] = vids[0];
		newcell[2] = vids[1];
		cellsids[0] = mesh->addCell(newcell);

		newcell[0] = idvertex;
		newcell[1] = vids[1];
		newcell[2] = vids[2];
		cellsids[1] = mesh->addCell(newcell);

		newcell[0] = idvertex;
		newcell[1] = vids[2];
		newcell[2] = vids[0];
		cellsids[2] = mesh->addCell(newcell);

		legalizeEdge(cellsids[0], mesh->getCell(cellsids[0]).getVertexIndex(
			idvertex));
		legalizeEdge(cellsids[1], mesh->getCell(cellsids[0]).getVertexIndex(
			idvertex));
		legalizeEdge(cellsids[2], mesh->getCell(cellsids[0]).getVertexIndex(
			idvertex));
	}

	template<class Traits>
	void mfDelaunay2D<Traits>::insertOut(ids c, ids v, int edge)
	{
		mfBoundaryCellCIterator2D<Traits> ibc(mesh);
		sVertex* vs[3];
		vs[2] = &(mesh->getVertex(v));
		ibc.initialize(c, edge);
		vs[0] = &(mesh->getVertex(ibc->getVertexId((ibc.getEdge() + 2) % 3)));
		vs[1] = &(mesh->getVertex(ibc->getVertexId((ibc.getEdge() + 1) % 3)));

		ids tempe, i;
		forward_list<ids> list, listw;

		do {
			--ibc;
			tempe = (ibc.getEdge() + 1) % 3;
			if (find(list.begin(), list.end(), ibc->getVertexId((tempe + 1) % 3))
				== list.end())
				list.insert_after(list.end(), ibc->getVertexId((tempe + 1) % 3));
			vs[0] = mesh->getVertex(ibc->getVertexId((ibc.getEdge() + 2) % 3));
			vs[1] = mesh->getVertex(ibc->getVertexId((ibc.getEdge() + 1) % 3));
		} while (geom.inLeft(vs[0], vs[1], vs[2]) == 1);

		int vids[3];
		for (i = 0; i < (list.size() - 1); ++i) {
			vids[0] = v;
			vids[1] = *(list.begin() + i);
			vids[2] = *(list.begin() + i + 1);
			listw.insert_after(listw.end(), mesh->addCell(vids));
		}

		for (i = 0; i < listw.size(); ++i)
			legalizeEdge(*(listw.begin() + i), mesh->getCell(*(listw.begin()
				+ i)).getVertexIndex(v));
	}

	template<class Traits>
	void mfDelaunay2D<Traits>::insertOn(ids c, ids idvertex, int edge)
	{
		ids copposite;
		sCell& cell = mesh->getCell(c);
		copposite = cell.getMateId(edge);

		if (copposite >= 0) {
			sCell& cellop = mesh->getCell(copposite);
			ids vids[4];

			vids[0] = cell.getVertexId(edge);
			vids[1] = cell.getVertexId((edge + 1) % 3);
			vids[2] = cellop.getVertexMateId(c);
			vids[3] = cell.getVertexId((edge + 2) % 3);

			ids newcell[3], cellsids[4];
			mesh->delCell(c);
			mesh->delCell(copposite);

			newcell[0] = idvertex;
			newcell[1] = vids[0];
			newcell[2] = vids[1];
			cellsids[0] = mesh->addCell(newcell);

			newcell[0] = idvertex;
			newcell[1] = vids[1];
			newcell[2] = vids[2];
			cellsids[1] = mesh->addCell(newcell);

			newcell[0] = idvertex;
			newcell[1] = vids[2];
			newcell[2] = vids[3];
			cellsids[2] = mesh->addCell(newcell);

			newcell[0] = idvertex;
			newcell[1] = vids[3];
			newcell[2] = vids[0];
			cellsids[3] = mesh->addCell(newcell);

			legalizeEdge(cellsids[0], mesh->getCell(cellsids[0]).getVertexIndex(
				idvertex));
			legalizeEdge(cellsids[1], mesh->getCell(cellsids[1]).getVertexIndex(
				idvertex));
			legalizeEdge(cellsids[2], mesh->getCell(cellsids[2]).getVertexIndex(
				idvertex));
			legalizeEdge(cellsids[3], mesh->getCell(cellsids[3]).getVertexIndex(
				idvertex));
		}
		else {
			ids vids[3];

			vids[0] = cell.getVertexId(edge);
			vids[1] = cell.getVertexId((edge + 1) % 3);
			vids[2] = cell.getVertexId((edge + 2) % 3);

			ids newcell[3], cellsids[2];
			mesh->delCell(c);

			newcell[0] = idvertex;
			newcell[1] = vids[0];
			newcell[2] = vids[1];
			cellsids[0] = mesh->addCell(newcell);

			newcell[0] = idvertex;
			newcell[1] = vids[2];
			newcell[2] = vids[0];
			cellsids[1] = mesh->addCell(newcell);

			legalizeEdge(cellsids[0], mesh->getCell(cellsids[0]).getVertexIndex(
				idvertex));
			legalizeEdge(cellsids[1], mesh->getCell(cellsids[1]).getVertexIndex(
				idvertex));
		}
	}

#undef SPACE
#undef IDS
#undef SMESH

}
