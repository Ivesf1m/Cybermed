#include "cybGaussianIO.h"

using std::flush;

CybGaussianIO::CybGaussianIO(string& fileName)
	: CybAssessIO(fileName, ".cyba_Gaussian")
{

}


CybGaussianIO::~CybGaussianIO()
{

}

void CybGaussianIO::write(void* GaussianObject)
{
	CybGaussian* Gaussian = (CybGaussian*)GaussianObject;

	ofstream fout(this->getFile());

	if (fout.fail())
	{
		cerr << "Problems when opening file " << this->getFile() << endl;
		cerr << flush;
		exit(1);
	}

	fout << "##" << Gaussian->getNumberOfVariables();

	fout << "\n\nPrior Probability [ " << Gaussian->getPrioriProbability() << " ]";

	fout << "\n\nMean" << "\n[\n";


	for (int i = 0; i < Gaussian->getNumberOfVariables(); i++)
	{
		fout << Gaussian->getMean(i);

		if (i < Gaussian->getNumberOfVariables() - 1)
			fout << ",\n";
	}
	fout << "\n]\n\n";

	fout << "Precision Matrix\n[\n";

	for (int i = 0; i < Gaussian->getNumberOfVariables(); i++)
	{
		for (int j = 0; j < Gaussian->getNumberOfVariables(); j++)
			fout << Gaussian->getCovariance()[i][j] << " ";
		fout << "\n";
	}
	fout << "]";
	fout.close();
}

void* CybGaussianIO::read()
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

	CybGaussian* Gaussian = new CybGaussian(number, prob);

	while (c != '[')
		fin >> c;

	while (c != ']')
	{
		fin >> mean;
		fin >> c;

		Gaussian->setMean(mean, count++);
	}

	while (c != '[')
		fin >> c;

	CybMatrix<float> matrix(Gaussian->getNumberOfVariables(),
		Gaussian->getNumberOfVariables());

	for (int i = 0; i < Gaussian->getNumberOfVariables(); i++)
		for(int j = 0; j < Gaussian->getNumberOfVariables(); ++j)
			fin >> matrix[i][j];

	Gaussian->setCovariance(matrix);

	Gaussian->checkVariables();

	fin.close();

	return (void*)Gaussian;
}
