#pragma once

#include "mfMesh.h"

namespace mf
{
#define IDS typename mfTopology<Traits>::ids

	template<class Traits>
	class mfTopology
	{
	public:
		typedef typename Traits::ids ids;
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfTopology(sMesh* _mesh = NULL);

		/** Destructor */
		~mfTopology();

		/**
		*	Sets the pointer to the mesh that will be manipulated.
		*	@param _mesh: pointer to the mesh
		*/
		void setMesh(sMesh* _mesh);

		void flip2D(ids c1, ids c2);

		void addOnEdge(ids idcell, int index, ids idvertex);

		ids collapseEdge(ids idcell1, int index1);

	private:
		sMesh* mesh;
	};

	template<class Traits>
	mfTopology<Traits>::mfTopology(sMesh* _mesh)
		: mesh(_mesh)
	{

	}

	template<class Traits>
	mfTopology<Traits>::~mfTopology()
	{

	}

	template<class Traits>
	void mfTopology<Traits>::setMesh(sMesh* _mesh)
	{
		this->mesh = _mesh;
	}

	template<class Traits>
	void mfTopology<Traits>::flip2D(ids c1, ids c2)
	{
		sCell* temp = NULL;
		sCell& cell1 = mesh->getCell(c1);
		sCell& cell2 = mesh->getCell(c2);

		int e1 = cell1.getMateIndex(c2);
		int e2 = cell2.getMateIndex(c1);
		int pos;

		//updating opposites
		cell1.setMateId(e1, cell2.getMateId((e2 + 2) % 3));
		if (cell2.getMateId((e2 + 2) % 3) >= 0) {
			temp = &(mesh->getCell(cell2.getMateId((e2 + 2) % 3)));
			temp->setMateId(temp->getMateIndex(c2), c1);
		}

		cell2.setMateId(e2, cell1.getMateId((e1 + 2) % 3));
		if (cell1.getMateId((e1 + 2) % 3) >= 0) {
			temp = &(mesh->getCell(cell1.getMateId((e1 + 2) % 3)));
			temp->setMateId(temp->getMateIndex(c1), c2);
		}

		cell1.setMateId((e1 + 2) % 3, c2);
		cell2.setMateId((e2 + 2) % 3, c1);

		//updating vertices
		cell1.setVertexId((e1 + 1) % 3, cell2.getVertexId(e2));
		cell2.setVertexId((e2 + 1) % 3, cell1.getVertexId(e1));

		sVertex& v1 = mesh->getVertex(cell1.getVertexId(e1));
		pos = v1.inSings(c1);
		if (pos >= 0)
			v1.setSingCell(pos, c2);

		sVertex& v2 = mesh->getVertex(cell2.getVertexId(e2));
		pos = v2.inSings(c2);
		if (pos >= 0)
			v2.setSingCell(pos, c1);

		sVertex& v3 = mesh->getVertex(cell1.getVertexId((e1 + 2) % 3));
		pos = v3.inSings(c2);
		if (pos >= 0)
			v3.setSingCell(pos, c1);

		sVertex& v4 = mesh->getVertex(cell2.getVertexId((e2 + 2) % 3));
		pos = v4.inSings(c1);
		if (pos >= 0)
			v4.setSingCell(pos, c2);
	}

	template<class Traits>
	void mfTopology<Traits>::addOnEdge(ids idcell, int index, ids idvertex)
	{
		ids coposto;
		sCell& cell = mesh->getCell(idcell);
		coposto = cell.getMateId(index);

		//has opposite cell
		if (coposto >= 0) {
			ids vids[4];

			vids[0] = cell.getVertexId(index);
			vids[1] = cell.getVertexId((index + 1) % 3);
			vids[2] = mesh->getCell(oposto).getVertexMateId(idcell);
			vids[3] = cell.getVertexId((index + 2) % 3);

			ids newcell[3];

			mesh->delCell(idcell);
			mesh->delCell(coposto);

			newcell[0] = idvertex;
			newcell[1] = vids[0];
			newcell[2] = vids[1];
			mesh->addCell(newcell);

			newcell[0] = idvertex;
			newcell[1] = vids[1];
			newcell[2] = vids[2];
			mesh->addCell(newcell);

			newcell[0] = idvertex;
			newcell[1] = vids[2];
			newcell[2] = vids[3];
			mesh->addCell(newcell);

			newcell[0] = idvertex;
			newcell[1] = vids[3];
			newcell[2] = vids[0];
			mesh->addCell(newcell);
		}
		else {
			ids vids[3];

			vids[0] = cell.getVertexId(index);
			vids[1] = cell.getVertexId((index + 1) % 3);
			vids[2] = cell.getVertexId((index + 2) % 3);

			ids newcell[3];

			mesh->delCell(idcell);

			newcell[0] = idvertex;
			newcell[1] = vids[0];
			newcell[2] = vids[1];
			mesh->addCell(newcell);

			newcell[0] = idvertex;
			newcell[1] = vids[2];
			newcell[2] = vids[0];
			mesh->addCell(newcell);
		}
	}

	template<class Traits>
	IDS mfTopology<Traits>::collapseEdge(ids idcell1, int index1)
	{
		ids idcell2, idvertex1, idvertex2, idctemp, idcfirst;
		sCell& cell1 = mesh->getCell(idcell1);
		idcell2 = cell1.getMateId(index1);
		int sing;
		sVertex* v1;
		sVertex* v2;

		if (idcell2 >= 0) {
			sCell& cell2 = mesh->getCell(idcell2);
			ids index2 = cell2.getMateIndex(idcell1);
			idvertex1 = cell1.getVertexId((index1 + 1) % 3);
			idvertex2 = cell1.getVertexId((index1 + 2) % 3);

			assert((cell1.getMateId((index1 + 1) % 3) >= 0) ||
				(cell1.getMateId((index1 + 2) % 3) >= 0));
			assert((cell2.getMateId((index2 + 1) % 3) >= 0) ||
				(cell2.getMateId((index2 + 2) % 3) >= 0));

			v1 = &(mesh->getVertex(idvertex1));
			v2 = &(mesh->getVertex(idvertex2));
			sing = v2->getNumberOfSings();

			for (int i = 0; i < sing; ++i) {
				idctemp = v2->getSingCell(i);
				idcfirst = idctemp;

				do {
					sCell& ctemp = mesh->getCell(idctemp);
					idctemp = ctemp.getMateId((ctemp.getVertexIndex(idvertex2)
						+ 1) % 3);
					ctemp.setVertexId(ctemp.getVertexIndex(idvertex2),
						idvertex1);
				} while ((idctemp != idcfirst) && (idctemp >= 0));
			}

			idctemp = cell1.getMateId((index1 + 1) % 3);
			if (idctemp >= 0) {
				sCell& ctemp = mesh->getCell(idctemp);
				ctemp.setMateId(ctemp.getMateIndex(idcell1), 
					cell1.getMateId((index1 + 2) % 3));
			}

			idctemp = cell1.getMateId((index1 + 2) % 3);
			if (idctemp >= 0) {
				sCell& ctemp = mesh->getCell(idctemp);
				ctemp.setMateId(ctemp.getMateIndex(idcell1),
					cell1->getMateId((index1 + 1) % 3));
			}

			idctemp = cell2.getMateId((index2 + 2) % 3);
			if (idctemp >= 0) {
				sCell& ctemp = mesh->getCell(idctemp);
				ctemp.setMateId(ctemp.getMateIndex(idcell2),
					cell2->getMateId((index2 + 1) % 3));
			}

			idctemp = cell2.getMateId((index2 + 1) % 3);
			if (idctemp >= 0) {
				sCell& ctemp = mesh->getCell(idctemp);
				ctemp.setMateId(ctemp.getMateIndex(idcell2),
					cell2->getMateId((index2 + 2) % 3));
			}

			sVertex& vtemp1 = mesh->getVertex(cell1.getVertexId(index1));
			if ((sing = vtemp1.inSings(idcell1)) >= 0)
				vtemp1.setSingCell(sing, cell1->getMateId((index1 + 1) % 3));

			sVertex& vtemp2 = mesh->getVertex(cell2.getVertexId(index2));
			if ((sing = vtemp2.inSings(idcell2)) >= 0)
				vtemp2.setSingCell(sing, cell2->getMateId((index2 + 1) % 3));

			sVertex& vtemp3 = mesh->getVertex(cell1.getVertexId((index1 + 1)
				% 3));
			if ((sing = vtemp3.inSings(idcell2)) >= 0) {
				if (cell2.getMateId((index2 + 2) % 3) >= 0)
					vtemp3.setSingCell(sing, cell2.getMateId((index2 + 2) % 3));
				else 
					vtemp3.delSing(sing);
			}
			else {
				if (vtemp3.getNumberOfSings() == 1)
					vtemp3.clearSings();
			}

			sVertex& vtemp4 = mesh->getVertex(cell1.getVertexId((index1 + 2)
				% 3));
			if ((sing = vtemp4.inSings(idcell1)) >= 0) {
				if (cell1.getMateId((index1 + 2) % 3) >= 0)
					vtemp4.setSingCell(sing, cell1.getMateId((index1 + 2) % 3));
				else 
					vtemp4.delSing(sing);
			}
			else {
				if (vtemp4.getNumberOfSings() == 1)
					vtemp4.clearSings();
			}

			mesh->freeCell(idcell2);
		}
		else {
			idvertex1 = cell1.getVertexId((index1 + 1) % 3);
			idvertex2 = cell1.getVertexId((index1 + 2) % 3);

			assert((cell1.getMateId((index1 + 1) % 3) >= 0) ||
				(cell1.getMateId((index1 + 2) % 3) >= 0));

			v1 = &(mesh->getVertex(idvertex1));
			v2 = &(mesh->getVertex(idvertex2));
			int sing = v2->getNumberOfSings();

			for (int i = 0; i < sing; ++i) {
				idctemp = v2->getSingCell(i);
				do {
					sCell& ctemp = mesh->getCell(idctemp);
					idctemp = ctemp.getMateId((ctemp.getVertexIndex(idvertex2)
						+ 1) % 3);
					ctemp.setVertexId(ctemp.getVertexIndex(idvertex2), idvertex1);
				} while (idctemp >= 0);
			}

			idctemp = cell1.getMateId((index1 + 1) % 3);
			if (idctemp >= 0) {
				sCell& ctemp = mesh->getCell(idctemp);
				ctemp.setMateId(ctemp.getMateIndex(idcell1),
					cell1.getMateId((index1 + 2) % 3));
			}

			idctemp = cell1.getMateId((index1 + 2) % 3);
			if (idctemp >= 0) {
				sCell& ctemp = mesh->getCell(idctemp);
				ctemp.setMateId(ctemp.getMateIndex(idcell1),
					cell1.getMateId((index1 + 1) % 3));
			}

			sVertex& vtemp = mesh->getVertex(cell1.getVertexId(index1));
			if ((sing = vtemp.inSings(idcell1)) >= 0)
				vtemp.setSingCell(sing, cell1.getMateId((index1 + 1) % 3));

			assert(v1->inSings(idcell1) >= 0);
			v1->delSing(v1->inSings(idcell1));

			if ((sing = v2->inSings(idcell1)) >= 0)
				v2->setSingCell(sing, cell1.getMateId((index1 + 2) % 3));
		}

		sing = v2->getNumberOfSings();
		for (int i = 0; i < sing; ++i)
			v1->addSing(v2->getSingCell(i));
		v2->clearSings();
		mesh->freeVertex(idvertex2);
		mesh->freeCell(idcell1);
		return idvertex1;
	}

#undef IDS
}
