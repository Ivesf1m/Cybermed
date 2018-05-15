#pragma once

#include <array>
#include "mfBase.h"

using std::array;

namespace mf
{
#define SPACE typename mfVertex<size, Traits>::space

	/**
	*	Base class for vertices
	*/
	template<int size, class Traits>
	class mfVertex : public mfBase<Traits>
	{
	public:
		typedef typename Traits::space space;

		/** Constructor */
		mfVertex();

		/** Destructor */
		virtual ~mfVertex();

		/**
		*	Returns a specific coordinate.
		*	@param pos: index of the coordinate
		*	@return value of the coordinate
		*/
		space getCoord(int pos);

		/**
		*	Returns a reference to the array with all the components.
		*/
		array<space, size>& getCoords();

		/**
		*	Defines the specified coordinate value
		*	@param pos: index of the coordinate
		*	@param value: the new value
		*/
		void setCoord(int pos, space value);

		/**
		*	Defines all coordinate values
		*	@param values: array with the new values
		*/
		void setCoords(array<space, size>& values);

		/**
		*	Returns the dimension of the vertex
		*/
		static inline int getDimension() { return size; };

	protected:
		array<space, size> coords;
	};

	template<int size, class Traits>
	mfVertex<size, Traits>::mfVertex()
	{

	}

	template<int size, class Traits>
	mfVertex<size, Traits>::~mfVertex()
	{

	}

	template<int size, class Traits>
	SPACE mfVertex<size, Traits>::getCoord(int pos)
	{
		space c;

		assert((pos >= 0) && (pos < size));

		mtx.lock();
		c = coords[pos];
		mtx.unlock();

		return c;
	}

	template<int size, class Traits>
	array<SPACE, size>& mfVertex<size, Traits>::getCoords()
	{
		return coords;
	}

	template<int size, class Traits>
	void mfVertex<size, Traits>::setCoord(int pos, space value)
	{
		assert((pos >= 0) && (pos < size));

		mtx.lock();
		coords[pos] = value;
		mtx.unlock();
	}

	template<int size, class Traits>
	void mfVertex<size, Traits>::setCoords(array<space, size>& values)
	{
		mtx.lock();
		coords = values;
		mtx.unlock();
	}

#undef SPACE
}
