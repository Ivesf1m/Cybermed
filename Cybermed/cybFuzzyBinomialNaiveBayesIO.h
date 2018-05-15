#pragma once

#include "cybAssessIO.h"
#include "cybFuzzyBinomialNaiveBayes.h"
#include <string>

using std::string;

class CybFuzzyBinomialNaiveBayesIO : public CybAssessIO
{
public:
	CybFuzzyBinomialNaiveBayesIO(string&);
	~CybFuzzyBinomialNaiveBayesIO();

	void write(void*);
	void* read();
};

