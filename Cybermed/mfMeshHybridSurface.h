#pragma once

#include "mfMesh.h"
#include "mfMeshOper.h"
#include "mfSing.h"
#include <vector>

using std::vector;

namespace mf
{
	/**
	*	Operation class for hybrid surfaces in 3d space
	*	Traits must have ids, sVertex, sCell, sSing, sMesh
	*/
	template<class Traits>
	class mfMeshHybridSurface : public mfMeshOper<Traits>
	{
	public:
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;
		typedef mfSing<Traits> sSing;

		/** Constructor */
		mfMeshHybridSurface(sMesh* _mesh);

		/** Destructor */
		virtual ~mfMeshHybridSurface();

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
	mfMeshHybridSurface<Traits>::mfMeshHybridSurface(sMesh* _mesh)
		: mfMeshOper<Traits>(_mesh)
	{

	}

	template<class Traits>
	mfMeshHybridSurface<Traits>::~mfMeshHybridSurface()
	{

	}

	template<class Traits>
	void mfMeshHybridSurface<Traits>::addCell(ids idcell, vector<ids>& idvertices)
	{
		//idvertices[3] is -1 if it is a triangle, >= 0 otherwise
		assert((idvertices[0] >= 0) && (idvertices[1] >= 0) && (idvertices[2]
			>= 0) && (idvertices[3] >= -1));
		assert((idvertices[0] != idvertices[1]) && (idvertices[0] !=
			idvertices[2]) && (idvertices[0] != idvertices[3]));
		assert((idvertices[1] != idvertices[2]) && (idvertices[1] !=
			idvertices[3]) && (idvertices[2] != idvertices[3]));

		sCell& c = mesh->getCell(idcell);
		ids* idv = idvertices.data();
		sVertex* v[4] = { &(mesh->getVertex(idvertices[0])),
			&(mesh->getVertex(idvertices[1])),
			&(mesh->getVertex(idvertices[2])) };

		if (idvertices[3] > -1) {
			v[3] = &(mesh->getVertex(idvertices[3])),
		}
		
		sSing* s;
		ids icop;
		ids idedge;
		int i, pos;

		c.clearMates();
		c.setVertexId(0, idvertices[0]);
		c.setVertexId(1, idvertices[1]);
		c.setVertexId(2, idvertices[2]);
		c.setVertexId(3, idvertices[3]);

		if (idcell[3] > -1) {
			for (i = 0; i < 4; ++i) {
				s = v[i]->getSing();

				while (s) {
					icop = s->getCell();
					sCell& cop = mesh->getCell(icop);
					if (((cop.getVertexId(3) + 1) ?
						(cop.getVertexId((cop.getVertexIndex(idv[i]) + 3) % 4)) :
						(cop.getVertexId((cop.getVertexIndex(idv[i]) + 2) % 3)))
						== idv[(i + 1) % 4]) {
						idedge = s->getCell();
						c.setEdgeId(i, idedge);
						cop.setEdgeId(cop.getVertexIndex(idv[(i + 1) % 4]),
							idedge);
						c.setMateId(i, icop);
						cop.setMateId(cop.getVertexIndex(idv[(i + 1) % 4]),
							idcell);
						s = NULL;
					}
					else
						s = s->getNext();
				}
			}

			for (i = 0; i < 4; ++i) {
				if (c.getMateId((i + 3) % 4) < 0) {
					if (c.getMateId(i) >= 0) {
						pos = v[i]->inSings(c.getMateId(i));
						v[i]->setSingCell(pos, idcell);
					}
					else
						v[i]->addSing(idcell);
				}
				else {
					if (c.getMateId(i) >= 0) {
						icop = c.getMateId(i);
						pos = v[i]->inSings(icop);
						while ((icop >= 0) && (icop != idcell)) {
							sCell& cop = mesh->getCell(icop);
							icop = cop.getMateId(cop.getVertexIndex(idv[i]));
							if (icop < 0)
								v[i]->delSing(pos);
						}
					}
				}
			}
		}
		else { //triangle
			for (i = 0; i < 3; ++i) {
				s = v[i]->getSing();
				while (s) {
					icop = s->getCell();
					sCell& cop = mesh->getCell(icop);
					if (((cop.getVertexId(3) == -1) ?
						(cop.getVertexId((cop.getVertexIndex(idv[i]) + 2) % 3)) :
						(cop.getVertexId((cop.getVertexIndex(idv[i]) + 3) % 4)))
						== idv[(i + 1) % 3]) {
						idedge = mesh->addEgde();
						c.setEdgeId(i, idedge);
						cop.setEdgeId(cop.getVertexIndex(idv[(i + 1) % 3]),
							idedge);
						c.setMateId(i, icop);
						cop.setMateId(cop.getVertexIndex(idv[(i + 1) % 3]),
							idcell);
						s = NULL;
					}
					else
						s = s->getNext();
				}
			}

			for (i = 0; i < 3; ++i) {
				if (c.getMateId((i + 2) % 3) < 0) {
					if (c.getMateId(i) >= 0) {
						pos = v[i]->inSings(c.getMateId(i));
						v[i]->setSingCell(pos, idcell);
					}
					else
						v[i]->addSing(idcell);
				}
				else {
					if (c.getMateId(i) >= 0) {
						icop = c.getMateId(i);
						pos = v[i]->inSings(icop);
						while ((icop >= 0) && (icop != idcell)) {
							sCell& cop = mesh->getCell(icop);
							icop = cop.getMateId(cop.getVertexIndex(idv[i]));
							if (icop < 0)
								v[i]->delSing(pos);
						}
					}
				}
			}
		}
	}

	template<class Traits>
	void mfMeshHybridSurface<Traits>::delCell(ids idcell)
	{
		int i, pos;
		sCell& c = mesh->getCell(idcell);
		sVertex* v[4] = { &(mesh->getVertex(c.getVertexId(0))),
			&(mesh->getVertex(c.getVertexId(1))),
			&(mesh->getVertex(c.getVertexId(2))),
			&(mesh->getVertex(c.getVertexId(3))) };
		ids icop;
		int numEdge = c.getNumberEdgesInCell();

		for (i = 0; i < numEdge; ++i) {
			if (numEdge == 3) { //triangle
				if (c.getMateId(i) >= 0) {
					icop = c.getMateId(i);
					while ((icop >= 0) && (icop != idcell)) {
						sCell& cop = mesh->getCell(icop);
						icop = cop.getMateId(cop.getVertexIndex(c.getVertexId(i)));
						if (icop < 0)
							v[i]->addSing(c.getMateId(i));
						else if (icop == idcell)
							v[i]->getSing()->setCell(c->getMateId(i));
					}
				}
				else {
					pos = v[i]->inSings(idcell);
					if (c.getMateId(i) >= 0)
						v[i]->setSingCell(pos, c.getMateId(i));
					else
						v[i]->delSing(pos);
				}
			}
			else {
				if (c.getMateId((i + 3) % 4) >= 0) {
					if (c.getMateId(i) >= 0) {
						icop = c.getMateId(i);
						while ((icop >= 0) && (icop != idcell)) {
							sCell& cop = mesh->getCell(icop);
							icop = cop.getMateId(cop.getVertexIndex(
								c.getVertexId(i)));
							if (icop < 0)
								v[i]->addSing(c.getMateId(i));
							else if (icop == idcell)
								v[i]->getSing()->setCell(c.getMateId(i));
						}
					}
				}
				else {
					pos = v[i]->inSings(idcell);
					if (c.getMateId(i) >= 0)
						v[i]->setSingCell(pos, c.getMateId(i));
					else
						v[i]->delSing(pos);
				}
			}
		}

		for (i = 0; i < numEdge; ++i) {
			icop = c.getMateId(i);
			if (icop >= 0) {
				sCell& cop = mesh->getCell(icop);
				cop.setMateId(cop.getMateIndex(idcell), -1);
			}
		}

		c.setMateId(0, -1);
		c.setMateId(1, -1);
		c.setMateId(2, -1);
		if(numEdge == 4)
			c.setMateId(3, -1);
	}
}