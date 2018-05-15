#pragma once

#include <array>
#include "mfBase.h"

using std::array;

namespace mf
{
#define IDS typename mfCell<size, Traits>::ids

	/**
	*	Base class for cells.
	*	Cell types are:
	*	2D - triangles, quadrilaterals and hybrid surfaces
	*	3D - tetrahedra, hexahedra, prism, pyramids and hybrid volumetric.
	*/
	template <int size, class Traits>
	class mfCell : public mfBase<Traits>
	{
		public:
			typedef typename Traits::ids ids;

			/**
			*	Constructor
			*	Initialize vertices and maters ids
			*/
			mfCell();

			/**
			*	Destructor
			*/
			virtual ~mfCell();

			/**
			*	Return the mate cell id of the specified index
			*/
			ids getMateId(int index);

			/**
			*	Return the vertex id of the specified index
			*/
			ids getVertexId(int index);

			/**
			*	Set the mate cell id of the specified index
			*/
			void setMateId(int index, ids cell);

			/**
			*	Set the vertex id of the specified index
			*/
			void setVertexId(int index, ids vertex);

			/**
			*	Reset mate cells ids
			*/
			void clearMates();

			/**
			*	Returns the position/index (in the cell) of the specified vertex id.
			*/
			int getVertexIndex(ids vertex);

			/**
			*	Returns the position/index (in the cell) of the specified mate cell id.
			*/
			int getMateIndex(ids cell);

			/**
			*	Returns the opposite cell if of the specified vertex id.
			*	Used only in surface meshes.
			*/
			ids getMateVertexId(ids vertex);

			/**
			*	Returns the opposite vertex id of the specified mate cell id.
			*/
			ids getVertexMateId(ids cell);

			/**
			*	Returns the dimension of this cell.
			*/
			static inline int getDimension() { return size - 1; };

			/**
			*	Returns the number of vertices of this cell.
			*/
			static inline int getNumberVerticesInCell() { return size; };

			/**
			*	Returns the number of edges of this cell.
			*/
			static inline int getNumberEdgesInCell() { return size; };

		protected:
			array<ids, size> vertices;
			array<ids, size> mates;			
	};

	template<int size, class Traits>
	mfCell<size, Traits>::mfCell()
	{
		mtx.lock();

		for (int i = 0; i < size; ++i)
			vertices[i] = -1;

		for (int i = 0; i < 3; ++i)
			mates[i] = -1;

		mtx.unlock();
	}

	template<int size, class Traits>
	mfCell<size, Traits>::~mfCell()
	{

	}

	template<int size, class Traits>
	IDS mfCell<size, Traits>::getMateId(int index)
	{
		assert((index >= 0) && (index < size));
		mtx.lock();
		ids temp = mates[index];
		mtx.unlock();
		return temp;
	}

	template<int size, class Traits>
	IDS mfCell<size, Traits>::getVertexId(int index)
	{
		assert((index >= 0) && (index < size));
		mtx.lock();
		ids temp = vertices[index];
		mtx.unlock();
		return temp;
	}

	template<int size, class Traits>
	void mfCell<size, Traits>::setMateId(int index, ids cell)
	{
		assert((index >= 0) && (index < size));
		mtx.lock();
		mates[index] = cell;
		mtx.unlock();
	}

	template<int size, class Traits>
	void mfCell<size, Traits>::setVertexId(int index, ids vertex)
	{
		assert((index >= 0) && (index < size));
		mtx.lock();
		vertices[index] = vertex;
		mtx.unlock();
	}

	template<int size, class Traits>
	IDS mfCell<size, Traits>::getMateVertexId(ids vertex)
	{
		for (int i = 0; i < size; ++i)
			if (getVertexId(i) == vertex)
				return getMateId(i);
		return -1;
	}

	template<int size, class Traits>
	IDS mfCell<size, Traits>::getVertexMateId(ids cell)
	{
		for (int i = 0; i < size; ++i)
			if (getMateId(i) == cell)
				return getVertexId(i);
		return -1;
	}

	template<int size, class Traits>
	int mfCell<size, Traits>::getMateIndex(ids cell)
	{
		for (int i = 0; i < size; ++i)
			if (getMateId(i) == cell)
				return i;
		return -1;
	}

	template<int size, class Traits>
	void mfCell<size, Traits>::clearMates()
	{
		for (int i = 0; i < size; ++i)
			setMateId(i, -1);
	}

	template<int size, class Traits>
	int mfCell<size, Traits>::getVertexIndex(ids vertex)
	{
		for (int i = 0; i < size; ++i)
			if (getVertexId(i) == vertex)
				return i;
		return -1;
	}

#undef IDS
}