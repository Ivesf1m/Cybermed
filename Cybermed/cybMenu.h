#pragma once

#include "cybMenuNode.h"
#include <list>

using std::list;

class CybMenu
{
public:
	~CybMenu();

	/**
	* This method adds an option to this menu.
	* @param string& name: the name associated to the option.
	* @param int event: the event associated.
	* @return void
	*/
	void add(string& name, int event);

	/**
	* This method adds a submenu to this menu.
	* @param string& name: the name associated to submenu.
	* @param CybMenu& submenu: the submenu associated.
	* @return void
	*/
	void add(string& name, CybMenu& submenu);

	/**
	* This method calls the menu initialization of the menu structure.
	* @param void.
	* @return void.
	*/
	void createMenu();

	/**
	* This method creates a menu structure.
	* @param CybMenu& submenu: the super menu of struct.
	*/
	static void createSubMenu(CybMenu& submenu);

	/**
	* This method returns the number associated with the event of menu.
	* If the name is not found, it returns -1 or the index of array
	* (the index associated to this event).
	*
	* @param string& name: the name associated with an item of menu.
	* @return int: the number associated to this event.
	*/
	int getEvent(string& name);

	/**
	* This method returns a name associated to this menu that is in index position.
	* @param int index: the position of the name.
	* @param string&: the name associated.
	*/
	string getName(unsigned int index);

	/**
	* This method returns a new instance of CybMenu class
	*
	* @param (void (*mainFunction)(int)): the function associated with the 
	* menu created.
	* @return CybMenu*: the CybMenu Reference.
	*/
	static CybMenu* getNewMenu(void(*menuFunction)(int));

	/**
	* This method sets the button of mouse associated with this menu.
	*
	* @param char button: the first caracter of button: 'r' or 'R' if the button
	* is right button, 'l' or 'L' if left button and 'm' or 'M' if middle button
	*/
	void setButton(char button);

protected:
	CybMenu(void(*menuFunction)(int));

private:
	list<CybMenuNode> elements;
	list<CybMenu> menu;
	int id;
	void(*menuFunction)(int);
	int size;
	int button;
};

