#include "cybFuzzyNaiveBayesIO.h"

using std::flush;

CybFuzzyNaiveBayesIO::CybFuzzyNaiveBayesIO(string& fileName)
	: CybAssessIO(fileName, ".cyba_FNB")
{

}

CybFuzzyNaiveBayesIO::~CybFuzzyNaiveBayesIO()
{

}
;
void CybFuzzyNaiveBayesIO::write(void* fnbwork)
{
	CybFuzzyNaiveBayes* fnb = (CybFuzzyNaiveBayes*)fnbwork;

	ofstream fout(this->getFile(), ofstream::out);

	if (fout.fail())
	{
		cerr << "Problems when opening output file " << this->getFile() << endl;
		cerr << flush;
		exit(1);
	}

	fout << "##" << fnb->getNumberOfVariables() << endl;

	fout << "##" << fnb->getNIntervals() << endl;

	fout << "\nPertinences\n";
	fout << "**" << endl;

	for (int i = 0; i < fnb->getNumberOfVariables(); i++)
	{
		for (int j = 0; j < fnb->getNIntervals(); j++)
		{
			fout << "[" << (*fnb->getPertinences())[j][i].first.first << ", " <<
				(*fnb->getPertinences())[j][i].first.second << "]:" <<
				(*fnb->getPertinences())[j][i].second << endl;
		}
		fout << "**" << endl;
	}

	fout << "\nProbability\n";
	fout << "**" << endl;

	for (int i = 0; i < fnb->getNumberOfVariables(); i++)
	{
		for (int j = 0; j < fnb->getNIntervals(); j++)
		{
			fout << "[" << (*fnb->getProbability())[j][i].first.first << ", " <<
				(*fnb->getProbability())[j][i].first.second << "]:" <<
				(*fnb->getProbability())[j][i].second << endl;
		}
		fout << "**" << endl;
	}

	fout.close();
}

void* CybFuzzyNaiveBayesIO::read()
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

	CybFuzzyNaiveBayes* fnb = new CybFuzzyNaiveBayes(variables, nIntervals);

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

	fnb->setPertinences(auxPert);

	while (c != '[')
		fin >> c;

	CybMatrix < pair< pair<double, double>, double> > *auxProb = new CybMatrix < pair< pair<double, double>, double> >(nIntervals, variables);

	for (int i = 0; i < variables; i++)
	{
		for (int j = 0; j < nIntervals; j++)
		{
			double a = 0, b = 0, d = 0;
			fin >> a >> c >> b >> c >> c >> d >> c;

			(*auxProb)[j][i].first.first = a;
			(*auxProb)[j][i].first.second = b;
			(*auxProb)[j][i].second = d;
		}
		fin >> c >> c;
	}

	fnb->setProbability(auxProb);

	fin.close();

	return (void*)fnb;
}
