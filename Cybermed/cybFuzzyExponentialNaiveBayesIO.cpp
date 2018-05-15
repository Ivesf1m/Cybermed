#include "cybFuzzyExponentialNaiveBayesIO.h"

using std::flush;

CybFuzzyExponentialNaiveBayesIO::CybFuzzyExponentialNaiveBayesIO(string& fileName)
	: CybAssessIO(fileName, ".cyba_FENB")
{

}


CybFuzzyExponentialNaiveBayesIO::~CybFuzzyExponentialNaiveBayesIO()
{

}

void CybFuzzyExponentialNaiveBayesIO::write(void* fenbwork)
{
	CybFuzzyExponentialNaiveBayes* fenb = (CybFuzzyExponentialNaiveBayes*)fenbwork;

	ofstream fout(this->getFile(), ofstream::out);

	if (fout.fail())
	{
		cerr << "Problems when opening output file " << this->getFile() << endl;
		cerr << flush;
		exit(1);
	}

	fout << "##" << fenb->getNumberOfVariables() << endl;

	fout << "##" << fenb->getNIntervals() << endl;

	fout << "\nPertinences\n";
	fout << "**" << endl;

	for (int i = 0; i < fenb->getNumberOfVariables(); i++)
	{
		for (int j = 0; j < fenb->getNIntervals(); j++)
		{
			fout << "[" << (*fenb->getPertinences())[j][i].first.first << ", " <<
				(*fenb->getPertinences())[j][i].first.second << "]:" <<
				(*fenb->getPertinences())[j][i].second << endl;
		}
		fout << "**" << endl;
	}

	fout << "\nParameters\n";
	fout << "**" << endl;

	vector<float> auxPar = fenb->getParameters();

	for (int i = 0; i < fenb->getNumberOfVariables(); i++)
	{
		fout << auxPar[i] << endl;
	}

	fout << "**" << endl;

	fout.close();
}

void* CybFuzzyExponentialNaiveBayesIO::read()
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

	CybFuzzyExponentialNaiveBayes* fenb = new CybFuzzyExponentialNaiveBayes(variables, nIntervals);

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

	fenb->setPertinences(auxPert);

	while (c != '*')
		fin >> c;
	fin >> c;

	vector<float> auxPar(variables);

	for (int i = 0; i < variables; i++)
	{
		float a = 0;
		fin >> auxPar[i];
	}

	fenb->setParameters(auxPar);

	fin.close();

	return (void*)fenb;
}
