#include "cybFuzzyGammaNaiveBayesIO.h"

using std::flush;

CybFuzzyGammaNaiveBayesIO::CybFuzzyGammaNaiveBayesIO(string& fileName)
	: CybAssessIO(fileName, ".cyba_FGamNB")
{

}

CybFuzzyGammaNaiveBayesIO::~CybFuzzyGammaNaiveBayesIO()
{

}

void CybFuzzyGammaNaiveBayesIO::write(void* fgnbwork)
{
	CybFuzzyGammaNaiveBayes* fgnb = (CybFuzzyGammaNaiveBayes*)fgnbwork;

	ofstream fout(this->getFile(), ofstream::out);

	if (fout.fail())
	{
		cerr << "Problems whem opening output file " << this->getFile() << endl;
		cerr << flush;
		exit(1);
	}

	fout << "##" << fgnb->getNumberOfVariables() << endl;

	fout << "##" << fgnb->getNIntervals() << endl;

	fout << "\nPertinences\n";
	fout << "**" << endl;

	for (int i = 0; i < fgnb->getNumberOfVariables(); i++)
	{
		for (int j = 0; j < fgnb->getNIntervals(); j++)
		{
			fout << "[" << (*fgnb->getPertinences())[j][i].first.first << ", " <<
				(*fgnb->getPertinences())[j][i].first.second << "]:" <<
				(*fgnb->getPertinences())[j][i].second << endl;
		}
		fout << "**" << endl;
	}

	fout << "\nAlpha\n";
	fout << "**" << endl;

	vector<float> auxMean = fgnb->getAlpha();

	for (int i = 0; i < fgnb->getNumberOfVariables(); i++)
	{
		fout << auxMean[i] << endl;
	}

	fout << "\nBeta\n";
	fout << "##" << endl;

	vector<float> auxStdDev = fgnb->getBeta();

	for (int i = 0; i < fgnb->getNumberOfVariables(); i++)
	{
		fout << auxStdDev[i] << endl;
	}

	fout << "**" << endl;


	fout.close();
}

void* CybFuzzyGammaNaiveBayesIO::read()
{
	ifstream fin(this->getFile());

	if (fin.fail())
	{
		cerr << "Problems when opening input file " << this->getFile() << endl;
		exit(1);
	}

	char c;
	int variables, nIntervals;

	fin >> c >> c >> variables >> c >> c >> nIntervals;

	CybFuzzyGammaNaiveBayes* fgnb = new CybFuzzyGammaNaiveBayes(variables, nIntervals);

	while (c != '[')
		fin >> c;

	CybMatrix < pair< pair<double, double>, double> > *auxPert = new CybMatrix < pair< pair<double, double>, double> >(nIntervals, variables);

	for (int i = 0; i < variables; i++)
	{
		for (int j = 0; j < nIntervals; j++)
		{
			double a = 0, b = 0, d = 0;
			fin >> a >> c >> b >> c >> c >> d >> c;

			(*auxPert)[j][i].first.first = a;
			(*auxPert)[j][i].first.second = b;
			(*auxPert)[j][i].second = d;
		}
		fin >> c >> c;
	}

	fgnb->setPertinences(auxPert);

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
