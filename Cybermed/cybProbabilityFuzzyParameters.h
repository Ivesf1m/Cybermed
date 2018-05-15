#pragma once

#include "cybAssess.h"
#include "cybMatrix.h"
#include <utility>

using std::pair;

/**
* @class	CybProbabilityFuzzyParameters
* @file	cybProbabilityFuzzyParameters.h
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

class CybProbabilityFuzzyParameters : public CybAssess
{
public:

	/**
	* Constructor.
	*/
	CybProbabilityFuzzyParameters(int, int);

	/**
	* Destructor.
	*/
	~CybProbabilityFuzzyParameters();

	CybMatrix< pair<double, double> >& getFuzzyParameters();

	void setFuzzyParameters(CybMatrix< pair<double, double> >&);

	int getAlphaCuts();

	void setAlphaCuts(int);

	virtual void training() = 0;

	virtual double assessment(CybVectorND<float>&) = 0;

protected:
	CybMatrix<pair<double, double>> parameters;
	int alphaCuts; //min = 12, starts 0.01, ends 1.0

	virtual void parametersEstimation() = 0;
};

