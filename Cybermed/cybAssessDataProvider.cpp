#include "cybAssessDataProvider.h"

CybAssessDataProvider::CybAssessDataProvider()
	: data(NULL), usedData(false)
{

}


CybAssessDataProvider::~CybAssessDataProvider()
{

}

CybVectorND<float>* CybAssessDataProvider::getData()
{
	usedData = true;
	return data;
}

void CybAssessDataProvider::setData(CybVectorND<float>* data)
{
	if (!usedData)
		delete this->data;
	this->data = data;
	usedData = false;
}
