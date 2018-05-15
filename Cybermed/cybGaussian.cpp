#include "cybGaussian.h"



CybGaussian::CybGaussian(int variablesNumber, float prioriProbability)
	: CybAssess(variablesNumber), prioriProbability(prioriProbability),
	mean(variablesNumber), covariance(variablesNumber, variablesNumber),
	determinant(0.0f), precisionMatrix(variablesNumber, variablesNumber)
{

}


CybGaussian::~CybGaussian()
{

}

float CybGaussian::getMean(int node_id)
{
	return this->mean[node_id];
}

vector<float>& CybGaussian::getMean()
{
	return mean;
}

void CybGaussian::setMean(float newMean, int node_id)
{
	this->mean[node_id] = newMean;
}

void CybGaussian::setMean(vector<float>& newMean)
{
	mean = newMean;
}

float CybGaussian::getVariance(int node_id)
{
	return this->covariance[node_id][node_id];
}

vector<float> CybGaussian::getVariance()
{
	vector<float> var;
	for (int i = 0; i < getNumberOfVariables(); ++i)
		var.push_back(covariance[i][i]);
	return var;
}

vector<float> CybGaussian::getCovariance(int node_id)
{
	vector<float> cov;
	for (int i = 0; i < getNumberOfVariables(); ++i)
		cov.push_back(covariance[node_id][i]);
	return cov;
}

CybMatrix<float>& CybGaussian::getCovariance()
{
	return covariance;
}

void CybGaussian::setCovariance(vector<float>& newCovariance, int node_id)
{
	for (int i = 0; i < getNumberOfVariables(); ++i)
		covariance[node_id][i] = covariance[i][node_id] = newCovariance[i];
}

void CybGaussian::setCovariance(CybMatrix<float>& newCovariance)
{
	covariance = newCovariance;
}

float CybGaussian::getPrioriProbability()
{
	return this->prioriProbability;
}

void CybGaussian::training()
{
	calculateMean();
	calculateCovariance();
}

double CybGaussian::assessment(CybVectorND<float>& data)
{
	return getFunctionResult(data, this->variables) + log(prioriProbability);
}

void CybGaussian::calculateMean()
{
	for (int i = 0; i < getNumberOfVariables(); i++)
	{
		float mean = 0;

		for (unsigned int j = 0; j < data.size(); j++)
			mean += data[j][i];

		this->setMean(mean /data.size(), i);
	}
}

void CybGaussian::calculateCovariance()
{
	for (int i = 0; i < getNumberOfVariables(); i++)
		for (int j = 0; j < getNumberOfVariables(); j++)
		{
			for (unsigned int k = 0; k < data.size(); k++)
			{
				covariance[i][j] += (data[k][i] - getMean(i)) *
					(data[k][j] - getMean(j));
			}

			covariance[i][j] /= data.size();
		}

	checkVariables();
}

double CybGaussian::getFunctionResult(CybVectorND<float>& data,
	vector<int>& variables)
{
	vector<float> aux1(variables.size(), 0.0f);
	vector<float> aux2(variables.size(), 0.0f);
	CybMatrix<float> inverse = precisionMatrix.invertMatrix();
	double res = 0.0;

	for (unsigned int i = 0; i < variables.size(); ++i)
		aux1[i] = (data[variables[i]] - getMean(variables[i]));

	for (unsigned int i = 0; i < variables.size(); ++i)
		for (unsigned int j = 0; j < variables.size(); ++j)
			aux2[i] += aux1[j] * precisionMatrix[j][i];

	for (unsigned int i = 0; i < variables.size(); ++i)
		res += aux1[i] * aux2[i];

	return -log(abs(determinant)) - res;
}

void CybGaussian::checkVariables()
{
	for (int i = 0; i < getNumberOfVariables(); ++i)
		if (getVariance(i))
			variables.push_back(i);

	determinant = static_cast<float>(covariance.determinant());
	precisionMatrix = covariance.invertMatrix();
}