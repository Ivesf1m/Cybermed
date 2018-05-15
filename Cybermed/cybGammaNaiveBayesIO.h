#pragma once

#include "cybAssessIO.h"
#include "cybGammaNaiveBayes.h"
#include <string>

using std::string;

class CybGammaNaiveBayesIO : public CybAssessIO
{
public:
	CybGammaNaiveBayesIO(string&);
	~CybGammaNaiveBayesIO();

	void write(void*);

	void* read();
};

