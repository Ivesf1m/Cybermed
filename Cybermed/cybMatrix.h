#pragma once

#include <iostream>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::vector;

/**
* This class stores a matrix-like object and implements access methods and
* basic operations.
*/


template<class type>
class CybMatrix
{
public:

	/*
	* Constructor 1.
	* This method constructs a matrix object with dimensions specified in the
	* parameters.
	*
	* @param int dim1: the first dimension of the matrix.
	* @param int dim2: the second dimension of the matrix.
	*/
	CybMatrix(int dim1, int dim2) {
		this->dim1 = dim1;
		this->dim2 = dim2;
		for (int i = 0; i < dim1; ++i) {
			matrix.push_back(vector<type>());
			matrix[i].resize(dim2);
		}
	}

	/*
	* Constructor 2.
	* This method constructs a matrix object with dimensions specified in the
	* parameters. The matrix is filled with the values passed as parameter.
	*
	* @param int dim1: the first dimension of the matrix.
	* @param int dim2: the second dimension of the matrix.
	* @param vector<type>& values: a vector containing the values that will be
	* put in the matrix. It must be of size dim1 * dim2.
	*/
	CybMatrix(int dim1, int dim2, vector<type>& values)
	{
		this->dim1 = dim1;
		this->dim2 = dim2;
		for (int i = 0; i < dim1; ++i) {
			matrix.push_back(vector<type>());
			for (int j = 0; j < dim2; ++j) {
				matrix[i].push_back(values[dim1 * i + j]);
			}
		}
	}

	/*
	* Constructor 3.
	* This method constructs a matrix object with dimensions specified in the
	* parameters. The matrix is filled with the value passed as parameter.
	*
	* @param int dim1: the first dimension of the matrix.
	* @param int dim2: the second dimension of the matrix.
	* @param type& value: the value that will be used to initialize
	* every element of the matrix.
	*/
	CybMatrix(int dim1, int dim2, type& value)
	{
		this->dim1 = dim1;
		this->dim2 = dim2;
		for (int i = 0; i < dim1; ++i) {
			matrix.push_back(vector<type>());
			for (int j = 0; j < dim2; ++j)
				matrix[i].push_back(value);
		}
	}

	/*
	* Destructor.
	*/
	~CybMatrix()
	{
		
	}

	/*
	* Overloaded assignment operator for CybMatrix objects.
	*
	* @param const CybMatrix<type>& m: the matrix that will be assigned to this
	* object.
	* @return CybMatrix<type>&: a reference to this object (with the new values).
	*/
	CybMatrix<type>& operator=(const CybMatrix<type>& m)
	{
		this->dim1 = m.dim1;
		this->dim2 = m.dim2;
		this->matrix = m.matrix;
		return *this;
	}

	/*
	* Overloaded addition operator for CybMatrix objects. Implements matrix addition.
	*
	* @param const CybMatrix<type>& m: the matrix that will be added to this object.
	* It must have the same dimensions this object has.
	* @return CybMatrix<type>: the result of the sum.
	*/
	CybMatrix<type> operator+(const CybMatrix<type>& m)
	{
		if ((this->dim1 != m.dim1) || (this->dim2 != m.dim2)) {
			cerr << "Invalid operation with operator +. Terminating." << endl;
			exit(1);
		}
		CybMatrix<type> v(this->dim1, this->dim2);
		for (int i = 0; i < this->dim1; ++i) {
			for (int j = 0; j < this->dim2; ++j) {
				v->matrix[i][j] = matrix[i][j] + m.matrix[i][j];
			}
		}
		return v;
	}
	
	/*
	* Overloaded subtraction operator for CybMatrix objects. Implements matrix subtraction.
	*
	* @param const CybMatrix<type>& m: the matrix that will be subtracted from this object.
	* It must have the same dimensions this object has.
	* @return CybMatrix<type>: the result of the subtraction.
	*/
	CybMatrix<type>& operator-(const CybMatrix<type>& m)
	{
		if ((this->dim1 != m.dim1) || (this->dim2 != m.dim2)) {
			cerr << "Invalid operation with operator -. Terminating." << endl;
			exit(1);
		}
		CybMatrix<type> v(this->dim1, this->dim2);
		for (int i = 0; i < this->dim1; ++i) {
			for (int j = 0; j < this->dim2; ++j) {
				v->matrix[i][j] = this->matrix[i][j] - m.matrix[i][j];
			}
		}
		return v;
	}

	/*
	* Overloaded multiplication operator for CybMatrix objects.
	* Implements matrix product.
	*
	* @param const CybMatrix<type>& m: the matrix that will be multiplicated
	* by this object. Its first dimension must be the same as this object's
	* second.
	* @return CybMatrix<type>&: a reference to the result of the multiplication
	*/
	CybMatrix<type>& operator*(const CybMatrix<type>& m)
	{
		if (this->dim2 != m.dim1) {
			cerr << "Invalid operation with operator *. Terminating." << endl;
			exit(1);
		}
		CybMatrix result(this->dim1, m.dim2);
		for (int i = 0; i < this->dim1; ++i) {
			for (int j = 0; j < m.dim2; ++j) {
				for (int k = 0; k < this->dim2; ++k) {
					result->matrix[i][j] += this->matrix[i][k] * m.matrix[k][j];
				}
			}
		}
		return result;
	}

	/*
	* Overloaded multiplication operator for CybMatrix objects.
	* Implements matrix times scalar operation.
	*
	* @param const double scalar: the scalar that will be multiplicated by this
	* object.
	* @return CybMatrix<type>: the result of the multiplication.
	*/
	CybMatrix<type>& operator*(const double scalar)
	{
		CybMatrix<type> v(this->dim1, this->dim2);
		for (int i = 0; i < this->dim1; ++i) {
			for (int j = 0; j < this->dim2; ++j) {
				v->matrix[i][j] = this->matrix[i][j] * scalar;
			}
		}
		return v;
	}

	/*
	* Overloaded division operator for CybMatrix objects.
	* Implements matrix divided by scalar operations.
	*
	* @param const double scalar: the scalar by which this object will be
	* divided.
	* @return CybMatrix<type>&: the result of the division.
	*/
	CybMatrix<type>& operator/(const double scalar)
	{
		CybMatrix<type> v(this->dim1, this->dim2);
		for (int i = 0; i < this->dim1; ++i) {
			for (int j = 0; j < this->dim2; ++j) {
				v->matrix[i][j] = this->matrix[i][j] / scalar;
			}
		}
		return v;
	}

	/*
	* Overloaded square brackets operator for CybMatrix objects.
	* Implements row access to this object's matrix.
	*
	* @param int x: the number of the row that needs to be accessed.
	* This number must be in the interval [0, dim1 - 1].
	* @return vector<type>&: a reference to the row of the matrix that corresponds
	* to the parameter index.
	*/
	vector<type>& operator[](int x)
	{
		if (x < 0 || x >= this->dim1) {
			cerr << "Invalid operation with operator [" << x << "]. Terminating." << endl;
			exit(1);
		}
		return this->matrix[x];
	}

	/*
	* Overloaded parenthesis operator for CybMatrix objects.
	* Implements element access to this object's matrix.
	*
	* @param int i: the number of the row that needs to be accessed.
	* This number must be in the interval [0, dim1 - 1].
	* @param int j: the number of the column that needs to be accessed.
	* This number must be in the interval [0, dim2 - 1].
	* @return type&: a reference to the element of the matrix that corresponds
	* to the parameter indices.
	*/
	type& operator()(int i, int j)
	{
		if (i < 0 || i >= dim1 || j < 0 || j >= dim2) {
			cerr << "Invalid operation with operator (" << i << "," << j << "). Terminating." << endl;
			exit(1);
		}
		return matrix[i][j];
	}

	/*
	* Getter for this object's first dimension.
	*
	* @param void
	* @return int: the first dimension of this matrix.
	*/
	int getDimension1() { return this->dim1; }

	/*
	* Getter for this object's second dimension.
	*
	* @param void
	* @return int: the second dimension of this matrix.
	*/
	int getDimension2() { return this->dim2; }

	/*
	* This method prints the current values inside this matrix.
	*
	* @param void
	* @return void
	*/
	void printMatrix()
	{
		for (int i = 0; i < dim1; ++i) {
			for (int j = 0; j < dim2; ++j) {
				cout << matrix[i][j] << "   ";
			}
			cout << endl;
		}
		cout << "\n" << endl;
	}

	/*
	* This method sets this matrix as an identity one.
	*
	* @param void
	* @return void
	*/
	void setIdentity()
	{
		for (int i = 0; i < this->dim1; ++i) {
			for (int j = 0; j < this->dim2; ++j) {
				if (i == j) this->matrix[i][j] = 1;
				else this->matrix[i][j] = 0;
			}
		}
	}

	/*
	* This method sets the matrix's elements according to the vector passed
	* as paramater.
	*
	* @param vector<type>& v: the vector that will be used to set the elements.
	* It must be of size dim1 * dim2.
	* @return void
	*/
	void setMatrix(vector<type>& v)
	{
		int contador = 0;
		for (int i = 0; i < this->dim1; ++i) {
			for (int j = 0; j < this->dim2; ++j) {
				this->matrix[i][j] = array[contador++];
			}
		}
	}

	/*
	* This method creates a new matrix that is the tranposed form of this
	* object's matrix.
	*
	* @param void
	* @return CybMatrix<type>: a reference to the new transposed matrix.
	*/
	CybMatrix<type> transpose()
	{
		CybMatrix<type> transp(this->dim2, this->dim1);
		for (int i = 0; i < this->dim2; ++i) {
			for (int j = 0; j < this->dim1; ++j) {
				transp->matrix[i][j] = this->matrix[j][i];
			}
		}
		return transp;
	}

	/*
	* This method transforms this matrix into an equivalent upper triangular
	* matrix using Gauss' elimination method.
	*
	* @param void
	* @return void
	*/
	void toUpperTriangular()
	{
		for (int i = 0; i < this->dim1; ++i) {
			for (int j = 0; j != i; ++j) {
				if (this->matrix[i][j]) {
					double* aux = new double[this->dim2];
					double prod = -(this->matrix[i][j] / this->matrix[j][j]);
					for (int k = 0; k < this->dim2; ++k) {
						aux[k] = prod * this->matrix[j][k];
					}
					for (int l = 0; l < dim2; ++l) {
						this->matrix[i][l] += aux[l];
					}
					delete[] aux;
				}
			}
		}
	}

	/*
	*	This method eliminates a line and a column from the matrix.
	*
	*	@param unsigned int line: the line to be eliminated.
	*	@param unsigned int column: the column to be eliminated.
	*	@return CybMatrix<type>: a new matrix with the result of the elimination.
	*/
	CybMatrix<type> deleteLineAndColumn(unsigned int line, unsigned int column)
	{
		if (line < 0 || line >= dim1) {
			cerr << "Invalid line index for elimination." << endl;
			exit(1);
		}

		if (column < 0 || column >= dim2) {
			cerr << "Invalid column index for elimination." << endl;
			exit(1);
		}

		if (dim1 == 1 || dim2 == 1) {
			cerr << "Insufficient matrix size for elimination." << endl;
			exit(1);
		}

		CybMatrix<type> res(dim1 - 1, dim2 - 1);

		for (int i = 0; i < dim1; ++i) {
			if (i == line)
				continue;
			for (int j = 0; j < dim2; ++j) {
				if (j == column)
					continue;
				else
					res[i][j] = matrix[i][j];
			}
		}

		return res;
	}

	/*
	*	Calculates the determinant for numeric square matrices.
	*/
	double determinant()
	{
		double det = 0.0;

		if (dim1 != dim2)
			return 0.0;

		if (dim1 == 0)
			return 0.0;

		if (dim1 == 1)
			return matrix[0][0];

		if (dim1 == 2)
			return (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);

		if (dim1 == 3)
			return (matrix[0][0] * matrix[1][1] * matrix[2][2]) +
			(matrix[0][1] * matrix[1][2] * matrix[2][0]) +
			(matrix[0][2] * matrix[1][0] * matrix[2][1]) -
			(matrix[0][2] * matrix[1][1] * matrix[0][2]) -
			(matrix[1][2] * matrix[2][1] * matrix[0][0]) -
			(matrix[2][2] * matrix[0][1] * matrix[1][2]);

		if (dim1 > 3) {
			CybMatrix<type> m(dim1 - 1, dim1 - 1);
			for (int col = 0; col < dim1; ++col) {
				m = this->deleteLineAndColumn(0, col);

				if (!(col % 2))
					det += m.determinant() * m[0][col];
				else
					det -= m.determinant() * m[0][col];
			}
		}
		return det;
	}

	/*
	*	Calculates the inverse matrix.
	*
	*	@return CybMatrix<type>: the inverse matrix.
	*/
	CybMatrix<type> invertMatrix()
	{
		if (dim1 != dim2) {
			cerr << "Cannot invert non-square matrix." << endl;
			exit(1);
		}
		
		int index = 1;
		double det = this->determinant();

		if (dim1 == 1) {
			CybMatrix<type> inv(1, 1);
			inv[0][0] = 1.0 / matrix[0][0];
			return inv;
		}
		else {
			CybMatrix<type> inv(dim1, dim1);

			if (dim1 % 2) {
				for (int l = 0; l < dim1; ++l) {
					for (int c = 0; c < dim1; ++c) {
						CybMatrix<type> aux(dim1 - 1, dim1 - 1);
						aux = this->deleteLineAndColumn(l, c);
						inv[l][c] = index * aux.determinant() / det;
						index *= -1;
					}
				}
			}
			else {
				for (int l = 0; l < dim1; ++l) {
					for (int c = 0; c < dim1; ++c) {
						CybMatrix<type> aux(dim1 - 1, dim1 - 1);
						aux = this->deleteLineAndColumn(l, c);
						inv[l][c] = index * aux.determinant() / det;
						index *= -1;
					}
					index *= -1;
				}
			}

			return inv;
		}
	}

private:
	int dim1;
	int dim2;
	vector< vector<type> > matrix;
};