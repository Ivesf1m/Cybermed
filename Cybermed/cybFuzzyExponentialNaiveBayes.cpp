#include "cybFuzzyExponentialNaiveBayes.h"

CybFuzzyExponentialNaiveBayes::CybFuzzyExponentialNaiveBayes(int variables)
	: CybFuzzyProbability(variables), parameters(variables)
{

}

CybFuzzyExponentialNaiveBayes::CybFuzzyExponentialNaiveBayes(int variables,
	int nIntervals)
	: CybFuzzyProbability(variables, nIntervals), parameters(variables)
{

}

CybFuzzyExponentialNaiveBayes::~CybFuzzyExponentialNaiveBayes()
{

}

vector<float>& CybFuzzyExponentialNaiveBayes::getParameters()
{
	return parameters;
}

void CybFuzzyExponentialNaiveBayes::setParameters(vector<float>& parameters)
{
	this->parameters = parameters;
}

void CybFuzzyExponentialNaiveBayes::training()
{
	//1st - calculate pertinences
	calcPertinences();

	//2nd - estimate parameters
	parametersEstimation();
}

double CybFuzzyExponentialNaiveBayes::assessment(CybVectorND<float>& auxdata)
{
	double density = 0.0;
	for (int i = 0; i < getNumberOfVariables(); i++)
		density += log(getParameters()[i]) - getParameters()[i] * auxdata[i] +
		getLogPertinence(auxdata[i], i);


	return density;
}

void CybFuzzyExponentialNaiveBayes::parametersEstimation()
{
	int size = data[0].getDimension();

	//1st - calculate mean
	vector<double> mean(getNumberOfVariables(), 0);
	for (int i = 0; i < getNumberOfVariables(); i++)
	{
		for (int j = 0; j < size; j++)
		{
			mean[i] += data[i][j];
		}
		mean[i] /= size;
	}

	//2nd - calculate lambda
	for (int i = 0; i < getNumberOfVariables(); i++)
	{
		parameters[i] = 1 / mean[i];
	}
}