#pragma once

#include "cybFuzzyProbability.h"

/**
* @class	CybFuzzyGaussianNaiveBayes
* @file	CybFuzzyGaussianNaiveBayes.h
* @short	.
*
* .
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	3.1.1
* @date	2017, May
*
*/

class CybFuzzyGaussianNaiveBayes : public CybFuzzyProbability
{
public:

	/**
	* Constructor.
	*/
	CybFuzzyGaussianNaiveBayes(int);

	CybFuzzyGaussianNaiveBayes(int, int);

	/**
	* Destructor.
	*/
	~CybFuzzyGaussianNaiveBayes();

	vector<float>& getMean();

	void setMean(vector<float>&);

	vector<float>& getStdDev();

	void setStdDev(vector<float>&);

	virtual void training();

	virtual double assessment(CybVectorND<float>&);

protected:

	void parametersEstimation();

private:
	vector<float> mean;
	vector<float> stdDev;
};

