#include "cybGammaNaiveBayesIO.h"

using std::flush;

CybGammaNaiveBayesIO::CybGammaNaiveBayesIO(string& fileName)
	: CybAssessIO(fileName, ".cyba_FGamNB")
{

}

CybGammaNaiveBayesIO::~CybGammaNaiveBayesIO()
{

}

void CybGammaNaiveBayesIO::write(void* fgnbwork)
{
	CybGammaNaiveBayes* fgnb = (CybGammaNaiveBayes*)fgnbwork;

	ofstream fout(this->getFile(), ofstream::out);

	if (fout.fail())
	{
		cerr << "Problems when opening output file " << this->getFile() << endl;
		cerr << flush;
		exit(1);
	}

	fout << "##" << fgnb->getNumberOfVariables() << endl;

	fout << "\nMean\n";
	fout << "**" << endl;

	vector<float> auxMean = fgnb->getAlpha();

	for (int i = 0; i < fgnb->getNumberOfVariables(); i++)
	{
		fout << auxMean[i] << endl;
	}

	fout << "\nStandard Deviation\n";
	fout << "##" << endl;

	vector<float> auxStdDev = fgnb->getBeta();

	for (int i = 0; i < fgnb->getNumberOfVariables(); i++)
	{
		fout << auxStdDev[i] << endl;
	}

	fout << "**" << endl;


	fout.close();
}

void* CybGammaNaiveBayesIO::read()
{
	ifstream fin(this->getFile());

	if (fin.fail())
	{
		cerr << "Problems when opening input file " << this->getFile() << endl;
		exit(1);
	}

	char c;
	int variables;

	fin >> c >> c >> variables;

	CybGammaNaiveBayes* fgnb = new CybGammaNaiveBayes(variables);

	while (c != '*')
		fin >> c;
	fin >> c;

	vector<float> auxMean(variables);

	for (int i = 0; i < variables; i++)
	{
		fin >> auxMean[i];
	}

	fgnb->setAlpha(auxMean);

	while (c != '#')
		fin >> c;
	fin >> c;

	vector<float> auxStdDev(variables);

	for (int i = 0; i < variables; i++)
	{
		fin >> auxStdDev[i];
	}

	fgnb->setBeta(auxStdDev);

	fin.close();

	return (void*)fgnb;
}
