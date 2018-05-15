#pragma once

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

template<class _TYPE>
class CybVectorND
{
public:
	CybVectorND(int dimension)
		: dim(dimension), elements(dimension)
	{

	}

	~CybVectorND()
	{

	}

	int getDimension()
	{
		return dim;
	}

	_TYPE operator^(CybVectorND<_TYPE>& v)
	{
		_TYPE temp = 0;
		for (int i = 0; i < dim; ++i)
			temp += elements[i] * v[i];
		return temp;
	}

	CybVectorND<_TYPE>& operator=(CybVectorND<_TYPE>& v)
	{
		dim = v.dim;
		elements.resize(dim);
		for (int i = 0; i < dim; ++i)
			elements[i] = v[i];
		return *this;
	}

	bool operator==(CybVectorND<_TYPE>& v)
	{
		for (int i = 0; i < dim; ++i)
			if (elements[i] != v[i])
				return false;
		return true;
	}

	bool operator!=(CybVectorND<_TYPE>& v)
	{
		return !((*this) == v);
	}

	CybVectorND<_TYPE> operator+(CybVectorND<_TYPE>& v)
	{
		CybVectorND<_TYPE> temp(dim);
		for (int i = 0; i < dim; ++i)
			temp[i] = elements[i] + v[i];
		return temp;
	}

	CybVectorND<_TYPE> operator+=(CybVectorND<_TYPE>& v)
	{
		return ((*this) = (*this) + v);
	}

	CybVectorND<_TYPE> operator-(CybVectorND<_TYPE> v)
	{
		CybVectorND<_TYPE> temp(dim);
		for (int i = 0; i < dim; ++i)
			temp[i] = elements[i] - v[i];
		return temp;
	}

	CybVectorND<_TYPE> operator-=(CybVectorND<_TYPE>& v)
	{
		return ((*this) = (*this) - v);
	}

	CybVectorND<_TYPE> operator*(_TYPE k)
	{
		CybVectorND<_TYPE> temp(dim);
		for (int i = 0; i < dim; ++i)
			temp[i] = elements[i] * k;
		return temp;
	}

	friend CybVectorND<_TYPE> operator*(_TYPE k, CybVectorND<_TYPE>& v)
	{
		return v * k;
	}

	CybVectorND<_TYPE> operator*=(_TYPE k)
	{
		return ((*this) = (*this) * k);
	}

	CybVectorND<_TYPE> operator/(_TYPE k)
	{
		CybVectorND<_TYPE> temp(dim);
		for (int i = 0; i < dim; ++i)
			temp[i] = elements[i] / k;
		return temp;
	}

	CybVectorND<_TYPE> operator/=(_TYPE k)
	{
		return ((*this) = (*this) / k);
	}

	_TYPE& operator[](int index)
	{
		return elements[index];
	}

	_TYPE* toCArray()
	{
		return elements.data();
	}

	void show()
	{
		for (int i = 0; i < dim; ++i)
			cout << elements[i] << "\t";
		cout << endl;
	}

private:
	int dim;
	vector<_TYPE> elements;
};
