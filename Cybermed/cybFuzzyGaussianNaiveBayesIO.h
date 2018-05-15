#pragma once

#include "cybAssessIO.h"
#include "cybFuzzyGaussianNaiveBayes.h"
#include <string>

using std::string;

class CybFuzzyGaussianNaiveBayesIO : public CybAssessIO
{
public:
	CybFuzzyGaussianNaiveBayesIO(string&);
	~CybFuzzyGaussianNaiveBayesIO();

	void write(void*);

	void* read();
};

