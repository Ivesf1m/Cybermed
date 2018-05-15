#pragma once
#include "mfMacros.h"
#include <mutex>

using std::mutex;

namespace mf
{

#define IDS typename mfBase<Traits>::ids

	/**
	*	Base class for all elements of meshes.
	*	Traits must have ids typename.
	*
	*/

	template <class Traits> class mfBase
	{
	public:
		typedef typename Traits::ids ids;

		/**
		*	Initialize the mutex.
		*	Set this element as out of mesh.
		*/
		mfBase();

		/**
		*	Destructor
		*	Destroy mutex
		*/
		virtual ~mfBase();

		/**
		*	Check if element is in mesh.
		*/
		bool inMesh();

		/**
		*	Set this element to a mesh.
		*/
		void setInMesh(bool value);

		/**
		*	Set the next free element in vector.
		*	The position is stored in the same place as flags.
		*/
		void setPosition(ids position);

		/**
		*	Get the next free element
		*/
		ids getPosition();

		/**
		*	Lock mutex.
		*/
		void lock();

		/**
		*	Unlock mutex.
		*/
		void unlock();

		/**
		*	Try to lock mutex.
		*/
		bool trylock();

	protected:
		MF_FLAGS_TYPE flags;
		mutex mtx;

	};

	template<class Traits>
	mfBase<Traits>::mfBase()
	{
		mtx.lock();
		flags = (MF_FLAGS_TYPE)0;
		mtx.unlock();
	}

	template<class Traits>
	mfBase<Traits>::~mfBase()
	{

	}

	template<class Traits>
	bool mfBase<Traits>::inMesh()
	{
		mtx.lock();
		bool temp = (flags & (1 << MF_FLAGS_INMESH)) != 0;
		mtx.unlock();
		return temp;
	}

	template<class Traits>
	void mfBase<Traits>::setInMesh(bool value)
	{
		mtx.lock();
		if (value)
			flags |= (1 << MF_FLAGS_INMESH);
		else
			flags &= ~(1 << MF_FLAGS_INMESH);
		mtx.unlock();
	}

	template<class Traits>
	void mfBase<Traits>::setPosition(ids position)
	{
		mtx.lock();
		flags = position;
		mtx.unlock();
	}

	template<class Traits>
	IDS mfBase<Traits>::getPosition()
	{
		mtx.lock();
		ids temp = flags;
		mtx.unlock();
		return temp;
	}

	template<class Traits>
	void mfBase<Traits>::lock()
	{
		mtx.lock();
	}

	template<class Traits>
	void mfBase<Traits>::unlock()
	{
		mtx.unlock();
	}

	template<class Traits>
	bool mfBase<Traits>::trylock()
	{
		return mtx.try_lock();
	}

#undef IDS

}