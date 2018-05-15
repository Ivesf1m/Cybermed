#include "cybGammaNaiveBayes.h"

CybGammaNaiveBayes::CybGammaNaiveBayes(int variables)
	: CybAssess(variables), alpha(variables), beta(variables)
{

}

CybGammaNaiveBayes::~CybGammaNaiveBayes()
{

}

void CybGammaNaiveBayes::setAlpha(vector<float>& alpha)
{
	this->alpha = alpha;
}

vector<float>& CybGammaNaiveBayes::getAlpha()
{
	return alpha;
}

void CybGammaNaiveBayes::setBeta(vector<float>& beta)
{
	this->beta = beta;
}

vector<float>& CybGammaNaiveBayes::getBeta()
{
	return beta;
}

void CybGammaNaiveBayes::training()
{
	//estimate parameters
	parametersEstimation();
}

double CybGammaNaiveBayes::assessment(CybVectorND<float>& auxdata)
{
	double density = 0.0;
	for (int i = 0; i < getNumberOfVariables(); i++)
		if (auxdata[i] > 0)
			density += alpha[i] * log(beta[i]) + (alpha[i] - 1) * log(auxdata[i])
			- beta[i] * auxdata[i] - log(tgamma(alpha[i]));
		else
			density += 0;

	return density;
}

void CybGammaNaiveBayes::parametersEstimation()
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