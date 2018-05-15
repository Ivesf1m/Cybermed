#pragma once

/**
* This class defines the basic draw operator for box drawers and must be extended when implementing
* one of them.
*/

class CybBoxDrawer
{
public:
	/*
	* Constructor
	*
	* @param void
	*/
	CybBoxDrawer() {}

	/*
	* Abstract draw method. Must be implemented in subclasses.
	*
	* @param void
	* @return void
	*/
	virtual void drawBox() = 0;
};