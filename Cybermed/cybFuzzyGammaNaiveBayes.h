#pragma once

#include "cybFuzzyProbability.h"

/**
* @class	CybFuzzyGammaNaiveBayes
* @file	CybFuzzyGammaNaiveBayes.h
* @short	.
*
* .
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	3.1.1
* @date	2017, May
*
*/

class CybFuzzyGammaNaiveBayes : public CybFuzzyProbability
{
public:

	/**
	* Constructor.
	*/
	CybFuzzyGammaNaiveBayes(int);

	CybFuzzyGammaNaiveBayes(int, int);

	/**
	* Destructor.
	*/
	~CybFuzzyGammaNaiveBayes();

	void setAlpha(vector<float>&);

	vector<float>& getAlpha();

	void setBeta(vector<float>&);

	vector<float>& getBeta();

	virtual void training();

	virtual double assessment(CybVectorND<float>&);

protected:

	void parametersEstimation();

private:
	vector<float> alpha;
	vector<float> beta;
};

