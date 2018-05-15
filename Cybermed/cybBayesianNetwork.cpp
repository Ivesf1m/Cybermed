#include "cybBayesianNetwork.h"
#include <algorithm>
#include "cybInfoNode.h"
#include "cybMatrix.h"

using std::find;


CybBayesianNetwork::CybBayesianNetwork(int nodesNumber)
	: nodesNumber(nodesNumber), error(1e-4f), interationsNumber(10000)
{
	mean.resize(nodesNumber);
	covariance.resize(nodesNumber * nodesNumber);
	allPaths.resize(nodesNumber);
	dag.resize(nodesNumber * nodesNumber);
	
	for (int i = 0; i < nodesNumber; ++i)
		for (int j = 0; j < nodesNumber; ++j)
			dag[i * nodesNumber + j] = false;
}


CybBayesianNetwork::~CybBayesianNetwork()
{

}

void CybBayesianNetwork::initData()
{
	for (int i = 0; i < nodesNumber; ++i) {
		float mean = 0.0f;

		for (unsigned int j = 0; j < getData().size(); ++j)
			mean += getData()[j][i];

		setMean(mean / getData().size(), i);
	}

	for (int i = 0; i < nodesNumber; ++i) {
		for (int j = 0; j < nodesNumber; ++j) {
			for (unsigned int k = 0; k < getData().size(); ++k) {
				covariance[i * nodesNumber + j] +=
					(getData()[k][i] - getMean(i)) *
					(getData()[k][j] - getMean(j));
			}
			covariance[i * nodesNumber + j] /= getData().size();
		}
	}
}

void CybBayesianNetwork::findClosedPaths(int start, int end,
	vector<vector<int>>& list)
{
	static vector<int> current;
	vector<int> aux;
	vector<int>::iterator it;

	if ((it = find(current.begin(), current.end(), start)) != current.end())
		return;

	current.push_back(start);

	if (start == end) {
		for (unsigned int i = 0; i < current.size(); ++i)
			aux.push_back(current[i]);
		list.push_back(aux);
		current.erase(it);
		return;
	}

	aux = getSons(start);

	for (unsigned int i = 0; i < aux.size(); ++i)
		findClosedPaths(aux[i], end, list);
	aux.clear();
	current.erase(it);
}

void CybBayesianNetwork::updateProbability(int nodeId)
{
	double sum = 0.0;
	double prob = 0.0;
	double aux = 0.0;
	double shift = 0.0;
	double oldScore = 0.0;
	double newScore = 0.0;
	int control = 0;
	double lastDifference = 0.0;
	vector<int> parents;

	do {
		if (control > interationsNumber)
			return;

		oldScore = score();
		
		for (unsigned int i = 0; i < allPaths[nodeId].size(); ++i)
			for (unsigned int w = 0; w < allPaths[nodeId][i].size(); ++w)
				if (allPaths[nodeId][i][w] != nodeId)
					if (find(parents.begin(), parents.end(), 
						allPaths[nodeId][i][w]) == parents.end())
						parents.push_back(allPaths[nodeId][i][w]);

		for (unsigned int i = 0; i < parents.size(); ++i) {
			for (unsigned int j = 0; j < getData().size(); ++j) {
				prob = getProbability(getData()[j], parents[i]);
				shift += prob * (getData()[j][parents[i]] -
					getMean(parents[i]));
				sum += prob;
			}
		}

		shift /= sum;

		if (isnan(shift) || isinf(shift))
			return;

		setMean(static_cast<float>(getMean(nodeId) + shift), nodeId);
		newScore = score();
		control++;

		cout << "difference: " << newScore - oldScore << endl;
	} while ((lastDifference = newScore - oldScore) > error);
}

void CybBayesianNetwork::initModel()
{
	for (int i = 0; i < nodesNumber; ++i)
		allPaths[i].clear();

	for(int i = 0; i < nodesNumber; ++i)
		for (int j = 0; j < nodesNumber; ++j) {
			vector<int> aux = getParents(i);
			if (i != j && aux.size() == 0)
				findClosedPaths(i, j, allPaths[j]);
		}
}

double CybBayesianNetwork::score()
{
	double prob = 0.0;
	double sco = 0.0;

	for (unsigned int i = 0; i < getData().size(); ++i) {
		prob = getProbability(getData()[i]);
		if (prob > 1 || isnan(prob) || isinf(prob))
			return -1e200;
		else
			sco += log(prob);
	}

	return sco;
}

void CybBayesianNetwork::setCovariance(vector<float>& cov)
{
	covariance = cov;
}

double CybBayesianNetwork::getJointProb(CybVectorND<float>& data, 
	vector<int>& parents)
{
	if (parents.size() == 1) {
		if (getVariance(parents[0]) == 0) {
			if (getMean(parents[0]) == data[parents[0]])
				return 1;
			else return 0;
		}
		else {
			return exp(-pow(data[parents[0]] - getMean(parents[0]), 2) /
				(2 * getVariance(parents[0]))) / sqrt(6.283185841);
		}
	}

	vector<float> aux1(parents.size(), 0.0f);
	vector<float> aux2(parents.size(), 0.0f);
	CybMatrix<float> matrix(parents.size(), parents.size());
	CybMatrix<float> inverse(parents.size(), parents.size());
	double res = 0.0;

	if (covariance.size() == 0)
		cerr << "Empty covariance matrix" << endl;
	for (unsigned int i = 0; i < parents.size(); ++i)
		for (unsigned int j = 0; j < parents.size(); ++j)
			matrix[i][j] = covariance[parents[i] * nodesNumber + parents[j]];

	double det = matrix.determinant();

	if (det == 0) {
		double aux = 1.0;
		vector<int> nodes;

		for (unsigned int i = 1; i < parents.size(); ++i)
			nodes.push_back(parents[i]);

		aux = getJointProb(data, nodes);

		nodes.clear();
		nodes.push_back(parents[0]);

		aux *= getJointProb(data, nodes);
		return aux;
	}

	inverse = matrix.invertMatrix();

	for (unsigned int i = 0; i < parents.size(); ++i)
		aux1[i] = (data[parents[i]] - getMean(parents[i]));

	for (unsigned int i = 0; i < parents.size(); ++i)
		for (unsigned int j = 0; j < parents.size(); ++j)
			aux2[i] += aux1[j] * matrix[i][j];

	for (unsigned int i = 0; i < parents.size(); ++i)
		res += aux1[i] * aux2[i];
	
	int size = static_cast<int>(parents.size());
	return sqrt(1.0 / det) * exp(-(res / 2.0)) * pow(6.283185841, -size / 2.0);
}

double CybBayesianNetwork::getProbability(CybVectorND<float>& data)
{
	double res = 1.0;
	for (int i = 0; i < nodesNumber; ++i)
		res *= getProbability(data, i);
	return res;
}

vector<float>& CybBayesianNetwork::getCovariance()
{
	return covariance;
}

double CybBayesianNetwork::getProbability(CybVectorND<float>& data, int nodeId)
{
	double parents = 0.0;
	double prob = 1.0;

	if (allPaths[nodeId].size() == 0) {
		vector<int> aux;
		aux.push_back(nodeId);
		prob = getJointProb(data, aux);
		return prob;
	}

	for (unsigned int i = 0; i < allPaths[nodeId].size(); ++i) {
		parents = 0.0;
		vector<int> aux = getParents(nodeId);

		for (unsigned int j = 0; j < aux.size(); ++j)
			for (unsigned int k = 0; k < allPaths[aux[j]].size(); ++k)
				parents += getJointProb(data, allPaths[aux[j]][k]);

		if (parents)
			prob *= getJointProb(data, allPaths[nodeId][i]) / parents;
		else
			prob *= getJointProb(data, allPaths[nodeId][i]);
	}

	return prob;
}

void CybBayesianNetwork::toString()
{
	for (int i = 0; i < nodesNumber; ++i) {
		for (int j = 0; j < nodesNumber; ++j)
			cout << dag[i * nodesNumber + j] << " ";
		cout << endl;
	}

	for (int i = 0; i < nodesNumber; ++i) {
		cout << "id " << i << ": ";
		cout << "(" << getMean(i) << ", " << getVariance(i) << ")" << endl;
	}
}

void CybBayesianNetwork::training()
{
	CybInfoNode* node = NULL;
	CybInfoNode* greater = NULL;
	bool hasMore = true;
	initData();
	initModel();
	double oldScore;
	vector<int> control(nodesNumber * nodesNumber, 0);

	while (hasMore) {
		oldScore = score();
		hasMore = false;

		for(int i = 0; i < nodesNumber; ++i)
			for(int j = 0; j < nodesNumber; ++j)
				if (i != j && !control[i * nodesNumber + j] &&
					!control[j * nodesNumber + i]) {
					node = new CybInfoNode(*this, i, j);
					if (node->getScore() != -1e200) {
						if (greater == NULL)
							greater = node;
						else if (node->getScore() > greater->getScore()) {
							delete greater;
							greater = node;
						}
						else
							delete node;
					}
					else delete node;
				}

		if (greater) {
			if (greater->getScore() > oldScore) {
				oldScore = greater->getScore();
				control[greater->getParent() * nodesNumber + greater->getSon()]
					= 1;
				control[greater->getSon() * nodesNumber + greater->getParent()]
					= 1;
				addArc(greater->getParent(), greater->getSon());
				initModel();
				updateProbability(greater->getSon());
				greater = NULL;
				hasMore = true;
			}
		}
	}

	toString();
	cout << "finalize" << endl;
}

double CybBayesianNetwork::assessment(CybVectorND<float>& v)
{
	return getProbability(v);
}

void CybBayesianNetwork::setMean(float newMean, int nodeId)
{
	mean[nodeId] = newMean;
}

vector<int> CybBayesianNetwork::getSons(int nodeId)
{
	vector<int> ret;
	for (int i = 0; i < nodesNumber; ++i)
		if (hasArc(nodeId, i))
			if (find(ret.begin(), ret.end(), i) == ret.end())
				ret.push_back(i);
	return ret;
}

float CybBayesianNetwork::getMean(int nodeId)
{
	return mean[nodeId];
}

float CybBayesianNetwork::getVariance(int nodeId)
{
	return covariance[nodeId * nodesNumber + nodeId];
}

void CybBayesianNetwork::addArc(int parentId, int sonId)
{
	dag[parentId * nodesNumber + sonId] = true;
}

void CybBayesianNetwork::removeArc(int parentId, int sonId)
{
	dag[parentId * nodesNumber + sonId] = false;
}

bool CybBayesianNetwork::hasArc(int parentId, int sonId)
{
	return dag[parentId * nodesNumber + sonId];
}

int CybBayesianNetwork::getNodesNumber()
{
	return nodesNumber;
}

vector<int> CybBayesianNetwork::getParents(int nodeId)
{
	vector<int> ret;
	for (int i = 0; i < nodesNumber; ++i)
		if (hasArc(i, nodeId))
			if (find(ret.begin(), ret.end(), i) == ret.end())
				ret.push_back(i);
	return ret;
}
