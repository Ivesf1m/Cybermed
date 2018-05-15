#pragma once

#include "mfMeshOper.h"
#include "mfSing.h"

namespace mf
{
	/**
	*	Operation class for triangles in 2D space
	*/
	template<class Traits>
	class mfMesh2D : public mfMeshOper<Traits>
	{
	public:
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sGeometric sGeometric;
		typedef typename Traits::sTopology sTopology;
		typedef mfSing<Traits> sSing;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfMesh2D(sMesh* _mesh);

		/** Destructor */
		virtual ~mfMesh2D();

		/**
		*	Adds cell in mesh
		*	@param idcell: cell id
		*	@param idvertices: vector with vertices ids of the cell
		*/
		void addCell(ids idcell, vector<ids>& idvertices);
		void addCell(ids idcell, vector<ids>& idvertices, vector<ids>& idops);

		/**
		*	Deletes a cell
		*	@param idcell: cell id
		*/
		void delCell(ids idcell);

	private:
		sGeometric geom;

		void lockVertices(ids* idv, sVertex** v);
		void unlockVertices(ids* idv, sVertex** v);
	};

	template<class Traits>
	mfMesh2D<Traits>::mfMesh2D(sMesh* _mesh)
		: mfMeshOper<Traits>(_mesh)
	{
		geom.setMesh(_mesh);
	}

	template<class Traits>
	mfMesh2D<Traits>::~mfMesh2D()
	{

	}

	template<class Traits>
	void mfMesh2D<Traits>::addCell(ids idcell, vector<ids>& idvertices)
	{
		assert((idvertices[0] >= 0) && (idvertices[1] >= 0) && (idvertices[2]
			>= 0));
		assert((idvertices[0] != idvertices[1]) && (idvertices[0] !=
			idvertices[2]) && (idvertices[1] != idvertices[2]));

		sCell& c = mesh->getCell(idcell);
		ids idv[3] = { idvertices[0], idvertices[1], idvertices[2] };
		sVertex* v[3] = {&(mesh->getVertex(idv[0])), &(mesh->getVertex(idv[1])),
						&(mesh->getVertex(idv[2])) };

		c.lock();
		lockVertices(idv, v);

		ids icop;
		int i, pos;

		c.clearMates();

		i = geom.orientation2D(idv, v);
		assert(i != 0);

		c.setVertexId(0, idv[0]);
		c.setVertexId(1, idv[1]);
		c.setVertexId(2, idv[2]);

		for (i = 0; i < 3; ++i) {
			sSing& s = v[(i + 2) % 3]->getSing();
			while (s) {
				icop = s.getCell();
				sCell& cop = mesh->getCell(icop);
				if (cop.getVertexId((cop.getVertexIndex(idv[(i + 2) % 3]) + 1) % 3)
					== idv[(i + 1) % 3]) {
					c.setMateId(i, icop);
					cop.setMateId((cop.getVertexIndex(idv[(i + 2) % 3]) + 2)
						% 3, idcell);
				}
				else
					s = s.getNext();
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

		unlockVertices(idv, v);
		c.unlock();
	}

	template<class Traits>
	void mfMesh2D<Traits>::addCell(ids idcell, vector<ids>& idvertices,
		vector<ids>& idops)
	{
		assert((idvertices[0] >= 0) && (idvertices[1] >= 0) && (idvertices[2]
			>= 0));
		assert((idvertices[0] != idvertices[1]) && (idvertices[0] !=
			idvertices[2]) && (idvertices[1] != idvertices[2]));

		sCell& c = mesh->getCell(idcell);
		sVertex* v[3] = { &(mesh->getVertex(idv[0])), &(mesh->getVertex(idv[1])),
			&(mesh->getVertex(idv[2])) };

		c.lock();
		lockVertices(idvertices, v);

		c.setVertexId(0, idvertices[0]);
		c.setVertexId(1, idvertices[1]);
		c.setVertexId(2, idvertices[2]);

		c.setMateId(0, idops[0]);
		c.setMateId(1, idops[1]);
		c.setMateId(2, idops[2]);

		unlockVertices(idvertices, v);
		c.unlock();
	}

	template<class Traits>
	void mfMesh2D<Traits>::delCell(ids idcell)
	{
		int i, pos;
		sCell& c = mesh->getCell(idcell);
		ids idv[3] = { idvertices[0], idvertices[1], idvertices[2] };
		sVertex* v[3] = { &(mesh->getVertex(idv[0])), &(mesh->getVertex(idv[1])),
			&(mesh->getVertex(idv[2])) };
		ids icop;

		c.lock();
		lockVertices(idv, v);

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

		unlockVertices(idv, v);
		c.unlock();
	}

	template<class Traits>
	void mfMesh2D<Traits>::lockVertices(ids* idv, sVertex** v)
	{
		if (idv[0] < idv[1]) {
			if (idv[0] < idv[2]) {
				if (idv[1] < idv[2]) {
					v[0]->lock();
					v[1]->lock();
					v[2]->lock();
				}
				else {
					v[0]->lock();
					v[2]->lock();
					v[1]->lock();
				}
			}
			else {
				v[2]->lock();
				v[0]->lock();
				v[1]->lock();
			}
		}
		else {
			if (idv[0] > idv[2]) {
				if (idv[1] < idv[2]) {
					v[1]->lock();
					v[2]->lock();
					v[0]->lock();
				}
				else {
					v[2]->lock();
					v[1]->lock();
					v[0]->lock();
				}
			}
			else {
				v[1]->lock();
				v[0]->lock();
				v[2]->lock();
			}
		}
	}

	template<class Traits>
	void mfMesh2D<Traits>::unlockVertices(ids* idv, sVertex** v)
	{
		if (idv[0] < idv[1]) {
			if (idv[0] < idv[2]) {
				if (idv[1] < idv[2]) {
					v[2]->unlock();
					v[1]->unlock();
					v[0]->unlock();
				}
				else {
					v[1]->unlock();
					v[2]->unlock();
					v[0]->unlock();
				}
			}
			else {
				v[1]->unlock();
				v[0]->unlock();
				v[2]->unlock();
			}
		}
		else {
			if (idv[0] > idv[2]) {
				if (idv[1] < idv[2]) {
					v[0]->unlock();
					v[2]->unlock();
					v[1]->unlock();
				}
				else {
					v[0]->unlock();
					v[1]->unlock();
					v[2]->unlock();
				}
			}
			else {
				v[2]->unlock();
				v[0]->unlock();
				v[1]->unlock();
			}
		}
	}
}
