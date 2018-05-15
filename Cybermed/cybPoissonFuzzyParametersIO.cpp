#include "cybPoissonFuzzyParametersIO.h"

using std::flush;

CybPoissonFuzzyParametersIO::CybPoissonFuzzyParametersIO(string& fileName)
	: CybAssessIO(fileName, ".cyba_PNBFP")
{

}

CybPoissonFuzzyParametersIO::~CybPoissonFuzzyParametersIO()
{

}

void CybPoissonFuzzyParametersIO::write(void* pfpwork)
{
	CybPoissonFuzzyParameters* pfp = (CybPoissonFuzzyParameters*)pfpwork;

	ofstream fout(this->getFile());

	if (fout.fail())
	{
		cerr << "Problems when opening output file " << this->getFile() << endl;
		cerr << flush;
		exit(1);
	}

	fout << "##" << pfp->getNumberOfVariables() << endl;

	fout << "##" << pfp->getAlphaCuts() << endl;

	fout << "\nParameters\n";
	fout << "**" << endl;

	for (int i = 0; i < pfp->getNumberOfVariables(); i++)
	{
		for (int j = 0; j < pfp->getAlphaCuts(); j++)
		{
			fout << "[" << pfp->getFuzzyParameters()[j][i].first << ", " << pfp->getFuzzyParameters()[j][i].second << "]" << endl;
		}
		fout << "**" << endl;
	}

	fout.close();
}

void* CybPoissonFuzzyParametersIO::read()
{
	ifstream fin(this->getFile());

	if (fin.fail())
	{
		cerr << "Problems whem opening input file " << this->getFile() << endl;
		exit(1);
	}

	char c;
	int variables;
	int cuts;

	fin >> c >> c >> variables >> c >> c >> cuts;

	CybPoissonFuzzyParameters* pfp = new CybPoissonFuzzyParameters(variables, cuts);

	while (c != '[')
		fin >> c;

	CybMatrix< pair<double, double> > auxPar(cuts + 1, variables);

	for (int i = 0; i < variables; i++)
	{
		for (int j = 0; j < cuts; j++)
		{
			double a = 0, b = 0;
			fin >> a >> c >> b >> c >> c;

			auxPar[j][i].first = a;
			auxPar[j][i].second = b;
		}
		fin >> c >> c;
	}

	pfp->setFuzzyParameters(auxPar);

	fin.close();

	return (void*)pfp;
}
