#pragma once

#include "mfMesh.h"
#include "mfMeshOper.h"
#include "mfSing.h"
#include "mfVertexStarIterator3D.h"

namespace mf
{
	/**
	*	Tetrahedron mesh operator class (oriented)
	*	Traits must have ids, sVertex, sCell, sSing and sMesh
	*/
	template<class Traits>
	class mfMeshTetra : public mfMeshOper<Traits>
	{
	public:
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sGeometric sGeometric;
		typedef typename Traits::sMesh sMesh;
		typedef mfSing<Traits> sSing;

		/** Constructor */
		mfMeshTetra(sMesh* _mesh);

		/** Destructor */
		virtual ~mfMeshTetra();

		/**
		*	Adds cell in mesh
		*	@param idcell: cell id
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
	};

	template<class Traits>
	mfMeshTetra<Traits>::mfMeshTetra(sMesh* _mesh)
		: mfMeshOper<Traits>(_mesh)
	{
		geom.setMesh(_mesh);
	}

	template<class Traits>
	mfMeshTetra<Traits>::~mfMeshTetra()
	{

	}

	template<class Traits>
	void mfMeshTetra<Traits>::addCell(ids idcell, vector<ids>& idvertices)
	{
		assert((idvertices[0] >= 0) && (idvertices[1] >= 0) &&
			(idvertices[2] >= 0) && (idvertices[3] >= 0));
		assert((idvertices[0] != idvertices[1]) && (idvertices[0] !=
			idvertices[2]) && (idvertices[0] != idvertices[3]));
		assert((idvertices[1] != idvertices[2]) && (idvertices[1] !=
			idvertices[3]) && (idvertices[2] != idvertices[3]));

		sCell& c = mesh->getCell(idcell);
		ids* idv = idvertices.data();
		sVertex* v[4] = { &(mesh->getVertex(idv[0])),
			&(mesh->getVertex(idv[1])),
			&(mesh->getVertex(idv[2])),
			&(mesh->getVertex(idv[3])) };
		int ns = 0; //number of vertices with singularity 0
		int temp;
		int i0 = 0, i1 = 1, i2 = 2, i3 = 3;
		int indTemp;

		lockVertices(idv, v);
		c.lock();

		c.clearMates();
		ns = geom.orientation3D(idv, v);
		assert(ns != 0);
		ns = 0;

		if (v[0]->getNumberOfSings() == 0)
			ns++;
		if (v[1]->getNumberOfSings() == 0) {
			indTemp = i0;
			i0 = i1;
			i1 = indTemp;
			ns++;
		}

		if (v[2]->getNumberOfSings() == 0) {
			indTemp = i0;
			i0 = i2;
			i2 = indTemp;
			ns++;
		}

		if (v[3]->getNumberOfSings() == 0) {
			indTemp = i0;
			i0 = i3;
			i3 = indTemp;
			ns++;
		}

		c.setVertexId(0, idv[0]);
		c.setVertexId(1, idv[1]);
		c.setVertexId(2, idv[2]);
		c.setVertexId(3, idv[3]);

		c.setMateId(0, -1);
		c.setMateId(1, -1);
		c.setMateId(2, -1);
		c.setMateId(3, -1);

		if (ns > 1) {
			//no shared faces
			v[0]->addSing(idcell);
			v[1]->addSing(idcell);
			v[2]->addSing(idcell);
			v[3]->addSing(idcell);
		}
		else if (ns == 1) {
			//only the face opposite to index1 can be shared
			searchFace(idcell, idv[i1]);
			v[i0]->addSing(idcell);
			if (c.getMateId(i0) < 0) {
				v[i1]->addSing(idcell);
				v[i2]->addSing(idcell);
				v[i3]->addSing(idcell);
			}
		}
		else {
			//any face can be shared
			searchFace(idcell, idv[i0]);
			searchFace(idcell, idv[i1]);

			if (c.getMateId(i0) < 0 && c.getMateId(i1) < 0) {
				v[i0]->addSing(idcell);
				v[i1]->addSing(idcell);
				v[i2]->addSing(idcell);
				v[i3]->addSing(idcell);
			}
			else {
				if (c.getMateId(i0) < 0) {
					v[i1]->addSing(idcell);
					v[i2]->addSing(idcell);
					v[i3]->addSing(idcell);
				}

				if (c.getMateId(i1) < 0) {
					v[i0]->addSing(idcell);
					v[i2]->addSing(idcell);
					v[i3]->addSing(idcell);
				}
			}

			searchSings(idv[i0]);
			searchSings(idv[i1]);
			searchSings(idv[i2]);
			searchSings(idv[i3]);
		}

		unlockVertices(ids, v);
		c.unlock();
	}

	template<class Traits>
	void mfMeshTetra<Traits>::delCell(ids idcell)
	{

	}

	template<class Traits>
	void mfMeshTetra<Traits>::searchFace(int idcell, int idvertex)
	{
		assert(mesh->getVertex(idvertex).getNumberOfSings());
		mfVertexStarIterator3D<Traits> ic(mesh);

		int iq[4], ig;

		for (ic.initialize(idvertex); ic.notFinished(); ++i) {
			ig = sameVertices(&ic, idcell, iq);
			if ((&ic != idcell) && (ig == 3)) {
				setMate(idcell, &ic, iq);
				setMate(&ic, idcell, iq);
			}
		}
	}

	template<class Traits>
	int mfMeshTetra<Traits>::sameVertices(int idcell1, int idcell2, int iq[])
	{
		int i, j, v1, v2, nv = 0;
		sCell& c1 = mesh->getCell(idcell1);
		sCell& c2 = mesh->getCell(idcell2);

		for (i = 0; i < 4; ++i) {
			v1 = c1.getVertexId(i);
			for (j = 0; j < 4; ++j) {
				v2 = c2.getVertexId(j);
				if (v1 == v2) {
					iq[nv++] = v1;
					break;
				}
			}
		}

		return nv;
	}

	template<class Traits>
	bool mfMeshTetra<Traits>::setMate(int idcell, int idcellmate, int iq[])
	{
		int temp;
		for (int i = 0; i < 4; ++i) {
			temp = mesh->getCell(idcell).getVertexId(i);
			if ((temp != iq[0]) && (temp != iq[1]) && (temp != iq[2])) {
				if (temp >= 0) {
					mesh->getCell(idcell).setMateId(i, idcellmate);
					return true;
				}
				else
					break;
			}
		}

		return false;
	}

	template<class Traits>
	void mfMeshTetra<Traits>::searchSings(int idvertex)
	{
		forward_list<ids> list;
		forward_list<ids> flags;
		sVertex& v = mesh->getVertex(idvertex);
		sSing* s = v->getSing();
		sSing* ants = s;
		int i;
		ids currentIndex;

		list.insert_after(list.end(), s->getCell());
		flags.insert_after(flags.end(), s->getCell());
		s = s->getNext();

		while (!list.empty()) {
			currentIndex = list.front();
			list.pop_front();
			sCell& current = mesh->getCell(currentIndex);

			for (i = 0; i < 4; ++i) {
				if ((current.getMateId(i) >= 0) && (current.getVertexId(i) !=
					idvertex) && (find(flags.begin(), flags.end(),
						current.getMateId(i)))) {
					list.insert_after(list.end(), current.getMateId(i));
					flags.insert_after(flags.end(), current.getMateId(i));
				}
			}

			while ((s != NULL) && (list.empty())) {
				if (find(flags.begin(), flags.end(), s->getCell()) ==
					flags.end()) {
					list.insert_after(list.end(), s->getCell());
					flags.insert_after(flags.end(), s->getCell());
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
	void mfMeshTetra<Traits>::lockVertices(ids* idv, sVertex** v)
	{
		ids order[4];
		int temp, i, j;

		for (i = 0; i < 4; ++i) {
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
	}

	template<class Traits>
	void mfMeshTetra<Traits>::unlockVertices(ids* idv, sVertex** v)
	{
		ids order[4];
		int temp, i, j;

		for (i = 0; i < 4; ++i) {
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
	}
}