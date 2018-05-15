#pragma once

#include "cybAssessIO.h"
#include "cybFuzzyGammaNaiveBayes.h"
#include <string>

using std::string;

class CybFuzzyGammaNaiveBayesIO : public CybAssessIO
{
public:
	CybFuzzyGammaNaiveBayesIO(string&);
	~CybFuzzyGammaNaiveBayesIO();

	void write(void*);

	void* read();
};

