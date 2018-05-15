#pragma once

#include "cybAssessIO.h"
#include "cybFuzzyNaiveBayes.h"
#include <string>

using std::string;

class CybFuzzyNaiveBayesIO : public CybAssessIO
{
public:
	CybFuzzyNaiveBayesIO(string&);
	~CybFuzzyNaiveBayesIO();

	void write(void*);

	void* read();
};

