#include "cybFuzzyGaussianNaiveBayes.h"

CybFuzzyGaussianNaiveBayes::CybFuzzyGaussianNaiveBayes(int variables)
	: CybFuzzyProbability(variables), mean(variables), stdDev(variables)
{

}

CybFuzzyGaussianNaiveBayes::CybFuzzyGaussianNaiveBayes(int variables,
	int nIntervals)
	: CybFuzzyProbability(variables, nIntervals), mean(variables),
	stdDev(variables)
{

}

CybFuzzyGaussianNaiveBayes::~CybFuzzyGaussianNaiveBayes()
{

}

vector<float>& CybFuzzyGaussianNaiveBayes::getMean()
{
	return mean;
}

void CybFuzzyGaussianNaiveBayes::setMean(vector<float>& mean)
{
	this->mean = mean;
}

vector<float>& CybFuzzyGaussianNaiveBayes::getStdDev()
{
	return stdDev;
}

void CybFuzzyGaussianNaiveBayes::setStdDev(vector<float>& stdDev)
{
	this->stdDev = stdDev;
}

void CybFuzzyGaussianNaiveBayes::training()
{
	//1st - calculate pertinences
	calcPertinences();

	//2nd - estimate parameters
	parametersEstimation();
}

double CybFuzzyGaussianNaiveBayes::assessment(CybVectorND<float>& auxdata)
{
	double density = 0;
	for (int i = 0; i < getNumberOfVariables(); i++)
		density += log(1 / stdDev[i]) - (pow(auxdata[i] - mean[i], 2) /
		(2 * pow(stdDev[i], 2))) + getLogPertinence(auxdata[i], i);

	return density;
}

void CybFuzzyGaussianNaiveBayes::parametersEstimation()
{
	int size = data[0].getDimension();

	//1st - calculate mean
	for (int i = 0; i < getNumberOfVariables(); i++)
	{
		for (int j = 0; j < size; j++)
		{
			mean[i] += data[i][j];
		}
		mean[i] /= size;
	}

	//2nd - calculate standard deviation
	for (int i = 0; i < getNumberOfVariables(); i++)
	{
		for (int j = 0; j < size; j++)
		{
			stdDev[i] += pow(data[i][j] - mean[i], 2);
		}
		stdDev[i] /= (size - 1);
	}
}