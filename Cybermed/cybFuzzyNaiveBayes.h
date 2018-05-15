#pragma once

#include "cybFuzzyProbability.h"

/**
* @class	CybFuzzyNaiveBayes
* @file	CybFuzzyNaiveBayes.h
* @short	.
*
* .
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	3.1.1
* @date	2017, May
*
*/

class CybFuzzyNaiveBayes : public CybFuzzyProbability
{
public:

	/**
	* Constructor.
	*/
	CybFuzzyNaiveBayes(int);

	CybFuzzyNaiveBayes(int, int);

	/**
	* Destructor.
	*/
	~CybFuzzyNaiveBayes();

	CybMatrix < pair< pair<double, double>, double> >* getProbability();

	void setProbability(CybMatrix < pair< pair<double, double>, double> >*);

	double getLogProbability(float, int);

	virtual void training();

	virtual double assessment(CybVectorND<float>&);

protected:

	void parametersEstimation();

private:

	CybMatrix < pair< pair<double, double>, double> > *probability;
};

