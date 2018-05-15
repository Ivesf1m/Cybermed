#pragma once

#include "cybAssessIO.h"
#include "cybFuzzyBetaNaiveBayes.h"
#include <string>

using std::string;

class CybFuzzyBetaNaiveBayesIO : public CybAssessIO
{
public:
	CybFuzzyBetaNaiveBayesIO(string&);
	~CybFuzzyBetaNaiveBayesIO();

	void write(void*);

	void* read();
};

