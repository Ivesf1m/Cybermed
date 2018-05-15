#include "cybNetworkIO.h"

using std::cerr;
using std::flush;

CybNetworkIO::CybNetworkIO(string& fileName)
	: CybAssessIO(fileName, ".cyba_BN")
{

}


CybNetworkIO::~CybNetworkIO()
{

}

void CybNetworkIO::write(void* network)
{
	CybBayesianNetwork* net = (CybBayesianNetwork*)network;

	ofstream fout(this->getFile());

	if (fout.fail())
	{
		cerr << "Problems when opening file " << this->getFile() << endl;
		cerr << flush;
		exit(1);
	}

	fout << "##" << net->getNodesNumber();

	fout << "\n\nMean" << "\n[\n";


	for (int i = 0; i < net->getNodesNumber(); i++)
	{
		fout << net->getMean(i);

		if (i < net->getNodesNumber() - 1)
			fout << ",\n";
	}
	fout << "\n]\n\n";

	fout << "Dependency\n";
	fout << "[\n";
	cout << flush;
	vector<int> mix;

	for (int i = 0; i < net->getNodesNumber(); i++)
	{
		mix = net->getParents(i);

		fout << i;

		if (mix.size() == 0)
		{
			if (i < net->getNodesNumber() - 1)
				fout << ";";
			fout << "\n";
			continue;
		}
		fout << ": ";

		for (unsigned int j = 0; j < mix.size(); j++)
		{
			fout << mix[j];
			if (j < mix.size() - 1)
				fout << ", ";
			else if (i < net->getNodesNumber() - 1)
				fout << ";";
		}

		fout << "\n";
	}
	fout << "]\n\n";

	fout << "Covariance\n[\n";

	for (int i = 0; i < net->getNodesNumber(); i++)
	{
		for (int j = 0; j < net->getNodesNumber(); j++)
			fout << net->getCovariance()[i* net->getNodesNumber() + j] << " ";
		fout << "\n";
	}
	fout << "]";
	fout.close();
}

void* CybNetworkIO::read()
{
	ifstream fin(this->getFile());

	if (fin.fail())
	{
		cerr << "Problems when opening input file " << this->getFile() << endl;
		exit(1);
	}

	char c = 'a';
	int number = 0;
	float mean;
	int id = 0;
	int parent;
	int count = 0;

	fin >> c;
	fin >> c;
	fin >> number;

	CybBayesianNetwork* net = new CybBayesianNetwork(number);

	while (c != '[')
		fin >> c;

	while (c != ']')
	{
		fin >> mean;
		fin >> c;

		net->setMean(mean, count++);
	}

	while (c != '[')
		fin >> c;

	while (true)
	{
		if (c == ']')
		{
			break;
		}

		fin >> id;
		fin >> c;

		while (c != ';' && c != ']')
		{
			fin >> parent;
			net->addArc(parent, id);
			fin >> c;
		}
	}

	while (c != '[')
		fin >> c;

	vector<float> matrix(net->getNodesNumber() * net->getNodesNumber());

	for (int i = 0; i < net->getNodesNumber() * net->getNodesNumber(); i++)
		fin >> matrix[i];

	net->setCovariance(matrix);

	net->initModel();

	fin.close();

	return (void*)net;
}
