#ifndef INC_VECTOR_H
#define INC_VECTOR_H

#include "type.h"
#include "point.h"

struct Point2D;
struct Point3D;
struct Vector3D
{
	public:
		Vector3D();
		Vector3D(Float x, Float y, Float z);
		Vector3D(Point3D p1, Point3D p2);
		Vector3D(const Vector3D& v);

		operator Point3D() const;
		operator Point2D() const;

		bool operator == (const Vector3D& v) const;
		bool operator != (const Vector3D& v) const;

		Vector3D operator ! () const;

		Vector3D operator + (const Vector3D& v) const;
		Vector3D operator - (const Vector3D& v) const;
		Vector3D operator * (Float scale) const;
		Vector3D operator / (Float scale) const;

		Vector3D& operator = (const Vector3D& v);
		Vector3D& operator += (const Vector3D& v);
		Vector3D& operator -= (const Vector3D& v);
		Vector3D& operator *= (Float scale);
		Vector3D& operator /= (Float scale);

		bool IsZero() const;

		Vector3D CrossProduct(const Vector3D& v) const;
		Float DotProduct(const Vector3D& v) const;

		Float GetLength() const;
		Float GetZeroAngleD() const;
		Float GetZeroAngleR() const { return GetZeroAngleD()/57.29577951; }

		Vector3D GetUnitVector() const;
		void SetUnitVector();

		void SetValues(Float x, Float y, Float z);
		void SetValues(Point3D p1, Point3D p2);
		void GetValues(Float& x, Float& y, Float& z) const;
		Float * GetValues() { return &x; }

		Float GetX() const { return x; }
		Float GetY() const { return y; }
		Float GetZ() const { return z; }

	private:
		Float x, y, z;
};

#endif
