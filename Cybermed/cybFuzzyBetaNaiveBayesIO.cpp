#include "cybFuzzyBetaNaiveBayesIO.h"

using std::flush;

CybFuzzyBetaNaiveBayesIO::CybFuzzyBetaNaiveBayesIO(string& fileName)
	: CybAssessIO(fileName, ".cyba_FBetNB")
{

}


CybFuzzyBetaNaiveBayesIO::~CybFuzzyBetaNaiveBayesIO()
{

}

void CybFuzzyBetaNaiveBayesIO::write(void* fbnbwork)
{
	CybFuzzyBetaNaiveBayes* fbnb = (CybFuzzyBetaNaiveBayes*)fbnbwork;

	ofstream fout(this->getFile(), ofstream::out);

	if (fout.fail()) {
		cerr << "Problem when opening output file " << this->getFile() << endl;
		cerr << flush;
		exit(1);
	}

	fout << "##" << fbnb->getNumberOfVariables() << endl;
	fout << "##" << fbnb->getNIntervals() << endl;
	fout << "\nPertinences\n";
	fout << "**" << endl;

	for (int i = 0; i < fbnb->getNumberOfVariables(); ++i) {
		for (int j = 0; j < fbnb->getNIntervals(); ++j) {
			fout << "[" << (*fbnb->getPertinences())[j][i].first.first <<
				", " << (*fbnb->getPertinences())[j][i].first.second << "]:" <<
				(*fbnb->getPertinences())[j][i].second << endl;
		}
		fout << "**" << endl;
	}

	fout << "\nMean\n";
	fout << "**" << endl;

	vector<float>& auxMean = fbnb->getAlpha();

	for (int i = 0; i < fbnb->getNumberOfVariables(); ++i)
		fout << auxMean[i] << endl;

	fout << "\nStandard Deviation\n";
	fout << "##" << endl;

	vector<float>& auxStdDev = fbnb->getBeta();
	
	for (int i = 0; i < fbnb->getNumberOfVariables(); ++i)
		fout << auxStdDev[i] << endl;

	fout << "**" << endl;

	fout.close();
}

void* CybFuzzyBetaNaiveBayesIO::read()
{
	ifstream fin(this->getFile());

	if (fin.fail()) {
		cerr << "Problems when opening input file " << this->getFile() << endl;
		exit(1);
	}

	char c;
	int variables, nIntervals;

	fin >> c >> c >> variables >> c >> c >> nIntervals;

	CybFuzzyBetaNaiveBayes* fbnb = new CybFuzzyBetaNaiveBayes(variables,
		nIntervals);

	while (c != '[')
		fin >> c;

	CybMatrix<pair<pair<double, double>, double>>* auxPert = new
		CybMatrix<pair<pair<double, double>, double>>(nIntervals, variables);

	for (int i = 0; i < variables; ++i) {
		for (int j = 0; j < nIntervals; ++j) {
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

	vector<float> auxMean(variables);

	for (int i = 0; i < variables; ++i)
		fin >> auxMean[i];

	fbnb->setAlpha(auxMean);

	while (c != '#')
		fin >> c;
	fin >> c;

	vector<float> auxStdDev(variables);

	for (int i = 0; i < variables; ++i)
		fin >> auxStdDev[i];

	fbnb->setBeta(auxStdDev);

	fin.close();

	return (void*)fbnb;
}
