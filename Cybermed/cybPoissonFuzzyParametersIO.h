#pragma once

#include "cybAssessIO.h"
#include "cybPoissonFuzzyParameters.h"
#include <string>

using std::string;

class CybPoissonFuzzyParametersIO : public CybAssessIO
{
public:
	CybPoissonFuzzyParametersIO(string&);

	~CybPoissonFuzzyParametersIO();

	void write(void*);

	void* read();
};

