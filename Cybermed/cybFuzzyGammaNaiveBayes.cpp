#include "cybFuzzyGammaNaiveBayes.h"

CybFuzzyGammaNaiveBayes::CybFuzzyGammaNaiveBayes(int variables)
	: CybFuzzyProbability(variables), alpha(variables), beta(variables)
{

}

CybFuzzyGammaNaiveBayes::CybFuzzyGammaNaiveBayes(int variables, int nIntervals)
	: CybFuzzyProbability(variables, nIntervals), alpha(variables),
	beta(variables)
{

}

CybFuzzyGammaNaiveBayes::~CybFuzzyGammaNaiveBayes()
{

}

void CybFuzzyGammaNaiveBayes::setAlpha(vector<float>& alpha)
{
	this->alpha = alpha;
}

vector<float>& CybFuzzyGammaNaiveBayes::getAlpha()
{
	return alpha;
}

void CybFuzzyGammaNaiveBayes::setBeta(vector<float>& beta)
{
	this->beta = beta;
}

vector<float>& CybFuzzyGammaNaiveBayes::getBeta()
{
	return beta;
}

void CybFuzzyGammaNaiveBayes::training()
{
	//1st - calculate pertinences
	calcPertinences();

	//2nd - estimate parameters
	parametersEstimation();
}

double CybFuzzyGammaNaiveBayes::assessment(CybVectorND<float>& auxdata)
{
	long double density = 0.0;
	for (int i = 0; i < getNumberOfVariables(); i++)
		density += getLogPertinence(auxdata[i], i) + alpha[i] * log(beta[i])
		+ (alpha[i] - 1) * log(auxdata[i]) - beta[i] * auxdata[i] -
		log(tgammal(alpha[i]));

	return density;
}

void CybFuzzyGammaNaiveBayes::parametersEstimation()
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
		//fator de correcao populacional
		variance[i] = (size - 1) * (variance[i] / size); 
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
