#include "cybBoxHolder.h"

CybBoxHolder CybBoxHolder::instance = CybBoxHolder();

CybBoxHolder::CybBoxHolder()
{

}


CybBoxHolder::~CybBoxHolder()
{

}

void CybBoxHolder::addBox(shared_ptr<CybBoxDrawer> drawer)
{
	drawerList.push_back(drawer);
}

void CybBoxHolder::drawBoxes()
{
	for (auto& drawer : drawerList)
		drawer->drawBox();
}

shared_ptr<CybBoxHolder> CybBoxHolder::getInstance()
{
	return std::make_shared<CybBoxHolder>(instance);
}

void CybBoxHolder::removeBox(unsigned int index)
{
	if (index >= 0 && index < drawerList.size()) {
		list< shared_ptr<CybBoxDrawer> >::iterator it = drawerList.begin();
		advance(it, index);
		drawerList.erase(it);
	}
	return;
}
