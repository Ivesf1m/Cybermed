#pragma once

#include "cybAssess.h"
#include "cybMatrix.h"

class CybMLM : public CybAssess
{
public:
	CybMLM(int numberOfVariables, float prioriProbability);

	~CybMLM();

	float getMean(int node_id);

	float getVariance(int node_id);

	void setMean(float newMean, int node_id);

	CybMatrix<float>& getCovariance();

	float getPrioriProbability();

	void setCovariance(CybMatrix<float>&);

	virtual void training();

	virtual double assessment(CybVectorND<float>& data);

	void checkVariables();

protected:

	virtual void initData();

	virtual double getFunctionResult(CybVectorND<float>& data,
		vector<int>& variables);

private:
	CybMatrix<float> covariance;
	CybMatrix<float> precisionMatrix;
	vector<float> mean;
	vector<int> variables;
	float prioriProbability;
	float determinant;
};

