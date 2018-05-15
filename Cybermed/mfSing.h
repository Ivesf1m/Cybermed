#pragma once

#include "mfMacros.h"

namespace mf
{
#define SSING typename mfSing<Traits>::sSing
#define IDS typename mfSing<Traits>::ids

	/**
	*	Base class of singular component in vertex
	*	Traits must have: ids, sSing
	*/

	template<class Traits>
	class mfSing
	{
	public:
		typedef typename Traits::ids ids;
		typedef mfSing<Traits> sSing;

		/** Constructor */
		mfSing();

		/** Destructor */
		~mfSing();

		/**	
		*	Returns the next singular component
		*	@return Reference to the next singular component
		*/
		sSing* getNext();

		/**	
		*	Defines the next singular component
		*	@param next: the next component on the singular list
		*/
		void setNext(sSing* next);

		/**	
		*	Returns the cell of this singular component
		*	@return ID of the cell of the singular component
		*/
		ids getCell();

		/**	
		*	Defines the cell of this singular component
		*	@param cell: ID of the cell of the singular component
		*/
		void setCell(ids cell);

	private:
		ids cell;
		sSing* next;
	};

	template<class Traits>
	mfSing<Traits>::mfSing()
		: next(NULL)
	{

	}

	template<class Traits>
	mfSing<Traits>::~mfSing()
	{

	}

	template<class Traits>
	SSING* mfSing<Traits>::getNext()
	{
		return next;
	}

	template<class Traits>
	void mfSing<Traits>::setNext(sSing* next)
	{
		this->next = next;
	}

	template<class Traits>
	IDS mfSing<Traits>::getCell()
	{
		return cell;
	}

	template<class Traits>
	void mfSing<Traits>::setCell(ids cell)
	{
		assert(cell >= 0);
		this->cell = cell;
	}

#undef SSING
#undef IDS
}