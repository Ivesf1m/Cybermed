#pragma once

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include "cybAssess.h"

using std::ifstream;
using std::ofstream;
using std::string;

class CybAssessIO
{
public:

	/**
	* Constructor.
	*
	* @param string fileName: the name of the file.
	* @param string sufix: The suffix for this file name.
	*/
	CybAssessIO(string fileName, string suffix);

	/**
	* Destructor.
	*/
	~CybAssessIO();

	/**
	* Pure virtual method. It will implement
	* the code to read the assess model.
	*
	* @param void.
	* @return void* A reference for an assess object.
	*/
	virtual void* read() = 0;

	/**
	* Pure virtual method. It will implement
	* the code to save the assess model.
	*
	* @param void* A reference to the assess object.
	* @return void.
	*/
	virtual void write(void*) = 0;

	/**
	* This method returns the file name.
	*
	* @param void.
	* @return string&: The file name.
	*/
	string& getFile();

	/**
	* This method verifies if the suffix is valid for
	* this assess type and puts a suffix if there isn't one.
	*
	* @param void.
	* @return void.
	*/
	void validFile();

	/**
	* This method returns the name of the data file.
	*
	* @param void.
	* @return string& The data file name.
	*/
	string& getDataFile();

	/**
	* This method writes in a file the assess data.
	*
	* @param CybAssess* assess A reference for assess object.
	* @return void.
	*/
	void writeData(CybAssess& assess);

	/**
	* This method returns the data saved in a file.
	*
	* @param void.
	* @return vector<CybVectorND<float>>& A list for the complete data.
	*/
	void readData(vector<CybVectorND<float>>& list);

private:
	string fileName; //Stores the file name for the assess method.
	string suffix;   //Stores the sufix for the assess method associated.
	string dataFile; //Stores the name for the data file.
};

