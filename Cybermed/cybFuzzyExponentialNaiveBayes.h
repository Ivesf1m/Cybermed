#pragma once

#include "cybFuzzyProbability.h"

/**
* @class	CybFuzzyExponentialNaiveBayes
* @file	CybFuzzyExponentialNaiveBayes.h
* @short	.
*
* .
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	3.1.1
* @date	2017, May
*
*/

class CybFuzzyExponentialNaiveBayes : public CybFuzzyProbability
{
public:

	/**
	* Constructor.
	*/
	CybFuzzyExponentialNaiveBayes(int);

	CybFuzzyExponentialNaiveBayes(int, int);

	/**
	* Destructor.
	*/
	~CybFuzzyExponentialNaiveBayes();

	vector<float>& getParameters();

	void setParameters(vector<float>&);

	virtual void training();

	virtual double assessment(CybVectorND<float>&);

protected:

	void parametersEstimation();

private:
	vector<float> parameters;
};

