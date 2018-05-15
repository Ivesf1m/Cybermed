#pragma once

#include "cybAssessIO.h"
#include "cybGaussian.h"
#include <string>

using std::string;

class CybGaussianIO : public CybAssessIO
{
public:
	CybGaussianIO(string&);

	~CybGaussianIO();

	void write(void*);

	void* read();
};

