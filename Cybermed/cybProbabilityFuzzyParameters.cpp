#include "cybProbabilityFuzzyParameters.h"

CybProbabilityFuzzyParameters::CybProbabilityFuzzyParameters(int variables,
	int cuts)
	: CybAssess(variables), parameters(cuts + 1, variables)
{
	this->alphaCuts = cuts;
}

CybProbabilityFuzzyParameters::~CybProbabilityFuzzyParameters()
{

}

CybMatrix< pair<double, double> >&
CybProbabilityFuzzyParameters::getFuzzyParameters()
{
	return parameters;
}

void CybProbabilityFuzzyParameters::setFuzzyParameters(
	CybMatrix< pair<double, double> >& parameters)
{
	this->parameters = parameters;
}

int CybProbabilityFuzzyParameters::getAlphaCuts()
{
	return alphaCuts;
}

void CybProbabilityFuzzyParameters::setAlphaCuts(int alphaCuts)
{
	this->alphaCuts = alphaCuts;
}