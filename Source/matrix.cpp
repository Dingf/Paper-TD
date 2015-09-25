#include "type.h"
#include "vector.h"
#include "matrix.h"

Matrix::Matrix()
{
	SetIdentity();
}

Matrix::Matrix(const Vector3D& row0, const Vector3D& row1, const Vector3D& row2)
{
	Rows[0] = row0;
	Rows[1] = row1;
	Rows[2] = row2;
}

Matrix::Matrix(const Matrix& m)
{
	*this = m;
}

Vector3D& Matrix::operator [] (uint32 index)
{
	return Rows[index];
}

const Vector3D& Matrix::operator [] (uint32 index) const
{
	return Rows[index];
}

bool Matrix::operator == (const Matrix& m) const
{
	return ((Rows[0] == m.Rows[0]) && (Rows[1] == m.Rows[1]) && (Rows[2] == m.Rows[2]));
}

bool Matrix::operator != (const Matrix& m) const
{
	return !(*this == m);
}

Matrix Matrix::operator + (const Matrix& m)
{
	return Matrix(Rows[0] + m.Rows[0], Rows[1] + m.Rows[1], Rows[2] + m.Rows[2]);
}

Matrix Matrix::operator - (const Matrix& m)
{
	return Matrix(Rows[0] - m.Rows[0], Rows[1] - m.Rows[1], Rows[2] - m.Rows[2]);
}

Matrix Matrix::operator * (Float scale)
{
	return Matrix(Rows[0] * scale, Rows[1] * scale, Rows[2] * scale);
}

Matrix Matrix::operator * (const Vector3D& v)
{
	return Matrix(Rows[0] * v.GetX(), Rows[1] * v.GetY(), Rows[2] * v.GetZ());
}

Matrix Matrix::operator * (const Matrix& m)
{
	Matrix matrix = m.Transpose();
	return Matrix(
		Vector3D(Rows[0].DotProduct(matrix.Rows[0]), Rows[0].DotProduct(matrix.Rows[1]), Rows[0].DotProduct(matrix.Rows[2])),
		Vector3D(Rows[1].DotProduct(matrix.Rows[0]), Rows[1].DotProduct(matrix.Rows[1]), Rows[1].DotProduct(matrix.Rows[2])),
		Vector3D(Rows[2].DotProduct(matrix.Rows[0]), Rows[2].DotProduct(matrix.Rows[1]), Rows[2].DotProduct(matrix.Rows[2])));
}

Matrix Matrix::operator / (Float scale)
{
	return Matrix(Rows[0]/scale, Rows[1]/scale, Rows[2]/scale);
}

Matrix Matrix::operator / (const Vector3D& v)
{
	return Matrix(Rows[0]/v.GetX(), Rows[1]/v.GetY(), Rows[2]/v.GetZ());
}

Matrix Matrix::operator / (const Matrix& m)
{
	Matrix matrix = *this;
	return matrix * m.Inverse();
}

const Matrix& Matrix::operator = (const Matrix& m)
{
	Rows[0] = m.Rows[0];
	Rows[1] = m.Rows[1];
	Rows[2] = m.Rows[2];
	return *this;
}

const Matrix& Matrix::operator += (const Matrix& m) 
{ 
	Rows[0] += m.Rows[0];
	Rows[1] += m.Rows[1];
	Rows[2] += m.Rows[2];
	return *this; 
}

const Matrix& Matrix::operator -= (const Matrix& m)
{
	Rows[0] -= m.Rows[0];
	Rows[1] -= m.Rows[1];
	Rows[2] -= m.Rows[2];
	return *this;
}

const Matrix& Matrix::operator *= (Float scale)
{
	Rows[0] *= scale;
	Rows[1] *= scale;
	Rows[2] *= scale;
	return *this;
}

const Matrix& Matrix::operator *= (const Matrix& m)
{
	*this = *this * m;
	return *this;
}

const Matrix& Matrix::operator /= (Float scale)
{
	Float invScale = (scale == 0.0) ? 0.0 : 1.0/scale;

	Rows[0] *= invScale;
	Rows[1] *= invScale;
	Rows[2] *= invScale;
	return * this;
}

const Matrix& Matrix::operator /= (const Matrix& m)
{
	*this = *this * m.Inverse();
	return *this;
}

bool Matrix::IsIdentity() const
{
	Matrix identity;
	return (*this == identity);
}

void Matrix::SetIdentity()
{
	Rows[0].SetValues(1, 0, 0);
	Rows[1].SetValues(0, 1, 0);
	Rows[2].SetValues(0, 0, 1);
}

Matrix Matrix::Inverse() const
{
	if (Determinant() == 0)
	{
		return Matrix(
				Vector3D(0, 0, 0),
				Vector3D(0, 0, 0),
				Vector3D(0, 0, 0));
	}
	else
	{
		Matrix matrix = Transpose();
		Float blah = Determinant();
		Float invDet = 1.0/Determinant();

		return Matrix(
				matrix.Rows[1].CrossProduct(matrix.Rows[2]) * invDet,
				matrix.Rows[2].CrossProduct(matrix.Rows[0]) * invDet,
				matrix.Rows[0].CrossProduct(matrix.Rows[1]) * invDet);
	}
}


Matrix Matrix::Transpose() const
{
	return Matrix(
		Vector3D(Rows[0].GetX(), Rows[1].GetX(), Rows[2].GetX()),
		Vector3D(Rows[0].GetY(), Rows[1].GetY(), Rows[2].GetY()),
		Vector3D(Rows[0].GetZ(), Rows[1].GetZ(), Rows[2].GetZ()));
}

Float Matrix::Determinant() const
{
	return (
		(Rows[0].GetX() * Rows[1].GetY() * Rows[2].GetZ())
	  + (Rows[0].GetY() * Rows[1].GetZ() * Rows[2].GetX())
	  + (Rows[0].GetZ() * Rows[1].GetX() * Rows[2].GetY())
	  - (Rows[0].GetX() * Rows[1].GetZ() * Rows[2].GetY())
	  - (Rows[0].GetZ() * Rows[1].GetY() * Rows[2].GetX())
	  - (Rows[0].GetY() * Rows[1].GetX() * Rows[2].GetZ()));
}