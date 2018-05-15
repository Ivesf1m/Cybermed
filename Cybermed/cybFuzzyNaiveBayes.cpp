#include "cybFuzzyNaiveBayes.h"

CybFuzzyNaiveBayes::CybFuzzyNaiveBayes(int variables)
	: CybFuzzyProbability(variables)
{

}


CybFuzzyNaiveBayes::CybFuzzyNaiveBayes(int variables, int nIntervals)
	: CybFuzzyProbability(variables, nIntervals)
{
	probability = new CybMatrix < pair< pair<double, double>, double> >
		(nIntervals, variables);
}


CybFuzzyNaiveBayes::~CybFuzzyNaiveBayes()
{
	delete probability;
}

CybMatrix < pair< pair<double, double>, double> >*
CybFuzzyNaiveBayes::getProbability()
{
	return probability;
}

double CybFuzzyNaiveBayes::getLogProbability(float data, int variable)
{
	for (int l = 0; l < nIntervals; l++)
	{
		if (l == nIntervals - 1) {
			if (data >= (*this->getProbability())[l][variable].first.first &&
				data <= (*this->getProbability())[l][variable].first.second)
				return log((*this->getProbability())[l][variable].second);
		}
		else if (data >= (*this->getProbability())[l][variable].first.first &&
			data < (*this->getProbability())[l][variable].first.second)
			return log((*this->getProbability())[l][variable].second);
	}
	return 0;
}

void CybFuzzyNaiveBayes::setProbability(
	CybMatrix < pair< pair<double, double>, double> >* probability)
{
	this->probability = probability;
}

void CybFuzzyNaiveBayes::training()
{
	//1st - calculate pertinences
	calcPertinences();

	//2nd - estimate parameters
	parametersEstimation();
}

double CybFuzzyNaiveBayes::assessment(CybVectorND<float>& auxdata)
{
	double density = 0.0;
	for (int i = 0; i < getNumberOfVariables(); i++)
		density += getLogProbability(auxdata[i], i) +
		getLogPertinence(auxdata[i], i);

	return density;
}

void CybFuzzyNaiveBayes::parametersEstimation()
{
	int size = data[0].getDimension();

	//1st - calculate sturges
	int sturges = round(1 + (3.322*log10(size)));

	probability = new CybMatrix < pair< pair<double, double>, double> >
		(sturges, getNumberOfVariables());

	for (int i = 0; i < getNumberOfVariables(); i++)
	{
		//2nd - get max and min
		float max = data[i][0];
		float min = data[i][0];

		for (int j = 1; j < size; j++) {
			if (max < data[i][j])
				max = data[i][j];
			if (min > data[i][j])
				min = data[i][j];
		}

		//3rd - calculate frequencies	
		vector < pair < pair < double, double >, double > > freq(sturges);
		double step = (max - min) / sturges;

		for (int l = 0; l < sturges; l++)
		{
			freq[l].first.first = min + step * l;
			freq[l].first.second = freq[l].first.first + step;
		}

		for (int l = 0; l < sturges; l++)
			freq[l].second = 0;

		for (int j = 0; j < size; j++)
			for (int l = 0; l < sturges; l++) {
				if (l == sturges - 1) {
					if (data[i][j] >= freq[l].first.first &&
						data[i][j] <= freq[l].first.second)
						freq[l].second += 1;
				}
				else if (data[i][j] >= freq[l].first.first &&
					data[i][j] < freq[l].first.second)
					freq[l].second += 1;
			}

		//4th - calculate relative frequencies
		for (int l = 0; l < sturges; l++) {
			(*probability)[l][i].first.first = freq[l].first.first;
			(*probability)[l][i].first.second = freq[l].first.second;
			(*probability)[l][i].second = freq[l].second / size;
		}
	}
}
