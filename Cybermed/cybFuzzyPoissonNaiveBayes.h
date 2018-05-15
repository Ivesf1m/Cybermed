#pragma once

#include "cybFuzzyProbability.h"

/**
* @class	CybFuzzyPoissonNaiveBayes
* @file	CybFuzzyPoissonNaiveBayes.h
* @short	.
*
* .
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	3.1.1
* @date	2017, May
*
*/

class CybFuzzyPoissonNaiveBayes : public CybFuzzyProbability
{
public:

	/**
	* Constructor.
	*/
	CybFuzzyPoissonNaiveBayes(int);

	CybFuzzyPoissonNaiveBayes(int, int);

	/**
	* Destructor.
	*/
	~CybFuzzyPoissonNaiveBayes();

	vector<float>& getParameters();

	void setParameters(vector<float>&);

	virtual void training();

	virtual double assessment(CybVectorND<float>&);

protected:

	void parametersEstimation();

private:
	vector<float> parameters;
};

