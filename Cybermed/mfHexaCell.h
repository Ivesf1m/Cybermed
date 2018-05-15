#pragma once
#include "mfCell.h"

namespace mf
{
#define IDS typename mfHexaCell<Traits>::ids

	/**
	*  Hexahedron cell default format with vertex indices:
	*      7----------6
	*     /|         /|
	*    /          / |
	*   /  |       /  |
	*  4----------5   |
	*  |   |      |   |
	*  |   3- -  -|- -2
	*  |  /       |  /
	*  |          | /
	*  |/         |/
	*  0----------1
	*
	*/

	template<class Traits>
	class mfHexaCell : public mfCell<8, Traits>
	{
		public:
			/* Constructor */
			mfHexaCell();

			/* Destructor */
			virtual ~mfHexaCell();

			/**
			*	Returns the edge id of the specified index
			*	@param index: position of edge
			*/
			ids getEdgeId(int index);
			ids getEdgeId(int vIndex1, int vIndex2);

			void setEdgeId(int index, ids edge);

			/**
			*	Returns the mate cell id of the specified index
			*	@param index: position of mate cell
			*/
			ids getMateId(int index);

			/**
			*	Returns the face index to the right of the edge defined by
			*	its 2 vertices.
			*	@param index1: index of the first vertex of the edge
			*	@param index2: index of the second vertex of the edge
			*	@return index of the face
			*/
			int getRightFaceIndex(ids index1, ids index2);

			/**
			*	Returns the face index to the left of the edge defined by
			*	its 2 vertices.
			*	@param index1: index of the first vertex of the edge
			*	@param index2: index of the second vertex of the edge
			*	@return index of the face
			*/
			int getLeftFaceIndex(ids index1, ids index2);

			int getFaceIndex(int v1, int v2, int v3);

			/**
			*	Determines if two vertices are adjacent
			*	@param index1: index of the first vertex
			*	@param index2: index of the second vertex
			*/
			bool verticesAreAdjacent(ids index1, ids index2);

			/**
			*	Defines the mate cell id of the specified index
			*	@param index: position of mate cell
			*	@param cell: the mate cell id
			*/
			void setMateId(int index, ids cell);

			/**
			*	Resets the mate cells ids. Defines -1 for all positions.
			*/
			void clearMates();

			/**
			*	Returns the position of the opposite vertex id of the specified
			*	mate cell id. Returns the position of the specified mate cell id.
			*	@param cell: the mate cell id
			*/
			int getMateIndex(ids cell);

			static inline int getDimension() { return 3; };
			static inline int getNumberVerticesInCell() { return 8; };
			static inline int getNumberEdgesInCell() { return 12; };
			static inline int getNumberFacesInCell() { return 6; };

		private:
			array<ids, 6> mates;
			array<ids, 12> edges;
	};

	template<class Traits>
	mfHexaCell<Traits>::mfHexaCell()
		: mates(-1), edges(-1)
	{

	}

	template<class Traits>
	mfHexaCell<Traits>::~mfHexaCell()
	{

	}

	template<class Traits>
	IDS mfHexaCell<Traits>::getEdgeId(int index)
	{
		assert((index > 0) && (index < 12));
		mtx.lock();
		ids temp = edges[index];
		mtx.unlock();
		return temp;
	}

	template<class Traits>
	IDS mfHexaCell<Traits>::getEdgeId(int vIndex1, int vIndex2)
	{
		ids temp = -1;
		if (vIndex1 == 0) {
			if (vIndex2 == 1)
				temp = edges[0];
			if (vIndex2 == 2)
				temp = edges[1];
			if (vIndex2 == 3)
				temp = edges[2];
		}
		return temp;
	}

	template<class Traits>
	void mfHexaCell<Traits>::setEdgeId(int index, ids edge)
	{
		assert((index >= 0) && (index < 12));
		mtx.lock();
		edges[index] = edge;
		mtx.unlock();
	}

	template<class Traits>
	IDS mfHexaCell<Traits>::getMateId(int index)
	{
		assert((index >= 0) && (index < 6));
		mtx.lock();
		ids temp = mates[index];
		mtx.unlock();
		return temp;
	}

	template<class Traits>
	void mfHexaCell<Traits>::setMateId(int index, ids cell)
	{
		assert((index >= 0) && (index < 6));
		mtx.lock();
		mates[index] = cell;
		mtx.unlock();
	}

	template<class Traits>
	int mfHexaCell<Traits>::getMateIndex(ids cell)
	{
		for (int i = 0; i < 6; ++i)
			if (getMateId(i) == cell)
				return i;
		return -1;
	}

	template<class Traits>
	void mfHexaCell<Traits>::clearMates()
	{
		for (int i = 0; i < 6; ++i)
			setMateId(i, -1);
	}

	template<class Traits>
	int mfHexaCell<Traits>::getRightFaceIndex(ids index1, ids index2)
	{
		assert((index1 >= 0) && (index1 <= 7));
		assert((index2 >= 0) && (index2 <= 7));
		assert(index1 != index2);

		switch (index1) {
			case 0:
				if (index2 == 1)
					return 0;
				if (index2 == 3)
					return 4;
				if (index2 == 4)
					return 1;
				break;
			case 1:
				if (index2 == 0)
					return 1;
				if (index2 == 2)
					return 0;
				if (index2 == 5)
					return 2;
				break;
			case 2:
				if (index2 == 1)
					return 2;
				if (index2 == 3)
					return 0;
				if (index2 == 6)
					return 3;
				break;
			case 3:
				if (index2 == 0)
					return 0;
				if (index2 == 2)
					return 3;
				if (index2 == 7)
					return 4;
				break;
			case 4:
				if (index2 == 0)
					return 4;
				if (index2 == 5)
					return 1;
				if (index2 == 7)
					return 5;
				break;
			case 5:
				if (index2 == 1)
					return 1;
				if (index2 == 4)
					return 5;
				if (index2 == 6)
					return 2;
				break;
			case 6:
				if (index2 == 2)
					return 2;
				if (index2 == 5)
					return 5;
				if (index2 == 7)
					return 3;
				break;
			case 7:
				if (index2 == 3)
					return 3;
				if (index2 == 4)
					return 4;
				if (index2 == 6)
					return 5;
				break;
		}
		return -1;
	}

	template<class Traits>
	int mfHexaCell<Traits>::getLeftFaceIndex(ids index1, ids index2)
	{
		assert((index1 >= 0) && (index1 <= 7));
		assert((index2 >= 0) && (index2 <= 7));
		assert(index1 != index2);

		switch (index1) {
			case 0:
				if (index2 == 1)
					return 1;
				if (index2 == 3)
					return 0;
				if (index2 == 4)
					return 4;
				break;
			case 1:
				if (index2 == 0)
					return 0;
				if (index2 == 2)
					return 2;
				if (index2 == 5)
					return 1;
				break;
			case 2:
				if (index2 == 1)
					return 0;
				if (index2 == 3)
					return 3;
				if (index2 == 6)
					return 2;
				break;
			case 3:
				if (index2 == 0)
					return 4;
				if (index2 == 2)
					return 0;
				if (index2 == 7)
					return 3;
				break;
			case 4:
				if (index2 == 0)
					return 1;
				if (index2 == 5)
					return 5;
				if (index2 == 7)
					return 4;
				break;
			case 5:
				if (index2 == 1)
					return 2;
				if (index2 == 4)
					return 1;
				if (index2 == 6)
					return 5;
				break;
			case 6:
				if (index2 == 2)
					return 3;
				if (index2 == 5)
					return 2;
				if (index2 == 7)
					return 5;
				break;
			case 7:
				if (index2 == 3)
					return 4;
				if (index2 == 4)
					return 5;
				if (index2 == 6)
					return 3;
				break;
		}
		return -1;
	}

	template<class Traits>
	int mfHexaCell<Traits>::getFaceIndex(int v1, int v2, int v3)
	{
		assert((v1 >= 0) && (v1 <= 7));
		assert((v2 >= 0) && (v2 <= 7));
		assert((v3 >= 0) && (v3 <= 7));
		assert((v1 != v2) && (v1 != v3) && (v2 != v3));

		switch (v1) {
			case 0:
				if (v2 == 1 && v3 == 5)
					return 1;
				if (v2 == 4 && v3 == 7)
					return 4;
				if (v2 == 3 && v3 == 2)
					return 0;
				break;
			case 1:
				if (v2 == 0 && v3 == 3)
					return 0;
				if (v2 == 5 && v3 == 4)
					return 1;
				if (v2 == 2 && v3 == 6)
					return 2;
				break;
			case 2:
				if (v2 == 1 && v3 == 0)
					return 0;
				if (v2 == 6 && v3 == 5)
					return 2;
				if (v2 == 3 && v3 == 7)
					return 3;
				break;
			case 3:
				if (v2 == 2 && v3 == 1)
					return 0;
				if (v2 == 7 && v3 == 6)
					return 3;
				if (v2 == 0 && v3 == 4)
					return 4;
				break;
			case 4:
				if (v2 == 0 && v3 == 1)
					return 1;
				if (v2 == 7 && v3 == 3)
					return 4;
				if (v2 == 5 && v3 == 6)
					return 5;
				break;
			case 5:
				if (v2 == 4 && v3 == 0)
					return 1;
				if (v2 == 1 && v3 == 2)
					return 2;
				if (v2 == 6 && v3 == 7)
					return 5;
				break;
			case 6:
				if (v2 == 5 && v3 == 1)
					return 2;
				if (v2 == 2 && v3 == 3)
					return 3;
				if (v2 == 7 && v3 == 4)
					return 5;
				break;
			case 7:
				if (v2 == 6 && v3 == 2)
					return 3;
				if (v2 == 3 && v3 == 0)
					return 4;
				if (v2 == 4 && v3 == 5)
					return 5;
				break;
		}

		//unordered search
		switch (v1) {
			case 0:
				if (v2 == 1) {
					if (v3 == 2 || v3 == 3)
						return 0;
					if (v3 == 4)
						return 1;
				}
				if (v2 == 2) {
					if (v3 == 1 || v3 == 3)
						return 0;
				}

				if (v2 == 3) {
					if (v3 == 1)
						return 0;
					if (v3 == 4 || v3 == 7)
						return 4;
				}

				if (v2 == 4) {
					if (v3 == 1 || v3 == 5)
						return 1;
					if (v3 == 3)
						return 4;
				}
				if (v2 == 5) {
					if (v3 == 1 || v3 == 4)
						return 1;
				}
				if (v2 == 7) {
					if (v3 == 4 || v3 == 3)
						return 4;
				}
				break;
			case 1:
				if (v2 == 0) {
					if (v3 == 2)
						return 0;
					if (v3 == 5 || v3 == 4)
						return 1;
				}
				if (v2 == 2) {
					if (v3 == 0 || v3 == 3)
						return 0;
					if (v3 == 5)
						return 2;
				}

				if (v2 == 3) {
					if (v3 == 0 || v3 == 2)
						return 0;
				}

				if (v2 == 4) {
					if (v3 == 0 || v3 == 5)
						return 1;
				}
				if (v2 == 5) {
					if (v3 == 0)
						return 1;
					if (v3 == 2 || v3 == 6)
						return 2;
				}
				if (v2 == 6) {
					if (v3 == 2 || v3 == 5)
						return 2;
				}
				break;
			case 2:
				if (v2 == 0) {
					if (v3 == 1 || v3 == 3)
						return 0;
				}
				if (v2 == 1) {
					if (v3 == 3)
						return 0;
					if (v3 == 5 || v3 == 6)
						return 2;
				}

				if (v2 == 6) {
					if (v3 == 3)
						return 0;
					if (v3 == 0 || v3 == 1)
						return 0;
				}

				if (v2 == 5) {
					if (v3 == 1 || v3 == 6)
						return 2;
				}
				if (v2 == 6) {
					if (v3 == 1)
						return 2;
					if (v3 == 3 || v3 == 7)
						return 3;
				}
				if (v2 == 7) {
					if (v3 == 3 || v3 == 6)
						return 3;
				}
				break;
			case 3:
				if (v2 == 0) {
					if (v3 == 2 || v3 == 1)
						return 0;
					if (v3 == 7)
						return 4;
				}
				if (v2 == 1) {
					if (v3 == 0 || v3 == 2)
						return 0;
				}

				if (v2 == 2) {
					if (v3 == 0)
						return 0;
					if (v3 == 7 || v3 == 6)
						return 3;
				}

				if (v2 == 4) {
					if (v3 == 0 || v3 == 7)
						return 4;
				}
				if (v2 == 6) {
					if (v3 == 2 || v3 == 7)
						return 3;
				}
				if (v2 == 7) {
					if (v3 == 2)
						return 3;
					if (v3 == 0 || v3 == 4)
						return 4;
				}
				break;
			case 4:
				if (v2 == 0) {
					if (v3 == 3 || v3 == 7)
						return 4;
					if (v3 == 5)
						return 1;
				}
				if (v2 == 1) {
					if (v3 == 0 || v3 == 5)
						return 1;
				}

				if (v2 == 3) {
					if (v3 == 0 || v3 == 7)
						return 4;
				}

				if (v2 == 5) {
					if (v3 == 0 || v2 == 1)
						return 1;
					if (v3 == 7)
						return 5;
				}
				if (v2 == 6) {
					if (v3 == 5 || v3 == 7)
						return 5;
				}
				if (v2 == 7) {
					if (v3 == 0)
						return 4;
					if (v3 == 5 || v3 == 6)
						return 5;
				}
				break;
			case 5:
				if (v2 == 0) {
					if (v3 == 1 || v3 == 4)
						return 1;
				}
				if (v2 == 1) {
					if (v3 == 0 || v3 == 4)
						return 1;
					if (v3 == 6)
						return 2;
				}

				if (v2 == 2) {
					if (v3 == 1 || v3 == 6)
						return 2;
				}

				if (v2 == 4) {
					if (v3 == 6 || v2 == 7)
						return 5;
					if (v3 == 1)
						return 1;
				}
				if (v2 == 6) {
					if (v3 == 4)
						return 5;
					if (v3 == 2 || v3 == 1)
						return 2;
				}
				if (v2 == 7) {
					if (v3 == 4 || v3 == 6)
						return 5;
				}
				break;
			case 6:
				if (v2 == 1) {
					if (v3 == 2 || v3 == 5)
						return 2;
				}
				if (v2 == 2) {
					if (v3 == 1 || v3 == 5)
						return 2;
					if (v3 == 7)
						return 3;
				}

				if (v2 == 3) {
					if (v3 == 2 || v3 == 7)
						return 3;
				}

				if (v2 == 4) {
					if (v3 == 5 || v2 == 7)
						return 5;
				}
				if (v2 == 5) {
					if (v3 == 2)
						return 2;
					if (v3 == 4 || v3 == 7)
						return 5;
				}
				if (v2 == 7) {
					if (v3 == 5)
						return 5;
					if (v3 == 2 || v3 == 3)
						return 3;
				}
				break;
			case 7:
				if (v2 == 0) {
					if (v3 == 4 || v3 == 3)
						return 4;

				}
				if (v2 == 2) {
					if (v3 == 3 || v3 == 6)
						return 3;
				}

				if (v2 == 3) {
					if (v3 == 4)
						return 4;
					if (v3 == 2 || v3 == 6)
						return 3;
				}

				if (v2 == 4) {
					if (v3 == 0 || v2 == 3)
						return 4;
					if (v3 == 6)
						return 5;
				}
				if (v2 == 5) {
					if (v3 == 4 || v3 == 6)
						return 5;
				}
				if (v2 == 6) {
					if (v3 == 3)
						return 3;
					if (v3 == 4 || v3 == 5)
						return 5;
				}
				break;
		}
		return -1;
	}

	template<class Traits>
	bool mfHexaCell<Traits>::verticesAreAdjacent(ids index1, ids index2)
	{
		assert((index1 >= 0) && (index1 <= 7));
		assert((index2 >= 0) && (index2 <= 7));
		assert(index1 != index2);

		switch(index1) {
			case 0:
				if (index2 == 1)
					return true;
				if (index2 == 3)
					return true;
				if (index2 == 4)
					return true;
				break;
			case 1:
				if (index2 == 0)
					return true;
				if (index2 == 2)
					return true;
				if (index2 == 5)
					return true;
				break;
			case 2:
				if (index2 == 1)
					return true;
				if (index2 == 3)
					return true;
				if (index2 == 6)
					return true;
				break;
			case 3:
				if (index2 == 0)
					return true;
				if (index2 == 2)
					return true;
				if (index2 == 7)
					return true;
				break;
			case 4:
				if (index2 == 0)
					return true;
				if (index2 == 5)
					return true;
				if (index2 == 7)
					return true;
				break;
			case 5:
				if (index2 == 1)
					return true;
				if (index2 == 4)
					return true;
				if (index2 == 6)
					return true;
				break;
			case 6:
				if (index2 == 2)
					return true;
				if (index2 == 5)
					return true;
				if (index2 == 7)
					return true;
				break;
			case 7:
				if (index2 == 3)
					return true;
				if (index2 == 4)
					return true;
				if (index2 == 6)
					return true;
				break;
		}
		return false;
	}

#undef IDS
}