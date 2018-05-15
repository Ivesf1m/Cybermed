#include "cybMLMIO.h"

using std::flush;

CybMLMIO::CybMLMIO(string& fileName)
	: CybAssessIO(fileName, ".cyba_MLM")
{

}


CybMLMIO::~CybMLMIO()
{

}

void CybMLMIO::write(void* mlmObject)
{
	CybMLM* mlm = (CybMLM*)mlmObject;

	ofstream fout(this->getFile());

	if (fout.fail())
	{
		cerr << "Problems when opening file " << this->getFile() << endl;
		cerr << flush;
		exit(1);
	}

	fout << "##" << mlm->getNumberOfVariables();

	fout << "\n\nPrior Probability [ " << mlm->getPrioriProbability() << " ]";

	fout << "\n\nMean" << "\n[\n";


	for (int i = 0; i < mlm->getNumberOfVariables(); i++)
	{
		fout << mlm->getMean(i);

		if (i < mlm->getNumberOfVariables() - 1)
			fout << ",\n";
	}
	fout << "\n]\n\n";

	fout << "Precision Matrix\n[\n";

	for (int i = 0; i < mlm->getNumberOfVariables(); i++)
	{
		for (int j = 0; j < mlm->getNumberOfVariables(); j++)
			fout << mlm->getCovariance()[i][j] << " ";
		fout << "\n";
	}
	fout << "]";
	fout.close();
}

void* CybMLMIO::read()
{
	ifstream fin(this->getFile());

	if (fin.fail())
	{
		cerr << "Problems when opening file " << this->getFile() << endl;
		exit(1);
	}

	char c = 'a';
	int number = 0;
	float mean;
	int count = 0;
	float prob = 1.0;

	fin >> c;
	fin >> c;
	fin >> number;


	while (c != '[')
		fin >> c;

	fin >> prob;

	fin >> c;

	CybMLM* mlm = new CybMLM(number, prob);

	while (c != '[')
		fin >> c;

	while (c != ']')
	{
		fin >> mean;
		fin >> c;

		mlm->setMean(mean, count++);
	}

	while (c != '[')
		fin >> c;

	CybMatrix<float> matrix(mlm->getNumberOfVariables(),
		mlm->getNumberOfVariables());

	for (int i = 0; i < mlm->getNumberOfVariables(); i++)
		for(int j = 0; j < mlm->getNumberOfVariables(); ++j)
			fin >> matrix[i][j];

	mlm->setCovariance(matrix);

	mlm->checkVariables();

	fin.close();

	return (void*)mlm;
}
