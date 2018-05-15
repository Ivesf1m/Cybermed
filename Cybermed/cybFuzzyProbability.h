#pragma once

#include "cybAssess.h"
#include "cybMatrix.h"
#include <utility>

using std::pair;

/**
* @class	CybFuzzyProbability
* @file	cybFuzzyProbability.h
* @short	The base class for all Zadeh's Fuzzy Probability assess methods.
*
* This is base class for all assess methods based on Zadeh's
* Fuzzy Probability implemented for the CyberMed System.
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	3.1.1
* @date	2017, May
*
*/

class CybFuzzyProbability : public CybAssess
{
public:

	/**
	* Constructor.
	*/
	CybFuzzyProbability(int);

	CybFuzzyProbability(int, int);

	/**
	* Destructor.
	*/
	~CybFuzzyProbability();

	CybMatrix< pair< pair<double, double>, double> >* getPertinences();

	void setPertinences(CybMatrix< pair< pair<double, double>, double> >*);

	double getPertinence(float, int);

	double getLogPertinence(float, int);

	int getNIntervals();

	void setNIntervals(int);

	virtual void training() = 0;

	virtual double assessment(CybVectorND<float>&) = 0;

protected:
	CybMatrix< pair< pair<double, double>, double> >* pertinences;
	int nIntervals;

	void calcPertinences();
};

