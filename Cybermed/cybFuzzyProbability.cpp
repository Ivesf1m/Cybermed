#include "cybFuzzyProbability.h"



CybFuzzyProbability::CybFuzzyProbability(int variables)
	: CybAssess(variables), pertinences(NULL)
{

}

CybFuzzyProbability::CybFuzzyProbability(int variables, int nIntervals)
	: CybAssess(variables), nIntervals(nIntervals)
{
	pertinences = new CybMatrix<pair<pair<double, double>, double>>(nIntervals,
		variables);
}

CybFuzzyProbability::~CybFuzzyProbability()
{
	delete pertinences;
}

CybMatrix< pair< pair<double, double>, double> >* 
CybFuzzyProbability::getPertinences()
{
	return pertinences;
}

void CybFuzzyProbability::setPertinences(CybMatrix< pair< pair<double, double>,
	double> >* pertinences)
{
	this->pertinences = pertinences;
}

double CybFuzzyProbability::getPertinence(float data, int variable)
{
	for (int l = 0; l < nIntervals; ++l) {
		if (l == nIntervals - 1) {
			if (data >= (*pertinences)[l][variable].first.first &&
				data <= (*pertinences)[l][variable].first.second)
				return (*pertinences)[l][variable].second;
		}
		else if (data >= (*pertinences)[l][variable].first.first &&
			data < (*pertinences)[l][variable].first.second)
			return (*pertinences)[l][variable].second;
	}
	return 0.0;
}

double CybFuzzyProbability::getLogPertinence(float data, int variable)
{
	double p = getPertinence(data, variable);
	if (p != 0.0)
		return log(p);
	else return 0.0;
}

int CybFuzzyProbability::getNIntervals()
{
	return nIntervals;
}

void CybFuzzyProbability::setNIntervals(int nIntervals)
{
	this->nIntervals = nIntervals;
}

void CybFuzzyProbability::calcPertinences()
{
	int size = data[0].getDimension();

	//1st - calculate sturges
	int sturges = static_cast<int>(round(1 + (3.22 * log10(size))));
	nIntervals = sturges;

	delete pertinences;
	pertinences = new CybMatrix<pair<pair<double, double>, double>>(sturges,
		getNumberOfVariables());

	for (int i = 0; i < getNumberOfVariables(); ++i) {
		//2nd - get max and min
		float max = data[i][0];
		float min = data[i][0];
		
		for (int j = 1; j < size; ++j) {
			if (max < data[i][j])
				max = data[i][j];
			if (min > data[i][j])
				min = data[i][j];
		}

		//3rd - calculate frequencies
		vector<pair<pair<double, double>, double>> freq(sturges);
		double step = (max - min) / sturges;

		for (int l = 0; l < sturges; ++l) {
			freq[l].first.first = min + step * l;
			freq[l].first.second = freq[l].first.first + step;
		}

		for (int l = 0; l < sturges; ++l)
			freq[l].second = 0.0;

		for(int j = 0; j < size; ++j)
			for (int l = 0; l < sturges; ++l) {
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
		for (int l = 0; l < sturges; ++l)
			freq[l].second = freq[l].second / size;

		//5th - calculate pertinences
		double maxFreq = freq[0].second;
		for (int l = 1; l < sturges; ++l)
			if (maxFreq < freq[l].second)
				maxFreq = freq[l].second;

		for (int l = 0; l < sturges; ++l) {
			(*pertinences)[l][i].first.first = freq[l].first.first;
			(*pertinences)[l][i].first.second = freq[l].first.second;
			if (freq[l].second == 0)
				(*pertinences)[l][i].second = 0.001;
			else
				(*pertinences)[l][i].second = freq[l].second / maxFreq;
		}
	}
}
