#pragma once

#include <algorithm>
#include <forward_list>
#include "mfMesh.h"
#include "mfMeshOper.h"
#include "mfSing.h"
#include "mfVertexStarIteratorTriSurf.h"

using std::find;
using std::forward_list;

namespace mf
{
	/**
	*	Operation class for triangles in 3d space (not oriented)
	*	Traits must have ids, sVertex, sCell, sSing and sMesh
	*/
	template<class Traits>
	class mfMeshNOSurface : public mfMeshOper<Traits>
	{
	public:
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;
		typedef mfSing<Traits> sSing;

		/** Constructor */
		mfMeshNOSurface(sMesh* _mesh);

		/** Destructor */
		virtual ~mfMeshNOSurface();

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

		void searchFace(int idcell, int idvertex);
		void searchSings(int idvertex);
		bool setMate(int idcell, int idcellmate, int iq[]);
		int sameVertices(int idcell, int idcell2, int iq[]);
	};

	template<class Traits>
	mfMeshNOSurface<Traits>::mfMeshNOSurface(sMesh* _mesh)
		: mfMeshOper(_mesh)
	{

	}

	template<class Traits>
	mfMeshNOSurface<Traits>::~mfMeshNOSurface()
	{

	}

	template<class Traits>
	void mfMeshNOSurface<Traits>::addCell(ids idcell, vector<ids>& idvertices)
	{
		assert((idvertices[0] >= 0) && (idvertices[1] >= 0) &&
			(idvertices[2] >= 0));
		assert((idvertices[0] != idvertices[1]) && (idvertices[0] !=
			idvertices[2]) && (idvertices[1] != idvertices[2]));

		sCell& c = mesh->getCell(idcell);
		ids idv[3] = { idvertices[0], idvertices[1], idvertices[2] };
		sVertex* v[3] = { &(mesh->getVertex(idv[0])),
			&(mesh->getVertex(idv[1])),
			&(mesh->getVertex(idv[2])) };
		int ns = 0; //number of vertices with singularity 0
		int temp;

		if (v[0]->getNumberOfSings() == 0)
			ns++;

		if (v[1]->getNumberOfSings() == 0) {
			temp = idv[0];
			idv[0] = idv[1];
			idv[1] = temp;
			ns++;
		}

		if (v[2]->getNumberOfSings() == 0) {
			temp = idv[0];
			idv[0] = idv[2];
			idv[2] = temp;
			ns++;
		}

		c.setVertexId(0, idv[0]);
		c.setVertexId(1, idv[1]);
		c.setVertexId(2, idv[2]);

		c.setMateId(0, -1);
		c.setMateId(1, -1);
		c.setMateId(2, -1);

		v[0] = mesh->getVertex(idv[0]);
		v[1] = mesh->getVertex(idv[1]);
		v[2] = mesh->getVertex(idv[2]);

		if (ns > 1) {
			//no common face
			v[0]->addSing(idcell);
			v[1]->addSing(idcell);
			v[2]->addSing(idcell);
		}
		else if(ns == 1){
			//only the face opposite to index1 can be shared
			searchFace(idcell, idv[1]);
			v[0]->addSing(idcell);
			if (c->getMateId(0) < 0) {
				v[1]->addSing(idcell);
				v[2]->addSing(idcell);
			}
		}
		else {
			//any face can be shared
			searchFace(idcell, idv[0]);
			searchFace(idcell, idv[1]);

			if (c.getMateId(0) < 0) {
				//no shared face was found
				v[1]->addSing(idcell);
				v[2]->addSing(idcell);
			}

			if (c.getMateId(1) < 0) {
				//no shared face was found
				v[0]->addSing(idcell);
				v[2]->addSing(idcell);
			}

			searchSings(idv[0]);
			searchSings(idv[1]);
			searchSings(idv[2]);
		}
	}

	template<class Traits>
	void mfMeshNOSurface<Traits>::delCell(ids idcell)
	{

	}

	template<class Traits>
	void mfMeshNOSurface<Traits>::searchFace(int idcell, int idvertex)
	{
		assert(mesh->getVertex(idvertex).getNumberOfSings());
		mfVertexStarIteratorTriSurf<Traits> ic(mesh);
		forward_list<ids> list;

		for (ic.initialize(idvertex); ic.notFinished(); ++ic) {
			assert(find(list.begin(), list.end(), &ic) == list.end());
			list.insert_after(list.end(), &ic);
		}

		int iq[3], ig;
		ids c;

		while (!list.empty()) {
			c = list.front();
			list.pop_front();

			ig = sameVertices(c, idcell, iq);
			assert(!((c != idcell) && (ig == 3)));

			if ((c != idcell) && (ig == 2)) {
				setMate(idcell, c, iq);
				setMate(c, idcell, iq);
			}
		}
	}

	template<class Traits>
	int mfMeshNOSurface<Traits>::sameVertices(int idcell1, int idcell2, int iq[])
	{
		int i, j, v1, v2, nv = 0;

		for (i = 0; i < 3; ++i) {
			v1 = mesh->getCell(idcell1).getVertexId(i);
			for (j = 0; j < 3; ++j) {
				v2 = mesh->getCell(idcell2).getVertexId(j);
				if (v1 == v2) {
					iq[nv++] = v1;
					break;
				}
			}
		}

		return nv;
	}

	template<class Traits>
	bool mfMeshNOSurface<Traits>::setMate(int idcell, int idcellmate, int iq[])
	{
		for (int i = 0; i < 3; ++i) {
			if ((mesh->getCell(idcell).getVertexId(i) != iq[0]) &&
				(mesh->getCell(idcell).getVertexId(i) != iq[1])) {
				if (mesh->getCell(idcell).getMateId(i) < 0) {
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
	void mfMeshNOSurface<Traits>::searchSings(int idvertex)
	{
		forward_list<ids> list;
		forward_list<ids> flags;
		sVertex& v = mesh->getVertex(idvertex);
		sSing* s = v.getSing();
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

			for (i = 0; i < 3; ++i) {
				if ((current.getMateId(i) >= 0) &&
					(current.getVertexId(i) != idvertex) &&
					(find(flags.begin(), flags.end(), current.getMateId(i))
						== flags.end())) {
					list.insert_after(list.end(), current.getMateId(i));
					flags.insert_after(flags.end(), current.getMateId(i));
				}
			}

			while ((s != NULL) && (list.empty())) {
				if (find(flags.begin(), flags.end(), s->getCell()) ==
					flags.end()) {
					list.insert_after(list.end(), s->getCell());
					flags.insert_after(flags.end(), s->getCell();
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
}
