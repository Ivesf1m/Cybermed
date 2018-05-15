#pragma once

#include "mfBase.h"

namespace mf
{
	template<class Traits>
	class mfSearchDIDO
	{
		typedef typename Traits::sVertex sVertex;
		typedef typename Traits::sCell sCell;
		typedef typename Traits::ids ids;
		typedef typename Traits::space space;
		typedef typename Traits::sGeometric sGeometric;
		typedef typename Traits::sMesh sMesh;

		/** Constructor */
		mfSearchDIDO(sMesh* _mesh);

		/** Destructor */
		~mfSearchDIDO();

		int dido(sVertex* v, space* coords, ids& idcell, int& side);

		/**
		*	Sets the mesh that this class will manipulate.
		*	@param mesh: pointer to the mesh
		*/
		void setMesh(sMesh* mesh);

	private:
		sMesh* mesh;
		sGeometric geom;
	};

	template<class Traits>
	mfSearchDIDO<Traits>::mfSearchDIDO(sMesh* _mesh)
		: mesh(_mesh)
	{
		geom.setMesh(mesh);
	}

	template<class Traits>
	mfSearchDIDO<Traits>::~mfSearchDIDO()
	{

	}

	template<class Traits>
	void mfSearchDIDO<Traits>::setMesh(sMesh* mesh)
	{
		assert(mesh);
		this->mesh = mesh;
	}

	template<class Traits>
	int mfSearchDIDO<Traits>::dido(sVertex* v, space* coords, ids& idcell,
		int& side)
	{
		//returns 0 if it is inside
		//returns 1 if it is outside
		//returns 2 if it is on the edge on the side
		//returns 3 if it is a repeated vertex

		assert(mesh);

		sCell* auxc = NULL;
		sVertex* v[3];
		ids icell, iauxc = 0;
		int on, left, ret = -1;

		assert(v->getNumberOfSings() == 1);

		iauxc = v->getSingCell(0);
		auxc = mesh->getCell(iauxc);
		side = 0;

		while (ret < 0) {
			v[0] = mesh->getVertex(auxc->getVertexId(side));
			v[1] = mesh->getVertex(auxc->getVertexId((side + 1) % 3));
			v[2] = mesh->getVertex(auxc->getVertexId((side + 2) % 3));
			on = -1;

			left = geom.inLeft(v[0], v[1], coords[0], coords[1]);
			if (left == 0) {
				if (auxc->getMateId((side + 2) % 3) >= 0) {
					icell = iauxc;
					iauxc = auxc->getMateId((lado + 2) % 3);
					auxc = mesh->getCell(iauxc);
					side = (auxc->getMateIndex(icell) + 2) % 3;
				}
				else {
					side = (side + 2) % 3;
					idcell = iauxc;
					ret = 1;
				}
			}
			else {
				if (left == 2)
					on = (lado + 2) % 3;
				left = geom.inLeft(v[1], v[2], coords[0], coords[1]);
				
				if (left == 0) {
					if (auxc->getMateId(side) >= 0) {
						icell = iauxc;
						iauxc = auxc->getMateId(side);
						auxc = mesh->getCell(iauxc);
						side = (auxc->getMateIndex(icell) + 2) % 3;
					}
					else {
						idcell = iauxc;
						ret = 1;
					}
				}
				else {
					if (left == 2) {
						if (on >= 0) {
							side = (side + 1) % 3;
							idcell = iauxc;
							ret = 3;
						}
						else
							on = side;
					}

					if (ret < 0) {
						left = geom.inLeft(v[2], v[0], coords[0], coords[1]);

						if (left == 0) {
							if (auxc->getMateId((lado + 1) % 3) >= 0) {
								icell = iauxc;
								iauxc = auxc->getMateId((lado + 1) % 3);
								auxc = mesh->getCell(iauxc);
								side = (auxc->getMateIndex(icell) + 2) % 3;
							}
							else {
								side = (lado + 1) % 3;
								idcell = iauxc;
								ret = 1;
							}
						}
						else {
							if (left == 2) {
								if (on >= 0) {
									idcell = iauxc;
									if (on == side)
										side = (side + 2) % 3;
									ret = 3;
								}
								else {
									idcell = iauxc;
									side = (side + 1) % 3;
									ret = 2;
								}
							}
							else {
								if (on >= 0) {
									idcell = iauxc;
									side = on;
									ret = 2;
								}
								else {
									idcell = iauxc;
									side = -1;
									ret = 0;
								}
							}
						}
					}
				}
			}
		}

		return ret;
	}

}
