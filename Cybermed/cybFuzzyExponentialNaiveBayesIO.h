#pragma once

#include "cybAssessIO.h"
#include "cybFuzzyExponentialNaiveBayes.h"
#include <string>

using std::string;

class CybFuzzyExponentialNaiveBayesIO : public CybAssessIO
{
public:
	CybFuzzyExponentialNaiveBayesIO(string&);
	~CybFuzzyExponentialNaiveBayesIO();

	void write(void*);

	void* read();
};

