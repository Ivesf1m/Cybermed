#pragma once

#include "cybAssessIO.h"
#include "cybMLM.h"
#include <string>

using std::string;

class CybMLMIO : public CybAssessIO
{
public:
	CybMLMIO(string&);

	~CybMLMIO();

	void write(void*);

	void* read();
};

