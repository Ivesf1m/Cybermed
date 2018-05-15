#include "cybFuzzyPoissonNaiveBayes.h"

CybFuzzyPoissonNaiveBayes::CybFuzzyPoissonNaiveBayes(int variables)
	: CybFuzzyProbability(variables), parameters(variables)
{

}

CybFuzzyPoissonNaiveBayes::CybFuzzyPoissonNaiveBayes(int variables, int nIntervals)
	: CybFuzzyProbability(variables, nIntervals), parameters(variables)
{

}

CybFuzzyPoissonNaiveBayes::~CybFuzzyPoissonNaiveBayes()
{

}

vector<float>& CybFuzzyPoissonNaiveBayes::getParameters()
{
	return parameters;
}

void CybFuzzyPoissonNaiveBayes::setParameters(vector<float>& parameters)
{
	this->parameters = parameters;
}

void CybFuzzyPoissonNaiveBayes::training()
{
	//1st - calculate pertinences
	calcPertinences();

	//2nd - estimate parameters
	parametersEstimation();
}

double CybFuzzyPoissonNaiveBayes::assessment(CybVectorND<float>& auxdata)
{
	//previously calculates logs in order to reduce running time
	vector<double> logs(getNumberOfVariables(), 0);
	double max = auxdata[0];
	for (int i = 1; i < getNumberOfVariables(); i++)
		if (max < auxdata[i])
			max = auxdata[i];

	double aux = 0;
	for (int j = 2; j < max; j++)
	{
		aux += log(j);
		for (int i = 0; i < getNumberOfVariables(); i++)
			if (auxdata[i] == j)
				logs[i] = aux;
	}

	double density = 0;
	for (int i = 0; i < getNumberOfVariables(); i++)
		density += log(getParameters()[i])* auxdata[i] - getParameters()[i] -
		logs[auxdata[i]] + getLogPertinence(auxdata[i], i);

	return density;
}

void CybFuzzyPoissonNaiveBayes::parametersEstimation()
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
		parameters[i] = mean[i];
	}
}