#pragma once

#include "cybAssess.h"
#include "cybMatrix.h"

/**
* @class	CybGammaNaiveBayes
* @file	CybGammaNaiveBayes.h
* @short	.
*
* .
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	3.1.1
* @date	2017, May
*
*/

class CybGammaNaiveBayes : public CybAssess
{
public:

	/**
	* Constructor.
	*/
	CybGammaNaiveBayes(int);

	/**
	* Destructor.
	*/
	~CybGammaNaiveBayes();

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

