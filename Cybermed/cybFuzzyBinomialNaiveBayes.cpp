#include "cybFuzzyBinomialNaiveBayes.h"



CybFuzzyBinomialNaiveBayes::CybFuzzyBinomialNaiveBayes(int variables)
	: CybFuzzyProbability(variables), probability(variables), N(variables)
{

}

CybFuzzyBinomialNaiveBayes::CybFuzzyBinomialNaiveBayes(int variables,
	int nIntervals)
	: CybFuzzyProbability(variables, nIntervals), probability(variables),
	N(variables)
{

}

CybFuzzyBinomialNaiveBayes::~CybFuzzyBinomialNaiveBayes()
{

}

vector<float>& CybFuzzyBinomialNaiveBayes::getProbability()
{
	return probability;
}

void CybFuzzyBinomialNaiveBayes::setProbability(vector<float>& probability)
{
	this->probability = probability;
}

vector<int>& CybFuzzyBinomialNaiveBayes::getN()
{
	return N;
}

void CybFuzzyBinomialNaiveBayes::setN(vector<int>& N)
{
	this->N = N;
}

void CybFuzzyBinomialNaiveBayes::training()
{
	//1st - calculate pertinences
	calcPertinences();

	//2nd - estimate parameters
	parametersEstimation();
}

double CybFuzzyBinomialNaiveBayes::assessment(CybVectorND<float>& auxdata)
{
	//previously calculates logs in order to reduce running time
	double max = auxdata[0];
	for (int i = 1; i < getNumberOfVariables(); i++)
		if (max < auxdata[i])
			max = auxdata[i];

	int imax = static_cast<int>(max);
	vector<double> logs(imax, 0);
	for (int j = 2; j < imax; j++)
	{
		logs[j] = logs[j - 1] + log(j);
	}

	double density = 0.0;
	for (int i = 0; i < getNumberOfVariables(); i++)
		density += getLogPertinence(auxdata[i], i) + logs[N[i]] -
		(logs[auxdata[i]] + logs[N[i] - auxdata[i]]) +
		(auxdata[i] * log(probability[i])) + ((N[i] - auxdata[i]) *
			log(1 - probability[i]));

	return density;
}

void CybFuzzyBinomialNaiveBayes::parametersEstimation()
{
	int size = data[0].getDimension();

	//1st - estimate N
	for (int i = 0; i < getNumberOfVariables(); i++)
			N[i] = size;

	//2nd - estimate probability p
	double c1 = 0.1, c2 = 1;
	for (int i = 0; i < getNumberOfVariables(); i++)
		for (int j = 0; j < size; j++)
			probability[i] += data[i][j];

	for (int i = 0; i < getNumberOfVariables(); i++)
		probability[i] = (c1 + probability[i]) / (c2 + size);

	for (int i = 0; i < getNumberOfVariables(); i++)
		probability[i] = probability[i] / N[i];
}