#pragma once

#include "cybFuzzyProbability.h"

/**
* @class	CybFuzzyBinomialNaiveBayes
* @file	CybFuzzyBinomialNaiveBayes.h
* @short	.
*
* .
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	3.1.1
* @date	2017, May
*
*/

class CybFuzzyBinomialNaiveBayes : public CybFuzzyProbability
{
public:

	/**
	* Constructor.
	*/
	CybFuzzyBinomialNaiveBayes(int);

	CybFuzzyBinomialNaiveBayes(int, int);

	/**
	* Destructor.
	*/
	~CybFuzzyBinomialNaiveBayes();

	vector<float>& getProbability();

	void setProbability(vector<float>&);

	vector<int>& getN();

	void setN(vector<int>&);

	virtual void training();

	virtual double assessment(CybVectorND<float>&);

protected:

	void parametersEstimation();

private:
	vector<float> probability;

	/** Parameter N from the Binomial Distribution **/
	vector<int> N;
};

