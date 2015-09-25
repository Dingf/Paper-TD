#ifndef INC_POINT_H
#define INC_POINT_H

#include "type.h"
#include "vector.h"

struct Vector3D;
struct Point3D;
struct Point2D
{
	public:
		Point2D();
		Point2D(Float u, Float v);
		Point2D(const Point2D& p);

		operator Vector3D() const;
		operator Point3D() const;

		bool operator == (const Point2D& p) const;
		bool operator != (const Point2D& p) const;

		Point2D operator ! () const;

		Point2D operator + (const Point2D& p) const;
		Point2D operator - (const Point2D& p) const;
		Point2D operator * (Float scale) const;
		Point2D operator / (Float scale) const;

		Point2D& operator = (const Point2D& p);
		Point2D& operator += (const Point2D& p);
		Point2D& operator -= (const Point2D& p);
		Point2D& operator *= (Float scale);
		Point2D& operator /= (Float scale);

		void SetValues(Float u, Float v);
		void GetValues(Float& u, Float& v) const;
		Float * GetValues() { return &u; }

		Float GetX() const { return u; }
		Float GetY() const { return v; }

		Float GetU() const { return u; }
		Float GetV() const { return v; }

	private:
		Float u, v;
};

struct Point3D
{
	public:
		Point3D();
		Point3D(Float x, Float y, Float z);
		Point3D(const Point3D& p);

		operator Vector3D() const;
		operator Point2D() const;

		bool operator == (const Point3D& p) const;
		bool operator != (const Point3D& p) const;

		Point3D operator ! () const;

		Point3D operator + (const Point3D& p) const;
		Point3D operator - (const Point3D& p) const;
		Point3D operator * (Float scale) const;
		Point3D operator / (Float scale) const;

		Point3D& operator = (const Point3D& p);
		Point3D& operator += (const Point3D& p);
		Point3D& operator -= (const Point3D& p);
		Point3D& operator *= (Float scale);
		Point3D& operator /= (Float scale);

		void SetValues(Float x, Float y);
		void SetValues(Float x, Float y, Float z);
		void GetValues(Float& x, Float& y, Float& z) const;
		Float * GetValues() { return &x; }

		Float GetX() const { return x; }
		Float GetY() const { return y; }
		Float GetZ() const { return z; }

	private:
		Float x, y, z;
};

extern Point3D Origin;
extern Point3D NullPoint;

Float CalculateDistance(const Point3D& point1, const Point3D& point2);
Float CalculateSlope(const Point3D& point1, const Point3D& point2);

#endif