#pragma once

#include <string>

using std::string;

class CybMenuNode
{
public:
	CybMenuNode(string& _name, int _event);
	~CybMenuNode();

	string name;
	int event;
};

