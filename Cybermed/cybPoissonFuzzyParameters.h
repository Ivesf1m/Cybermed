#pragma once

#include "cybProbabilityFuzzyParameters.h"

/**
* @class	CybPoissonFuzzyParameters
* @file	cybPoissonFuzzyParameters.h
* @short	The base class for all Bucley's Fuzzy Probability assess methods.
*
* This is base class for all assess methods based on Bucley's
* Fuzzy Probability implemented for the CyberMed System.
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	3.1.1
* @date	2016, September
*
*/

class CybPoissonFuzzyParameters : public CybProbabilityFuzzyParameters
{
public:
	/**
	* Constructor.
	*/
	CybPoissonFuzzyParameters(int, int);

	/**
	* Destructor.
	*/
	~CybPoissonFuzzyParameters();

	virtual void training();

	virtual double assessment(CybVectorND<float>&);

protected:
	virtual void parametersEstimation();

private:
	void calculateMean();
	float inverseNormal(float);
};

