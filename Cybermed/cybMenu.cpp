#include "cybMenu.h"
#include <gl/freeglut.h>


CybMenu::CybMenu(void (*menuFunction)(int))
	: menuFunction(menuFunction), button('r'), size(0)
{

}


CybMenu::~CybMenu()
{

}

void CybMenu::add(string& name, int event)
{
	elements.push_back(CybMenuNode(name, event));
	menu.push_back(CybMenu(NULL));
	size++;
}

void CybMenu::add(string& name, CybMenu& submenu)
{
	elements.push_back(CybMenuNode(name, size));
	menu.push_back(submenu);
	size++;
}

void CybMenu::createMenu()
{
	CybMenu::createSubMenu(*this);
	glutAttachMenu(button);
}

void CybMenu::createSubMenu(CybMenu& submenu)
{
	for (auto& m : submenu.menu)
		createSubMenu(m);
	submenu.id = glutCreateMenu(submenu.menuFunction);
	list<CybMenuNode>::iterator it1 = submenu.elements.begin();
	list<CybMenu>::iterator it2 = submenu.menu.begin();
	for (; it1 != submenu.elements.end(); ++it1)
	{
		if (it2->menuFunction)
			glutAddSubMenu(it1->name.c_str(), it2->id);
		else
			glutAddMenuEntry(it1->name.c_str(), it1->event);

		++it2;
	}
}

int CybMenu::getEvent(string& name)
{
	for (auto& e : elements)
		if (e.name.compare(name) == 0)
			return e.event;
	return -1;
}

string CybMenu::getName(unsigned int index)
{
	int i = 0;
	for (auto& e : elements) {
		if (i == index)
			return e.name;
		++i;
	}
	return string("");
}

CybMenu* CybMenu::getNewMenu(void(*menuFunction)(int))
{
	return new CybMenu(menuFunction);
}

void CybMenu::setButton(char button)
{
	if (button == 'r' || button == 'R')
		this->button = GLUT_RIGHT_BUTTON;
	else if (button == 'l' || button == 'L')
		this->button = GLUT_LEFT_BUTTON;
	else if (button == 'm' || button == 'M')
		this->button = GLUT_MIDDLE_BUTTON;
}
