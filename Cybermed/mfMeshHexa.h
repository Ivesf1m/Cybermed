#pragma once

#include <algorithm>
#include "mfMesh.h"
#include "mfMeshOper.h"
#include "mfSing.h"
#include "mfVertexStarIterator3D.h"

using std::find;

namespace mf
{
	/**
	*	Operation class for hexahedra in 3d space (no orientation)
	*	Traits must have ids, sVertex, sCell, sSing and sMesh.
	*/
	template<class Traits>
	class mfMeshHexa : public mfMeshOper<Traits>
	{
	public:
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sGeometric sGeometric;
		typedef mfSing<Traits> sSing;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfMeshHexa(sMesh* _mesh);

		/** Destructor */
		~mfMeshHexa();

		/**
		*	Adds cell in mesh
		*	@param idcell: the cell id
		*	@param idvertices: vector with vertices ids
		*/
		void addCell(ids idcell, vector<ids>& idvertices);

		/**
		*	Deletes a cell
		*	@param idcell: the cell id
		*/
		void delCell(ids idcell);

	private:
		sGeometric geom;

		void searchFace(int idcell, int idvertex);
		void searchSings(int idvertex);
		bool setMate(int idcell, int idcellmate, int iq[]);
		int sameVertices(int idcell1, int idcell2, int iq[]);

		void lockVertices(ids* idv, sVertex** v);
		void unlockVertices(ids* idv, sVertex** v);

		void addSing(ids idcell, sVertex* v, ids idvertex);
	};

	template<class Traits>
	mfMeshHexa<Traits>::mfMeshHexa(sMesh* _mesh)
		: mfMeshOper<Traits>(_mesh)
	{
		geom.setMesh(_mesh);
	}

	template<class Traits>
	mfMeshHexa<Traits>::~mfMeshHexa()
	{

	}

	template<class Traits>
	void mfMeshHexa<Traits>::addCell(ids idcell, vector<ids>& idvertices)
	{
		assert((idvertices[0] >= 0) && (idvertices[1] >= 0));
		assert((idvertices[2] >= 0) && (idvertices[3] >= 0));
		assert((idvertices[4] >= 0) && (idvertices[5] >= 0));
		assert((idvertices[6] >= 0) && (idvertices[7] >= 0));

		sCell& c = mesh->getCell(idcell);
		sVertex* v[8] = { &(mesh->getVertex(idvertices[0])),
			&(mesh->getVertex(idvertices[1])),
			&(mesh->getVertex(idvertices[2])),
			&(mesh->getVertex(idvertices[3])),
			&(mesh->getVertex(idvertices[4])),
			&(mesh->getVertex(idvertices[5])),
			&(mesh->getVertex(idvertices[6])),
			&(mesh->getVertex(idvertices[7])) };

		int ns = 0; //number of vertices with singularity 0
		int temp;

		lockVertices(idvertices.data(), v);
		c.lock();

		c.clearMatex();

		c.setVertexId(0, idvertices[0]);
		c.setVertexId(1, idvertices[1]);
		c.setVertexId(2, idvertices[2]);
		c.setVertexId(3, idvertices[3]);
		c.setVertexId(4, idvertices[4]);
		c.setVertexId(5, idvertices[5]);
		c.setVertexId(6, idvertices[6]);
		c.setVertexId(7, idvertices[7]);

		c.setMateId(0, -1);
		c.setMateId(1, -1);
		c.setMateId(2, -1);
		c.setMateId(3, -1);
		c.setMateId(4, -1);
		c.setMateId(5, -1);

		addSing(idcell, v[0], idvertices[0]);
		addSing(idcell, v[1], idvertices[1]);
		addSing(idcell, v[2], idvertices[2]);
		addSing(idcell, v[3], idvertices[3]);
		addSing(idcell, v[4], idvertices[4]);
		addSing(idcell, v[5], idvertices[5]);
		addSing(idcell, v[6], idvertices[6]);
		addSing(idcell, v[7], idvertices[7]);

		unlockVertices(idvertices.data(), v);
		c.unlock();
	}

	template<class Traits>
	void mfMeshHexa<Traits>::addSing(ids idcell, sVertex* v, ids idvertex)
	{
		if (v->getNumberOfSings() == 0)
			v->addSing(idcell);
		else {
			searchFace(idcell, idvertex);
			v->addSing(idcell);
		}
	}

	template<class Traits>
	void mfMeshHexa<Traits>::delCell(ids idcell)
	{

	}

	template<class Traits>
	void mfMeshHexa<Traits>::searchFace(int idcell, int idvertex)
	{
		assert(mesh->getVertex(idvertex).getNumberOfSings());
		mfVertexStarIterator3D<Traits> ic(mesh);
		vector<ids> list;

		for (ic.initialize(idvertex); ic.notFinished(); ++ic)
			list.push_back(&ic);

		int iq[4], ig;
		ids c;

		while (!list.empty()) {
			c = list.back();
			list.pop_back();
			ig = sameVertices(c, idcell, iq);

			if ((c != idcell) && (ig == 3)) {
				setMate(idcell, c, iq);
				setMate(c, idcell, iq);
			}			
		}
	}

	template<class Traits>
	int mfMeshHexa<Traits>::sameVertices(int idcell1, int idcell2, int iq[4])
	{
		int i, j, v1, v2, nv = 0;
		sCell& c1 = mesh->getCell(idcell1);
		sCell& c2 = mesh->getCell(idcell2);

		for (i = 0; i < 7; ++i) {
			v1 = c1.getVertexId(i);
			for (j = 0; j < 8; ++j) {
				v2 = c2.getVertexId(j);
				if (v1 == v2) {
					iq[nv++] = v1;
					break;
				}
			}
			if (nv == 3)
				return 3;
		}

		return nv;
	}

	template<class Traits>
	bool mfMeshHexa<Traits>::setMate(int idcell, int idcellmate, int iq[])
	{
		int temp;
		int v0, v1, v2;
		sCell& c = mesh->getCell(idcell);

		v0 = c.getVertexIndex(iq[0]);
		v1 = c.getVertexIndex(iq[1]);
		v2 = c.getVertexIndex(iq[2]);

		c.setMateId(c.getFaceIndex(v0, v1, v2), idcellmate);
		return true;
	}

	template<class Traits>
	void mfMeshHexa<Traits>::searchSings(int idvertex)
	{
		vector<ids> list;
		vector<ids> flags;
		sVertex& v = mesh->getVertex(idvertex);
		sSing* s = v.getSing();
		sSing* ants = s;
		ids currentIndex;

		list.push_back(s->getCell());
		flags.push_back(s->getCell());
		s = s->getNext();

		while (!list.empty()) {
			currentIndex = list.front();
			list.erase(list.begin());
			sCell& current = mesh->getCell(currentIndex);

			for (int i = 0; i < 4; ++i) {
				if ((current.getMateId(i) >= 0) &&
					(current.getVertexId(i) != idvertex) &&
					(find(flags.begin(), flags.end(), current.getMateId(i)) ==
						flags.end())) {
					list.push_back(current.getMateId(i));
					flags.push_back(current.getMateId(i));
				}
			}

			while ((s != NULL) && (list.empty())) {
				if (find(flags.begin(), flags.end(), s->getCell()) ==
					flags.end()) {
					list.push_back(s->getCell());
					flags.push_back(s->getCell());
					ants = s;
					s = s->getNext();
				}
				else {
					ants->setNext(s->getNext());
					delete s;
					s = ants->getNext();
				}
			}
		}
		list.clear();
		flags.clear();
	}

	template<class Traits>
	void mfMeshHexa<Traits>::lockVertices(ids* idv, sVertex** v)
	{
		ids order[8];
		int temp, i, j;

		for (i = 0; i < 8; ++i) {
			order[i] = i;
			for (j = i; j > 0; j--) {
				if (idv[order[j - 1]] > idv[order[j]]) {
					temp = order[j];
					order[j] = order[j - 1];
					order[j - 1] = temp;
				}
			}
		}

		v[order[0]]->lock();
		v[order[1]]->lock();
		v[order[2]]->lock();
		v[order[3]]->lock();
		v[order[4]]->lock();
		v[order[5]]->lock();
		v[order[6]]->lock();
		v[order[7]]->lock();
	}

	template<class Traits>
	void mfMeshHexa<Traits>::unlockVertices(ids* idv, sVertex** v)
	{
		ids order[8];
		int temp, i, j;

		for (i = 0; i < 8; ++i) {
			order[i] = i;
			for (j = i; j > 0; j--) {
				if (idv[order[j - 1]] > idv[order[j]]) {
					temp = order[j];
					order[j] = order[j - 1];
					order[j - 1] = temp;
				}
			}
		}

		v[order[0]]->unlock();
		v[order[1]]->unlock();
		v[order[2]]->unlock();
		v[order[3]]->unlock();
		v[order[4]]->unlock();
		v[order[5]]->unlock();
		v[order[6]]->unlock();
		v[order[7]]->unlock();
	}
}