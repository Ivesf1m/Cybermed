#pragma once

#include "cybAssessIO.h"
#include "cybFuzzyPoissonNaiveBayes.h"
#include <string>

using std::string;

class CybFuzzyPoissonNaiveBayesIO : public CybAssessIO
{
public:
	CybFuzzyPoissonNaiveBayesIO(string&);
	~CybFuzzyPoissonNaiveBayesIO();

	void write(void*);

	void* read();
};

