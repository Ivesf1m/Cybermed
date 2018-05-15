#pragma once

#include "mfSing.h"
#include "mfVertex.h"

namespace mf
{
#define SPACE typename mfSingularVertex<size, Traits>::space
#define SSING typename mfSingularVertex<size, Traits>::sSing
#define IDS typename mfSingularVertex<size, Traits>::ids

	/**
	*	Base class of the singular vertex
	*	Traits must have ids, space and sSing
	*/
	template<int size, class Traits>
	class mfSingularVertex : public mfVertex<size, Traits>
	{
	public:
		typedef typename Traits::ids ids;
		typedef typename Traits::space space;
		typedef mfSing<Traits> sSing;

		/** Constructor */
		mfSingularVertex();

		/** Destructor */
		virtual ~mfSingularVertex();

		/**
		*	Returns true if this vertex is singular
		*/
		bool isSingular();

		/**
		*	Returns the number of singular components
		*/
		int getNumberOfSings();

		/**
		*	Returns the first cell of each singular component
		*	@param pos: index of singular component
		*	@return: one cell of pos singular component
		*/
		ids getSingCell(int pos = 0);

		/**
		*	Sets a singular component
		*	To add a singular component use addSing
		*	@param pos: index of singular component
		*	@param cell: cell to be set
		*	@param return true if pos exists
		*/
		bool setSingCell(int pos, ids cell);

		/**
		*	Adds a singular component
		*	@param cell: cell to be added
		*	@return: index of the new singular component
		*/
		int addSing(ids cell);

		/**
		*	Deletes a singular component
		*	@param pos: index of singular component
		*	@return: true if successful
		*/
		bool delSing(int pos);

		/**
		*	Deletes all singular components
		*/
		void clearSings();

		/**
		*	Returns the position of the specified cell in the list.
		*/
		int inSings(ids cell);

		/**
		*	Returns the first singular component
		*/
		sSing* getSing();

	private:
		sSing* sings;
	};

	template<int size, class Traits>
	mfSingularVertex<size, Traits>::mfSingularVertex()
		: sings(NULL)
	{

	}

	template<int size, class Traits>
	mfSingularVertex<size, Traits>::~mfSingularVertex()
	{
		clearSings();
	}

	template<int size, class Traits>
	bool mfSingularVertex<size, Traits>::isSingular()
	{
		return (getNumberOfSings() >= 2);
	}

	template<int size, class Traits>
	int mfSingularVertex<size, Traits>::getNumberOfSings()
	{
		mtx.lock();

		sSing* s = sings;
		int i = 0;
		while (s) {
			s = s->getNext();
			++i;
		}
		mtx.unlock();
		return i;
	}

	template<int size, class Traits>
	IDS mfSingularVertex<size, Traits>::getSingCell(int pos)
	{
		assert(pos >= 0);

		mtx.lock();

		sSing* s = sings;
		ids c = -1;
		int i = 0;

		while (s && (i < pos)) {
			s = s->getNext();
			++i;
		}

		if (s)
			c = s->getCell();

		mtx.unlock();

		return c;
	}

	template<int size, class Traits>
	bool mfSingularVertex<size, Traits>::setSingCell(int pos, ids cell)
	{
		assert(pos >= 0);
		assert(cell >= 0);

		mtx.lock();

		sSing* s = sings;
		int i = -1;

		while (s && (i < pos)) {
			i++;
			if (i == pos) {
				s->setCell(cell);
				mtx.unlock();
				return true;
			}
			s = s->getNext();
		}

		mtx.unlock();
		return false;
	}

	template<int size, class Traits>
	int mfSingularVertex<size, Traits>::addSing(ids cell)
	{
		assert(cell >= 0);

		mtx.lock();
		sSing* s = sings;
		sSing* ss = NULL;
		bool sair = false;
		int pos = 0;

		if (s) {
			while (!sair) {
				if (s->getCell() == cell)
					sair = true;
				else {
					if (!s->getNext()) {
						ss = new sSing();
						assert(ss);
						s->setNext(ss);
						sair = true;
					}
					s = s->getNext();
					++pos;
				}
			}
		}
		else {
			ss = new sSing();
			assert(ss);
			sings = ss;
		}

		if (ss)
			ss->setCell(cell);

		mtx.unlock();
		return pos;
	}

	template<int size, class Traits>
	bool mfSingularVertex<size, Traits>::delSing(int pos) {
		assert(pos >= 0);

		mtx.lock();

		sSing* s = sings;
		sSings* ant = NULL;
		int i = -1;
		while (s && (i < pos)) {
			i++;
			if (i == pos) {
				if (ant)
					ant->setNext(s->getNext());
				else
					sings = s->getNext();
				delete s;
				mtx.unlock();
				return true;
			}
			ant = s;
			s = s->getNext();
		}

		mtx.unlock();
		return false;
	}

	template<int size, class Traits>
	void mfSingularVertex<size, Traits>::clearSings()
	{
		mtx.lock();
		sSing* s;
		while (sings->getNext()) {
			s = sings;
			sings = sings->getNext();
			delete s;
		}
		delete s;
		mtx.unlock();
	}

	template<int size, class Traits>
	SSING* mfSingularVertex<size, Traits>::getSing()
	{
		mtx.lock();
		sSing* s = sings;
		mtx.unlock();
		return s;
	}

	template<int size, class Traits>
	int mfSingularVertex<size, Traits>::inSings(ids cell)
	{
		mtx.lock();

		sSing* s = sings;
		int i = 0;

		while (s && (s->getCell() != cell)) {
			s = s->getNext();
			i++;
		}

		mtx.unlock();

		if (s)
			return i;
		else
			return -1;
	}
}