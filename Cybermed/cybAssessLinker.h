#pragma once

#include "cybAssess.h"
#include "cybAssessIO.h"
#include "cybAssessDataProvider.h"
#include <thread>

using std::thread;

/**
* @class	CYB_ASSESS_MODE
* @file	cybAssessLinker.h
* @short	Define the assess states for the linker.
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	1.0
* @date	2008, August
*
*/
enum CYB_ASSESS_MODE
{
	DO_ASSESSMENT, /**> The object does the assessment. */
	DO_NOTHING, 	/**> The object does nothing.*/
	/**> The object obtains data from the associated data provider. */
	OBTAIN_DATA,
	/**> The object obtains data from the associated data provider and does
	* the assessment. */
	OBTAIN_DATA_AND_DO_ASSESSMENT, 
	SAVE_ASSESS,	/**> Saves the assess model. */
	SAVE_DATA,		/**> Saves the assess data. */
	/**> Does the model training (must be used after the object takes
	* the data). */
	TRAINING		
};

/**
* @class	CybAssessLinker
* @file	cybAssessLinker.h
* @short	The base class for all data linkers for the CyberMed System
*			(See CybAssessDataProvider).
*
* This class is the base for all specific assess simulator (Used only if
* you want to integrate with the CyberMed System).
*
* @author	LabTEVE (http://www.de.ufpb.br/~labteve), Federal University of Paraiba
* @version	1.0
* @date	2008, August
*
*/
class CybAssessLinker : public thread
{
public:

	/**
	* Constructor.
	*
	* @param CybAssess* assess The assess object associated.
	* @param CybAssessDataProvider* dataProvider The data provider associated.
	* @param CybAssessIO* io The io object associated.
	*/
	CybAssessLinker(CybAssess* assess, CybAssessDataProvider* dataProvider,
		CybAssessIO* io);

	/**
	* Destructor.
	*/
	~CybAssessLinker();

	/**
	* This method returns the assess object associated.
	*
	* @param void.
	* @return CybAssess* the assess object associated.
	*/
	CybAssess* getAssess();

	/**
	* Returns a reference to the result set.
	*
	* @param void.
	* @return vector<double>& A reference to the result list.
	*/
	vector<double>& getResultSet();

	/**
	* This method returns the io object associated.
	*
	* @param void.
	* @return CybAssessIO* A pointer to the io object associated.
	*/
	CybAssessIO* getIO();

	/**
	* This method returns a pointer to the data provider associated.
	*
	* @param void.
	* @return CybAssessDataProvider* A pointer to the data provider associated.
	*/
	CybAssessDataProvider* getDataProvider();

	/**
	* This method returns the current mode of execution.
	*
	* @param void.
	* @return CYB_ASSESS_MODE The current mode.
	*/
	CYB_ASSESS_MODE getCurrentMode();

	/**
	* This method sets the current mode.
	*
	* @param CYB_ASSESS_MODE mode the new current mode.
	* @return void.
	*/
	void setCurrentMode(CYB_ASSESS_MODE mode);

	/**
	* This method is called by the thread and implements the
	* assessment life cycle.
	*
	* @param void.
	* @return void.
	*/
	void run();

protected:
	CybAssess* assess; //the assess object associated
	CybAssessDataProvider* dataProvider; //the data provider object
	CybAssessIO* io; //the io object associated
	vector<double> resultSet;
	CYB_ASSESS_MODE currentMode; //the current mode of execution.
};

