#include "cybAssessIO.h"

using std::cerr;
using std::cout;
using std::endl;
using std::flush;

CybAssessIO::CybAssessIO(string fileName, string suffix)
	: fileName(fileName), suffix(suffix)
{
	validFile();
}


CybAssessIO::~CybAssessIO()
{

}

string& CybAssessIO::getFile()
{
	return fileName;
}

string& CybAssessIO::getDataFile()
{
	return dataFile;
}

void CybAssessIO::validFile()
{
	string cybadata(".cybaDATA");
	fileName = fileName + cybadata + suffix;
}

void CybAssessIO::writeData(CybAssess& assess)
{
	ofstream fout(dataFile);
	cout << "Writing file " << fileName << endl;

	if (fout.fail()) {
		cerr << "Error when opening output file " << fileName << endl;
		cerr << flush;
		exit(0);
	}

	if (assess.getData().size() == 0)
		return;

	int dimension = assess.getData()[0].getDimension();
	fout << "##" << dimension << "\n\n";
	fout << "Data:\n\n";

	for (auto v : assess.getData()) {
		for (int j = 0; j < dimension; ++j)
			fout << v[j] << " ";
		fout << "\n";
	}

	fout.close();
}

void CybAssessIO::readData(vector<CybVectorND<float>>& list)
{
	ifstream fin(dataFile);
	cout << "Reading file " << fileName << endl;

	if (fin.fail()){
		cerr << "Error when opening input file " << fileName << endl;
		exit(0);
	}

	char c;
	int dimension;

	fin >> c;
	fin >> c;
	fin >> dimension;

	while (c != ':')
		fin >> c;

	while (!fin.eof()) {
		CybVectorND<float> v(dimension);
		for (int i = 0; i < dimension; ++i)
			fin >> v[i];

		if (fin.eof())
			fin.close();

		list.push_back(v);
	}
}
