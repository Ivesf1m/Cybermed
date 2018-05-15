#pragma once

#include "cybAssess.h"
#include "cybMatrix.h"

/**
* @class	CybGaussian
* @file	cybGaussian.h
* @short	The class for the Gaussian assess method.
*
* This class does the specification for Gaussian assess methods.
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	3.1.1
* @date	2016, September
*
*/

class CybGaussian : public CybAssess
{
public:

	/**
	* Constructor.
	*/
	CybGaussian(int variablesNumber, float prioriProbability);

	/**
	* Destructor.
	*/
	~CybGaussian();

	/**
	* This method returns the mean of one determined variable.
	*
	* @param node_id id of the variable from which the mean is required.
	* @return float Mean of one determined variable.
	*/
	float getMean(int node_id);

	/**
	* This method returns a array with all the means.
	*
	* @param void
	* @return vector<float>& Array with all the means.
	*/
	vector<float>& getMean();

	/**
	* This method sets the mean of a determined variable.
	*
	* @param int node_id id of the variable to set the mean.
	* @param float newMean value of the mean to be set.
	* @return void.
	*/
	void setMean(float newMean, int node_id);

	/**
	* This method sets the array with all the means.
	*
	* @param vector<float> newMean array with values for all the means.
	* @return void.
	*/
	void setMean(vector<float>& newMean);

	/**
	* This method returns the variance of one determined variable.
	*
	* @param node_id id of the variable from which the variance is required.
	* @return float Variance of one determined variable.
	*/
	float getVariance(int node_id);

	/**
	* This method returns a array with all the variances.
	*
	* @param void
	* @return vector<float>& Array with all the variances.
	*/
	vector<float> getVariance();

	/**
	* This method returns the covariance of one determined variable.
	*
	* @param node_id id of the variable from which the covariance is required.
	* @return vector<float> Covariance of one determined variable.
	*/
	vector<float> getCovariance(int node_id);

	/**
	* This method returns an array with all the covariances.
	*
	* @param void
	* @return CybMatrix<float>& Array with all the covariances.
	*/
	CybMatrix<float>& getCovariance();

	/**
	* This method sets the covariance of a determined variable.
	*
	* @param int node_id id of the variable to set the covariance.
	* @param vector<float>& newCovariance value of the covariance to be set.
	* @return void.
	*/
	void setCovariance(vector<float>& newCovariance, int node_id);

	/**
	* This method sets the array with all the covariances.
	*
	* @param CybMatrix<float>& newCovariance value of the covariance to be set.
	* @return void.
	*/
	void setCovariance(CybMatrix<float>& newCovariance);

	/**
	* This method returns the value of the priori probability.
	*
	* @param void
	* @return float Value of the priori probability.
	*/
	float getPrioriProbability();

	/**
	* This method implements the training
	* for this assess method.
	*
	* @param void.
	* @return void.
	*/
	virtual void training();

	/**
	* This method will implement the assessment
	* for a vector of data.
	*
	* @param CybVectorND<float>& The classified data.
	* @return void.
	*/
	virtual double assessment(CybVectorND<float>& data);

	/**
	* This method starts some of the attributes.
	*
	* @param void.
	* @return void.
	*/
	void checkVariables();

protected:
	vector<float> mean; //stores the means of the variables
	CybMatrix<float> covariance; //stores the matrix of covariance

	/**
	* This method calculates all the means and stores
	* them on the attribute 'mean'.
	*
	* @param void.
	* @return void.
	*/
	void calculateMean();

	/**
	* This method calculates all the covariances and
	* stores them on the attribute 'covariance'.
	*
	* @param void.
	* @return void.
	*/
	void calculateCovariance();

private:
	CybMatrix<float> precisionMatrix; //stores the precision matrix
	vector<int> variables; //stores the variables
	float prioriProbability; //stores the priori probability
	float determinant; //stores the determinante of the precision matrix

	/**
	* This method returns the result of the function.
	*
	* @param vector<float>& data: data used in the function.
	* @param vector<int>& variables: list with the variables.
	* @return double the result of the function.
	*/
	double getFunctionResult(CybVectorND<float>& data, vector<int>& variables);
};

