#ifndef INC_RECTANGLE_H
#define INC_RECTANGLE_H

#include "type.h"
#include "point.h"
#include "vector.h"
#include "shape.h"

struct Rect : public Shape
{
	public:
		Rect();
		Rect(Float left, Float top, Float right, Float bottom);
		Rect(const Point2D& topLeft, const Point2D& bottomRight);
		Rect(const Rect& r);

		bool operator == (const Rect& r) const;
		bool operator != (const Rect& r) const;

		Rect operator + (Float scale) const;
		Rect operator - (Float scale) const;
		Rect operator + (const Vector3D& v) const;
		Rect operator - (const Vector3D& v) const;
		Rect operator * (Float scale) const;
		Rect operator / (Float scale) const;

		Rect& operator = (const Rect& r);
		Rect& operator += (Float scale);
		Rect& operator -= (Float scale);
		Rect& operator += (const Vector3D& v);
		Rect& operator -= (const Vector3D& v);
		Rect& operator *= (Float scale);
		Rect& operator /= (Float scale);

		void SetValues(Float left, Float top, Float right, Float bottom);
		void GetValues(Float& left, Float& top, Float& right, Float& bottom) const;
		Float * GetValues() { return (Float *)&left; }

		Float GetWidth() const { return right - left; }
		Float GetHeight() const { return bottom - top; }
		Float GetLeft() const { return left; }
		Float GetTop() const { return top; }
		Float GetRight() const { return right; }
		Float GetBottom() const { return bottom; }

		Point3D GetRandomPoint() const;
		bool ContainsPoint(const Point3D& point) const;
		void Translate(const Vector3D& v);

		Shape * CreateInstance() const;

	private:
		void FitRect();
		Float left, top, right, bottom;
};

#endif