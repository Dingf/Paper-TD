#ifndef INC_CIRCLE_H
#define INC_CIRCLE_H

#include "type.h"
#include "point.h"
#include "vector.h"
#include "shape.h"

struct Circle : public Shape
{
	public:
		Circle();
		Circle(const Point3D& center, Float radius);
		Circle(const Circle& r);

		bool operator == (const Circle& r) const;
		bool operator != (const Circle& r) const;

		Circle operator + (Float scale) const;
		Circle operator - (Float scale) const;
		Circle operator + (const Vector3D& v) const;
		Circle operator - (const Vector3D& v) const;
		Circle operator * (Float scale) const;
		Circle operator / (Float scale) const;

		Circle& operator = (const Circle& c);
		Circle& operator += (Float scale);
		Circle& operator -= (Float scale);
		Circle& operator += (const Vector3D& v);
		Circle& operator -= (const Vector3D& v);
		Circle& operator *= (Float scale);
		Circle& operator /= (Float scale);

		void SetValues(const Point3D& center, Float radius);
		void GetValues(Point3D& center, Float& radius) const;

		Float GetRadius() const { return radius; }

		      Point3D& GetCenter()       { return center; }
		const Point3D& GetCenter() const { return center; }

		Point3D GetRandomPoint() const;
		bool ContainsPoint(const Point3D& point) const;
		void Translate(const Vector3D& v);

		Shape * CreateInstance() const;

	private:
		Point3D center;
		Float radius;
};

#endif