#include "cybFuzzyBetaNaiveBayes.h"

CybFuzzyBetaNaiveBayes::CybFuzzyBetaNaiveBayes(int variables)
	: CybFuzzyProbability(variables), alpha(variables), beta(variables)
{

}

CybFuzzyBetaNaiveBayes::CybFuzzyBetaNaiveBayes(int variables, int nIntervals)
	: CybFuzzyProbability(variables, nIntervals), alpha(variables),
	beta(variables)
{

}

CybFuzzyBetaNaiveBayes::~CybFuzzyBetaNaiveBayes()
{

}

void CybFuzzyBetaNaiveBayes::setAlpha(vector<float>& alpha)
{
	this->alpha = alpha;
}

vector<float>& CybFuzzyBetaNaiveBayes::getAlpha()
{
	return alpha;
}

void CybFuzzyBetaNaiveBayes::setBeta(vector<float>& beta)
{
	this->beta = beta;
}

vector<float>& CybFuzzyBetaNaiveBayes::getBeta()
{
	return beta;
}

void CybFuzzyBetaNaiveBayes::training()
{
	//1st - calculate pertinences
	calcPertinences();

	//2nd - estimate parameters
	parametersEstimation();
}

double CybFuzzyBetaNaiveBayes::assessment(CybVectorND<float>& auxdata)
{
	double density = 0.0;
	for (int i = 0; i < getNumberOfVariables(); i++)
		density += getLogPertinence(auxdata[i], i) + log(tgamma(alpha[i] +
			beta[i])) - (log(tgamma(alpha[i])) + log(tgamma(beta[i]))) +
			((alpha[i] - 1) * log(auxdata[i])) + ((beta[i] - 1) *
				log(1 - auxdata[i]));

	return density;
}

void CybFuzzyBetaNaiveBayes::parametersEstimation()
{
	int size = data[0].getDimension();

	//1st - calculate mean
	vector<float> mean(getNumberOfVariables());
	for (int i = 0; i < getNumberOfVariables(); i++)
	{
		for (int j = 0; j < size; j++)
		{
			mean[i] += data[i][j];
		}
		mean[i] /= size;
	}

	//2nd - calculate standard deviation
	vector<float> variance(getNumberOfVariables());
	for (int i = 0; i < getNumberOfVariables(); i++)
	{
		for (int j = 0; j < size; j++)
		{
			variance[i] += pow(data[i][j] - mean[i], 2);
		}
		variance[i] /= (size - 1);
	}

	for (int i = 0; i < getNumberOfVariables(); i++)
	{
		variance[i] = (size - 1) * (variance[i] / size); //fator de correcao populacional
	}

	//1st - estimate alpha
	for (int i = 0; i < getNumberOfVariables(); i++)
	{
		alpha[i] = mean[i] * (((mean[i] * (1 - mean[i])) / variance[i]) - 1);
	}

	//2nd - estimate beta
	for (int i = 0; i < getNumberOfVariables(); i++)
	{
		beta[i] = (alpha[i] / mean[i]) - alpha[i];
	}
}