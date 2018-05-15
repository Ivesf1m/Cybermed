#pragma once

#include "cybVectorND.h"
#include <cmath>
#include <vector>

class CybAssess
{
protected:
	//Stores the data vectors used in assess (for training or assessment).
	vector<CybVectorND<float>> data;

	//Stores the amount of variables being used.
	int numberOfVariables;

public:

	/**
	* Constructor.
	*/
	CybAssess();

	/**
	* Constructor.
	*/
	CybAssess(int);

	/**
	* Destructor.
	*/
	~CybAssess();

	/**
	* This method returns a reference for joint of data.
	*
	* @param void.
	* @return vector<CybVectorND<float>>&: A reference for a list with data.
	*/
	vector<CybVectorND<float>>& getData();

	/**
	* This method adds new data to the list.
	*
	* @param CybVectorND<float>& data: The new data added.
	* @return void.
	*/
	void addData(CybVectorND<float>& data);

	/**
	* This method returns the amount of data being used.
	*
	* @param void.
	* @return int: Amount of variables to be used.
	*/
	int getNumberOfVariables();

	/**
	* This method sets the amount of variables to be used.
	*
	* @param int: Amount of variables to be used.
	* @return void.
	*/
	void setNumberOfVariables(int);

	/**
	* Pure virtual method. It will implement the training
	* for the assess method.
	*
	* @param void.
	* @return void.
	*/
	virtual void training() = 0;

	/**
	* Pure method. It will implement the assessment
	* for a vector of data.
	*
	* @param CybVectorND<float>& The classified data.
	* @return void.
	*/
	virtual double assessment(CybVectorND<float>&) = 0;
};
