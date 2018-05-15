#pragma once

#include "cybVectorND.h"

/**
* @class	CybAssessDataProvider
* @file	cybAssessDataProvider.h
* @short	The base class for all data providers (See CybAssessLinker).
*
* This class does take the data and catch it for the assess method.
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	1.0
* @date	2008, August
*
*/

class CybAssessDataProvider
{
public:

	/**
	* Constructor.
	*/
	CybAssessDataProvider();

	~CybAssessDataProvider();

	/**
	* This method returns the current data.
	*
	* @param void.
	* @return CybVectorND<float>& Returns the current data.
	*/
	CybVectorND<float>* getData();

	/**
	* This method sets the current data.
	*
	* @param CybVectorND<float>& data The new current data.
	* @return void.
	*/
	void setData(CybVectorND<float>* data);

private:
	CybVectorND<float>* data; //the current vector
	bool usedData; //indicated if the vector was caught
};

