#ifndef INC_MATRIX_H
#define INC_MATRIX_H

#include "type.h"
#include "vector.h"

struct Matrix
{
	public:
		Matrix();
		Matrix(const Vector3D& row0, const Vector3D& row1, const Vector3D& row2);
		Matrix(const Matrix& m);

		Vector3D& operator [] (uint32 index);
		const Vector3D& operator [] (uint32 index) const;

		bool operator == (const Matrix& m) const;
		bool operator != (const Matrix& m) const;

		Matrix operator + (const Matrix& m);
		Matrix operator - (const Matrix& m);
		Matrix operator * (Float scale);
		Matrix operator * (const Vector3D& v);
		Matrix operator * (const Matrix& m);
		Matrix operator / (Float scale);
		Matrix operator / (const Vector3D& v);
		Matrix operator / (const Matrix& m);

		const Matrix& operator = (const Matrix& m);
		const Matrix& operator += (const Matrix& m);
		const Matrix& operator -= (const Matrix& m);
		const Matrix& operator *= (Float scale);
		const Matrix& operator *= (const Matrix& m);
		const Matrix& operator /= (Float scale);
		const Matrix& operator /= (const Matrix& m);

		bool IsIdentity() const;
		void SetIdentity();

		Matrix Inverse() const;
		Matrix Transpose() const;
		Float Determinant() const;

	private:
		Vector3D Rows[3];
};

#endif