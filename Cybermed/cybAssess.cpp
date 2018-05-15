#include "cybAssess.h"

CybAssess::CybAssess()
{

}

CybAssess::CybAssess(int numVariables)
	: numberOfVariables(numVariables)
{

}

CybAssess::~CybAssess()
{

}

vector<CybVectorND<float>>& CybAssess::getData()
{
	return data;
}

void CybAssess::addData(CybVectorND<float>& data)
{
	this->data.push_back(data);
}

int CybAssess::getNumberOfVariables()
{
	return numberOfVariables;
}

void CybAssess::setNumberOfVariables(int numVariables)
{
	this->numberOfVariables = numVariables;
}