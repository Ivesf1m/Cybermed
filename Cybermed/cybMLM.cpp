#include "cybMLM.h"

CybMLM::CybMLM(int numberOfVariables, float prioriProbability)
	: CybAssess(numberOfVariables), precisionMatrix(numberOfVariables,
		numberOfVariables), prioriProbability(prioriProbability),
	mean(numberOfVariables), covariance(numberOfVariables, numberOfVariables),
	determinant(0.0f)
{

}


CybMLM::~CybMLM()
{

}

float CybMLM::getMean(int node_id)
{
	return this->mean[node_id];
}

float CybMLM::getVariance(int node_id)
{
	return covariance[node_id][node_id];
}

float CybMLM::getPrioriProbability()
{
	return this->prioriProbability;
}

void CybMLM::setMean(float newMean, int node_id)
{
	this->mean[node_id] = newMean;
}

CybMatrix<float>& CybMLM::getCovariance()
{
	return this->covariance;
}

void CybMLM::setCovariance(CybMatrix<float>& newCovariance)
{
	this->covariance = newCovariance;
}

void CybMLM::initData()
{
	//calculate the mean for each variable
	for (int i = 0; i < getNumberOfVariables(); i++)
	{
		float mean = 0;

		for (unsigned int j = 0; j < data.size(); j++)
			mean += data[j][i];

		this->setMean(mean / data.size(), i);
	}

	//calculate the precision matrix
	for (int i = 0; i < getNumberOfVariables(); i++)
		for (int j = 0; j < getNumberOfVariables(); j++)
		{
			for (unsigned int k = 0; k < data.size(); k++)
			{
				this->covariance[i][j] += (data[k][i] - this->getMean(i)) *
					(data[k][j] - this->getMean(j));
			}

			this->covariance[i][j] /= data.size();
		}

	checkVariables();
}

void CybMLM::checkVariables()
{
	for (int i = 0; i < getNumberOfVariables(); i++)
		if (getVariance(i))
			variables.push_back(i);

	determinant = static_cast<float>(covariance.determinant());

	precisionMatrix = covariance.invertMatrix();
}

void CybMLM::training()
{
	initData();
}

double CybMLM::assessment(CybVectorND<float>& data)
{
	return getFunctionResult(data, variables) + log(prioriProbability);
}

double CybMLM::getFunctionResult(CybVectorND<float>& data,
	vector<int>& variables)
{
	vector<float> aux1(variables.size(), 0.0f);
	vector<float> aux2(variables.size(), 0.0f);
	CybMatrix<float> inverse(variables.size(), variables.size());
	double res = 0.0;

	for (unsigned int i = 0; i < variables.size(); i++)
		aux1[i] = (data[variables[i]] - getMean(variables[i]));

	for (unsigned int i = 0; i < variables.size(); i++)
		for (unsigned int j = 0; j < variables.size(); j++)
			aux2[i] += aux1[j] * precisionMatrix[j][i];

	for (unsigned int i = 0; i < variables.size(); i++)
		res += aux1[i] * aux2[i];

	return -log(abs(determinant)) - res;
}
