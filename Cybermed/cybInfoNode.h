#pragma once

#include "cybBayesianNetwork.h"

class CybInfoNode
{
public:
	CybInfoNode(CybBayesianNetwork& net, int parent, int son)
		: parent(parent), son(son)
	{
		net.addArc(parent, son);
		net.initModel();
		score = net.score();
		net.removeArc(parent, son);
		net.initModel();
	}

	double getScore()
	{
		return score;
	}

	int getParent()
	{
		return parent;
	}

	int getSon()
	{
		return son;
	}

private:
	int parent;
	int son;
	double score;
};