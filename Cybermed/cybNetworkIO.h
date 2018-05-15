#pragma once

#include "cybAssessIO.h"
#include "cybBayesianNetwork.h"
#include <string>

using std::string;

class CybNetworkIO : public CybAssessIO
{
public:
	CybNetworkIO(string&);

	~CybNetworkIO();

	void write(void*);

	void* read();
};

