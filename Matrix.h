// Copyright (c) Jonathan Karlsson 2011
// Code may be used freely for non-commercial purposes.
// Author retains the right to use code for commercial purposes.
// Author retains his moral rights under the applicable copyright laws.

#ifndef MAT_H_INCLUDED__
#define MAT_H_INCLUDED__

#include <stdarg.h>
#include "Vector.h"

namespace mml
{

	//
	// Matrix
	//
	template < int rows, int columns > // rows is rows (V), columns is columns (->), p_row is current row, p_column is current column
	class Matrix
	{
	public:
		static const int Rows = rows;
		static const int Columns = columns;
	private:
		mml::Vector<columns> e[rows];
	public:
		//
		// index operators
		//
		const mml::Vector<columns> &operator[](int p_row) const { return e[p_row]; }
		mml::Vector<columns> &operator[](int p_row) { return e[p_row]; }
		//
		// conversion operators
		//
		operator float * ( void ) { return e[0]; }
		operator const float * const ( void ) const { return e[0]; }
	public:
		//
		// default
		//
		Matrix( void ) {
			// don't initialize
		}
		//
		// copy
		//
		Matrix(const mml::Matrix<rows,columns> &mat) {
			for (int p_row = 0; p_row < rows; ++p_row) {
				for (int p_column = 0; p_column < columns; ++p_column) {
					e[p_row][p_column] = mat[p_row][p_column];
				}
			}
		}
		//
		// conversion
		//
		template < int m2, int n2>
		explicit Matrix(const mml::Matrix<m2,n2> &mat) {
			const int minm = rows < m2 ? rows : m2;
			const int minn = columns < n2 ? columns : n2;
			int p_row = 0;
			for (; p_row < minm; ++p_row) {
				int p_column = 0;
				for (; p_column < minn; ++p_column) {
					e[p_row][p_column] = mat[p_row][p_column];
				}
				for (; p_column < columns; ++p_column) {
					e[p_row][p_column] = 0.f;
				}
			}
			for (; p_row < rows; ++p_row) {
				for (int p_column = 0; p_column < columns; ++p_column) {
					e[p_row][p_column] = 0.f;
				}
			}
		}
		//
		// initializer
		//
		Matrix(float e00, ...) {
			va_list vl;
			va_start(vl, e00);
			e[0][0] = e00;
			for (int p_column = 1; p_column < columns; ++p_column) { e[0][p_column] = (float)va_arg(vl, double); }
			for (int p_row = 1; p_row < rows; ++p_row) {
				for (int p_column = 0; p_column < columns; ++p_column) {
					e[p_row][p_column] = (float)va_arg(vl, double);
				}
			}
			va_end(vl);
		}
		//
		// conversion
		//
		Matrix(const float * const mat) {
			for (int p_row = 0; p_row < rows; ++p_row) {
				for (int p_column = 0; p_column < columns; ++p_column) {
					e[p_row][p_column] = mat[p_row*rows + p_column];
				}
			}
		}
	public:
		//
		// Stack
		//
		void Stack(const mml::Matrix<rows,columns> &mat)
		{
			*this = mat * (*this);
		}
		//
		// IdentityMatrix
		//
		void SetIdentity( void )
		{
			for (int p_row = 0; p_row < rows; ++p_row) {
				for (int p_column = 0; p_column < columns; ++p_column) {
					e[p_row][p_column] = (float)(p_row == p_column);
				}
			}
		}
		//
		// IdentityMatrix
		//
		static mml::Matrix<rows,rows> IdentityMatrix( void )
		{
			mml::Matrix<rows,rows> ident;
			ident.SetIdentity();
			return ident;
		}
	};

}

namespace mml
{

	//
	// Transp
	//
	template < int rows, int columns >
	mml::Matrix<columns,rows> Transp(const mml::Matrix<rows,columns> &mat)
	{
		mml::Matrix<columns,rows> transp;
		for (int p_row = 0; p_row < rows; ++p_row) {
			for (int p_column = 0; p_column < columns; ++p_column) {
				transp[p_column][p_row] = mat[p_row][p_column];
			}
		}
		return transp;
	}

	//
	// Collapse
	//
	template < int rows, int columns >
	mml::Matrix<rows-1,columns-1> Collapse(const mml::Matrix<rows,columns> &mat, int p_row, int p_column)
	{
		mml::Matrix<rows-1,columns-1> min;

		int i2 = 0;
		for (; i2 < p_row; ++i2) {
			int j2 = 0;
			for (; j2 < p_column; ++j2) { min[i2][j2] = mat[i2][j2]; }
			for (; j2 < columns-1; ++j2) { min[i2][j2] = mat[i2][j2+1]; }
		}
		for (; i2 < rows-1; ++i2) {
			int j2 = 0;
			for (; j2 < p_column; ++j2) { min[i2][j2] = mat[i2+1][j2]; }
			for (; j2 < columns-1; ++j2) { min[i2][j2] = mat[i2+1][j2+1]; }
		}

		return min;
	}

	//
	// Trace
	//
	template < int rows >
	float Trace(const mml::Matrix<rows,rows> &mat)
	{
		float trace = 0.f;
		for (int p_row = 0; p_row < rows; ++p_row) {
			trace += mat[p_row][p_row];
		}
		return trace;
	}

	//
	// Det
	//
	template < int rows >
	float Det(const mml::Matrix<rows,rows> &mat)
	{
		// Stopping conditions
		if (rows < 1) { return 0.f; }
		if (rows == 1) { return mat[0][0]; }

		// Transform to upper triangular matrix
		mml::Matrix<rows,rows> upperTri = mat;
		int counti = 0;
		for(int p_row = 0; p_row < rows-1; ++p_row) {
			// Elementary Row Operation I
			if(upperTri[p_row][p_row] == 0.f) {
				for(int k = p_row; k < rows; ++k) {
					if(upperTri[k][p_row] != 0.f) {
						for(int p_column = 0; p_column < rows; ++p_column) {
							float temp = upperTri[p_row][p_column];
							upperTri[p_row][p_column] = upperTri[k][p_column];
							upperTri[k][p_column] = temp;
						}
						k = rows;
					}
				}
				++counti;
			}
			// Elementary Row Operation III
			if(upperTri[p_row][p_row] != 0.f) {
				for(int k = p_row + 1; k < rows; ++k) {
					float factor = -1.f * upperTri[k][p_row] /  upperTri[p_row][p_row];
					for(int p_column = p_row; p_column < rows; ++p_column) {
						upperTri[k][p_column] = upperTri[k][p_column] + (factor * upperTri[p_row][p_column]);
					}
				}
			}
		}

		// Calculate determinant
		float det = 1.f;
		for (int p_row = 0; p_row < rows; ++p_row) {
			det *= upperTri[p_row][p_row];
		}
		if (counti % 2 != 0) {
			det = -det;
		}

		return det;
	}

	//
	// Inv
	//
	template < int rows >
	mml::Matrix<rows,rows> Inv(const mml::Matrix<rows,rows> &mat)
	{
		// Hasn't checked the determinant. If det=0 then there is no inverse.
		mml::Matrix<rows,rows> inv = mat;
		//if (rows <= 0) { return; }  // sanity check
		//if (rows == 1) { return; }  // must be of dimension >= 2
		for (int p_row = 1; p_row < rows; ++p_row) { inv[0][p_row] /= inv[0][0]; } // normalize row 0
		for (int p_row = 1; p_row < rows; ++p_row) {
			for (int p_column = p_row; p_column < rows; ++p_column) { // do a column of L
				float sum = 0.f;
				for (int k = 0; k < p_row; ++k) {
					sum += inv[p_column][k] * inv[k][p_row];
				}
				inv[p_column][p_row] -= sum;
			}
			if (p_row == rows-1) continue;
			for (int p_column = p_row+1; p_column < rows; ++p_column) {  // do a row of U
				float sum = 0.f;
				for (int k = 0; k < p_row; ++k) {
					sum += inv[p_row][k]*inv[k][p_column];
				}
				inv[p_row][p_column] = (inv[p_row][p_column]-sum) / inv[p_row][p_row];
			}
		}
		for (int p_row = 0; p_row < rows; ++p_row) { // invert L
			for (int p_column = p_row; p_column < rows; ++p_column) {
				float x = 1.f;
				if (p_row != p_column) {
					x = 0.f;
					for (int k = p_row; k < p_column; ++k) {
						x -= inv[p_column][k]*inv[k][p_row];
					}
				}
				inv[p_column][p_row] = x / inv[p_column][p_column];
			}
		}
		for (int p_row = 0; p_row < rows; ++p_row) {  // invert U
			for (int p_column = p_row; p_column < rows; ++p_column) {
				if (p_row == p_column) { continue; }
				float sum = 0.f;
				for (int k = p_row; k < p_column; ++k) {
					sum += inv[k][p_column]*( (p_row==k) ? 1.f : inv[p_row][k] );
				}
				inv[p_row][p_column] = -sum;
			}
		}
		for (int p_row = 0; p_row < rows; ++p_row) {   // final inversion
			for (int p_column = 0; p_column < rows; ++p_column) {
				float sum = 0.f;
				for (int k = ((p_row>p_column)?p_row:p_column); k < rows; ++k) {
					sum += ((p_column==k)?1.f:inv[p_column][k])*inv[k][p_row];
				}
				inv[p_column][p_row] = sum;
			}
		}
		return inv;
	}

	inline void FromVec3(const mml::Vector<3> &p_vec, mml::Matrix<3,3> &p_matrix)
	{
		mml::Matrix<3,3> m = p_matrix;
		float d;
		m[0] = p_vec;
		d = p_vec[0] * p_vec[0] + p_vec[1] * p_vec[1];
		if (!d) {
			m[1][0] = 1.f;
			m[1][1] = 0.f;
			m[1][2] = 0.f;
		} else {
			d = 1.f/sqrt(d);
			m[1][0] = -p_vec[1]*d;
			m[1][1] = p_vec[0]*d;
			m[1][2] = 0.f;
		}
		m[2] = mml::Cross(p_vec, m[1]);
		p_matrix = m;
	}

}

//
// Matrix/Matrix operators
//
template < int rows, int columns >
mml::Matrix<rows,columns> &operator+=(mml::Matrix<rows,columns> &l, const mml::Matrix<rows,columns> &r)
{
	for (int p_row = 0; p_row < rows; ++p_row) {
		l[p_row] += r[p_row];
	}
	return l;
}
template < int rows, int columns >
mml::Matrix<rows,columns> &operator-=(mml::Matrix<rows,columns> &l, const mml::Matrix<rows,columns> &r)
{
	for (int p_row = 0; p_row < rows; ++p_row) {
		l[p_row] -= r[p_row];
	}
	return l;
}
template < int rows >
mml::Matrix<rows,rows> &operator*=(mml::Matrix<rows,rows> &l, mml::Matrix<rows,rows> r)
{
	r = Transp(r); // transpose right
	for (int p_row = 0; p_row < rows; ++p_row) {
		const mml::Vector<rows> li = l[p_row]; // current l vector
		for (int p_column = 0; p_column < rows; ++p_column) {
			l[p_row][p_column] = Dot(li, r[p_column]);
		}
	}
	return l;
}
template < int rows, int columns >
mml::Matrix<rows,columns> &operator*=(mml::Matrix<rows,columns> &l, float r)
{
	for (int p_row = 0; p_row < rows; ++p_row) {
		l[p_row] *= r;
	}
	return l;
}
template < int rows, int columns >
mml::Matrix<rows,columns> operator+(mml::Matrix<rows,columns> l, const mml::Matrix<rows,columns> &r) { return (l+=r); }
template < int rows, int columns >
mml::Matrix<rows,columns> operator-(mml::Matrix<rows,columns> l, const mml::Matrix<rows,columns> &r) { return (l-=r); }
template < int left_rows, int shared, int right_columns >
mml::Matrix<left_rows,right_columns> operator*(const mml::Matrix<left_rows,shared> &l, const mml::Matrix<shared,right_columns> &r)
{
	const mml::Matrix<right_columns,shared> tr = Transp(r); // transpose right
	mml::Matrix<left_rows,right_columns> mat;
	for (int p_row = 0; p_row < left_rows; ++p_row) {
		for (int p_column = 0; p_column < right_columns; ++p_column) {
			mat[p_row][p_column] = Dot(l[p_row], tr[p_column]);
		}
	}
	return mat;
}
template < int rows, int columns >
mml::Matrix<rows,columns> operator+(mml::Matrix<rows,columns> l, float r) { return (l*=r); }
template < int rows, int columns >
mml::Matrix<rows,columns> operator+(float l, mml::Matrix<rows,columns> r) { return (r*=l); }

//
// mml::Vector/Matrix operators
//
template < int rows, int columns >
mml::Vector<rows> operator*(const mml::Vector<columns> &vec, const mml::Matrix<rows,columns> &mat)
{
	mml::Vector<rows> rvec;
	for (int p_row = 0; p_row < rows; ++p_row) {
		rvec[p_row] = Dot(vec, mat[p_row]);
	}
	return rvec;
}
template < int rows >
mml::Vector<rows> &operator*=(mml::Vector<rows> &vec, const mml::Matrix<rows,rows> &mat)
{
	vec = vec * mat;
	return vec;
}
// additional math functionality that is used often
inline mml::Vector<3> operator*(const mml::Vector<3> &v, const mml::Matrix<4,4> &m)
{
	const mml::Vector<3> *m0 = (const mml::Vector<3>*)&m[0];
	const mml::Vector<3> *m1 = (const mml::Vector<3>*)&m[1];
	const mml::Vector<3> *m2 = (const mml::Vector<3>*)&m[2];
	return mml::Vector<3>(
		mml::Dot(v, *m0) + m[0][3],
		mml::Dot(v, *m1) + m[1][3],
		mml::Dot(v, *m2) + m[2][3]
		);
}
inline mml::Vector<3> &operator*=(mml::Vector<3> &v, const mml::Matrix<4,4> &m)
{
	v = v * m;
	return v;
}

#endif
