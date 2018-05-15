#pragma once

#include "cybBoxDrawer.h"
#include <iostream>
#include <list>
#include <memory>

using std::list;
using std::shared_ptr;

/**
* This class holds references to all existent box drawers and calls their
* draw method.
*/

class CybBoxHolder
{
public:
	~CybBoxHolder();

	/*
	* This method adds a box drawer pointer to the list.
	*
	* @param CybBoxDrawer& drawer: a reference to the drawer to be added.
	* @return void
	*/
	void addBox(shared_ptr<CybBoxDrawer>);

	/*
	* This method iterates through the list of drawers and calls their draw method.
	*
	* @param void
	* @return void
	*/
	void drawBoxes();

	/*
	* This method is used to access the single instance of this class.
	*
	* @param void
	* @return CybBoxHolder*: the single instance of this class.
	*/
	static shared_ptr<CybBoxHolder> getInstance();
	/*
	* This method removes a box drawer pointer to the list.
	*
	* @param CybBoxDrawer& drawer: a reference to the drawer to be removed.
	* @return void
	*/
	void removeBox(unsigned int index);

private:
	list< shared_ptr<CybBoxDrawer> > drawerList;
	static CybBoxHolder instance;

	CybBoxHolder();
};

