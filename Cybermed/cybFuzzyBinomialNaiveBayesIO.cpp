#include "cybFuzzyBinomialNaiveBayesIO.h"

using std::flush;

CybFuzzyBinomialNaiveBayesIO::CybFuzzyBinomialNaiveBayesIO(string& fileName)
	: CybAssessIO(fileName, ".cyba_FBNB")
{

}


CybFuzzyBinomialNaiveBayesIO::~CybFuzzyBinomialNaiveBayesIO()
{

}

void CybFuzzyBinomialNaiveBayesIO::write(void* fbnbwork)
{
	CybFuzzyBinomialNaiveBayes* fbnb = (CybFuzzyBinomialNaiveBayes*)fbnbwork;

	ofstream fout(this->getFile(), ofstream::out);

	if (fout.fail())
	{
		cerr << "Problems when opening output file " << this->getFile() << endl;
		cerr << flush;
		exit(1);
	}

	fout << "##" << fbnb->getNumberOfVariables() << endl;

	fout << "##" << fbnb->getNIntervals() << endl;

	fout << "\nPertinences\n";
	fout << "**" << endl;

	for (int i = 0; i < fbnb->getNumberOfVariables(); i++)
	{
		for (int j = 0; j < fbnb->getNIntervals(); j++)
		{
			fout << "[" << (*fbnb->getPertinences())[j][i].first.first << ", " <<
				(*fbnb->getPertinences())[j][i].first.second << "]:" <<
				(*fbnb->getPertinences())[j][i].second << endl;
		}
		fout << "**" << endl;
	}

	fout << "\nProbability\n";
	fout << "**" << endl;

	vector<float> auxProb = fbnb->getProbability();

	for (int i = 0; i < fbnb->getNumberOfVariables(); i++)
	{
		fout << auxProb[i] << endl;
	}

	fout << "**" << endl;

	fout << "\nN\n";
	fout << "##" << endl;

	vector<int> auxN = fbnb->getN();

	for (int i = 0; i < fbnb->getNumberOfVariables(); i++)
	{
		fout << auxN[i] << endl;
	}

	fout << "**" << endl;

	fout.close();
}

void* CybFuzzyBinomialNaiveBayesIO::read()
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

	CybFuzzyBinomialNaiveBayes* fbnb = new CybFuzzyBinomialNaiveBayes(variables, nIntervals);

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

	fbnb->setPertinences(auxPert);

	while (c != '*')
		fin >> c;
	fin >> c;

	vector<float> auxProb(variables);

	for (int i = 0; i < variables; i++)
	{
		fin >> auxProb[i];
	}

	fbnb->setProbability(auxProb);

	while (c != '#')
		fin >> c;
	fin >> c;

	vector<int> auxN(variables);

	for (int i = 0; i < variables; i++)
	{
		fin >> auxN[i];
	}

	fbnb->setN(auxN);

	fin.close();

	return (void*)fbnb;
}
