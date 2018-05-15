#pragma once

#include <cmath>
#include <iostream>
#include "cybAssess.h"


class CybBayesianNetwork : public CybAssess
{
public:
	CybBayesianNetwork(int nodesNumber);

	virtual ~CybBayesianNetwork();

	void setCovariance(vector<float>&);

	double score();

	vector<float>& getCovariance();

	virtual void initModel();

	virtual void toString();

	virtual void training();

	virtual double assessment(CybVectorND<float>&);

	float getMean(int nodeId);

	float getVariance(int nodeId);

	void addArc(int parentId, int sonId);

	void removeArc(int parentId, int sonId);

	bool hasArc(int parentId, int sonId);

	int getNodesNumber();

	vector<int> getParents(int nodeId);

	vector<int> getSons(int nodeId);

	void setMean(float newMean, int nodeId);

protected:

	virtual void initData();

	virtual double getJointProb(CybVectorND<float>&, vector<int>&);

	virtual void updateProbability(int);

	void findClosedPaths(int, int, vector<vector<int>>&);

	virtual double getProbability(CybVectorND<float>&, int);

	virtual double getProbability(CybVectorND<float>&);

private:
	vector<float> covariance;
	vector<float> mean;
	vector<bool> dag;
	vector<vector<vector<int>>> allPaths;
	float error;
	int interationsNumber;
	int nodesNumber;
};

