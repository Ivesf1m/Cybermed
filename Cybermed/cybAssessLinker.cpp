#include "cybAssessLinker.h"



CybAssessLinker::CybAssessLinker(CybAssess* assess, CybAssessDataProvider*
	dataProvider, CybAssessIO* io)
	: thread(&CybAssessLinker::run, this), assess(assess),
	dataProvider(dataProvider), io(io)
{
}


CybAssessLinker::~CybAssessLinker()
{

}

CybAssess* CybAssessLinker::getAssess()
{
	return this->assess;
}

vector<double>& CybAssessLinker::getResultSet()
{
	return this->resultSet;
}

CybAssessIO* CybAssessLinker::getIO()
{
	return this->io;
}

CybAssessDataProvider* CybAssessLinker::getDataProvider()
{
	return this->dataProvider;
}

CYB_ASSESS_MODE CybAssessLinker::getCurrentMode()
{
	return this->currentMode;
}

void CybAssessLinker::setCurrentMode(CYB_ASSESS_MODE mode)
{
	this->currentMode = mode;
}

void CybAssessLinker::run()
{
	CybVectorND<float>* aux;

	switch (this->currentMode)
	{
	case DO_NOTHING: break;

	case OBTAIN_DATA: 
		this->assess->addData(*dataProvider->getData());
		dataProvider->getData()->show();
		cout << endl;
		break;

	case OBTAIN_DATA_AND_DO_ASSESSMENT:
		aux = dataProvider->getData();
		this->assess->addData(*aux);
		this->resultSet.push_back(this->assess->assessment(*aux));
		break;

	case TRAINING: this->assess->training();
		break;

	case DO_ASSESSMENT:
		this->resultSet.clear();
		for (unsigned int i = 0; i < this->assess->getData().size(); i++)
			this->resultSet.push_back(this->assess->assessment(
				this->assess->getData()[i]));
		break;

	case SAVE_ASSESS:
		this->io->write(this->assess);
		break;

	case SAVE_DATA: this->io->writeData(*(this->assess));
		break;
	}
}
