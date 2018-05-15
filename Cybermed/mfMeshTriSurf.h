#pragma once

#include "mfMesh.h"
#include "mfMeshOper.h"
#include "mfSing.h"

namespace mf
{
	/**
	*	Operation class for triangular surfaces in 3D space (oriented)
	*	Traits must have ids, sVertex, sCell, sSing and sMesh
	*/
	template<class Traits>
	class mfMeshTriSurface : public mfMeshOper<Traits>
	{
	public :
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;
		typedef mfSing<Traits> sSing;

		/** Constructor */
		mfMeshTriSurface(sMesh* _mesh);

		/** Destructor */
		virtual ~mfMeshTriSurface();

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
	};

	template<class Traits>
	mfMeshTriSurface<Traits>::mfMeshTriSurface(sMesh* _mesh)
		: mfMeshOper<Traits>(_mesh)
	{

	}

	template<class Traits>
	mfMeshTriSurface<Traits>::~mfMeshTriSurface()
	{

	}

	template<class Traits>
	void mfMeshTriSurface<Traits>::addCell(ids idcell, vector<ids>& idvertices)
	{
		assert((idvertices[0] >= 0) && (idvertices[1] >= 0) &&
			(idvertices[2] >= 0));
		assert((idvertices[0] != idvertices[1]) && (idvertices[0] !=
			idvertices[2]) && (idvertices[1] != idvertices[2]));

		sCell& c = mesh->getCell(idcell);
		ids* idv = idvertices.data();
		sVertex* v[3] = { &(mesh->getVertex(idv[0])),
			&(mesh->getVertex(idv[1])),
			&(mesh->getVertex(idv[2])) };
		sSing* s;
		ids icop;
		int i, pos;

		c.clearMates();
		c.setVertexId(0, idv[0]);
		c.setVertexId(1, idv[1]);
		c.setVertexId(2, idv[2]);

		for (i = 0; i < 3; ++i) {
			s = v[(i + 2) % 3]->getSing();

			while (s) {
				icop = s->getCell();
				sCell& cop = mesh->getCell(icop);
				if (cop.getVertexId((cop.getVertexIndex(idv[(i + 2) % 3]) + 1)
					% 3) == idv[(i + 1) % 3]) {
					c.setMateId(i, icop);
					cop.setMateId((cop.getVertexIndex(idv[(i + 2) % 3]) + 2)
						% 3, idcell);
					s = NULL;
				}
				else
					s = s->getNext();
			}
		}

		for (i = 0; i < 3; ++i) {
			if (c.getMateId((i + 2) % 3) < 0) {
				if (c.getMateId((i + 1) % 3) >= 0) {
					pos = v[i]->inSings(c.getMateId((i + 1) % 3));
					v[i]->setSingCell(pos, idcell);
				}
				else
					v[i]->addSing(idcell);
			}
			else {
				if (c.getMateId((i + 1) % 3) >= 0) {
					icop = c.getMateId((i + 1) % 3);
					pos = v[i]->inSings(icop);
					while ((icop >= 0) && (icop != idcell)) {
						sCell& cop = mesh->getCell(icop);
						icop = cop.getMateId((cop.getVertexIndex(idv[i]) + 1) % 3);
						if (icop < 0)
							v[i]->delSing(pos);
					}
				}
			}
		}
	}

	template<class Traits>
	void mfMeshTriSurface<Traits>::delCell(ids idcell)
	{
		int i, pos;
		sCell& c = mesh->getCell(idcell);
		sVertex* v[3] = { &(mesh->getVertex(c.getVertexId(0))),
			&(mesh->getVertex(c.getVertexId(1))),
			&(mesh->getVertex(c.getVertexId(2))) };
		ids icop;

		for (i = 0; i < 3; ++i) {
			if (c.getMateId((i + 2) % 3) >= 0) {
				if (c.getMateId((i + 1) % 3) >= 0) {
					icop = c.getMateId((i + 1) % 3);
					while ((icop >= 0) && (icop != idcell)) {
						sCell& cop = mesh->getCell(icop);
						icop = cop.getMateId((cop.getVertexIndex(c.getVertexId(i))
							+ 1) % 3);
						if (icop < 0)
							v[i]->addSing(c.getMateId((i + 1) % 3));
						else if (icop == idcell)
							v[i]->getSing()->setCell(c.getMateId((i + 1) % 3));
					}
				}
			}
			else {
				pos = v[i]->inSings(idcell);
				if (c.getMateId((i + 1) % 3) >= 0)
					v[i]->setSingCell(pos, c.getMateId((i + 1) % 3));
				else
					v[i]->delSing(pos);
			}
		}

		for (i = 0; i < 3; ++i) {
			icop = c.getMateId(i);
			if (icop >= 0) {
				sCell& cop = mesh->getCell(icop);
				cop.setMateId(cop.getMateIndex(idcell), -1);
			}
		}

		c.setMateId(0, -1);
		c.setMateId(1, -1);
		c.setMateId(2, -1);
	}
}
