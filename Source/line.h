#ifndef INC_LINE_H
#define INC_LINE_H

#include "type.h"
#include "point.h"
#include "vector.h"
#include "shape.h"

struct LineSegment;
struct Line : public Shape
{
	public:
		Line();
		Line(const Point3D& p1, const Point3D& p2);
		Line(const Line& l);

		operator LineSegment() const;

		bool operator == (const Line& l) const;
		bool operator != (const Line& l) const;

		Line operator + (const Vector3D& v) const;
		Line operator - (const Vector3D& v) const;
		Line operator * (Float scale) const;
		Line operator / (Float scale) const;

		Line& operator = (const Line& l);
		Line& operator += (const Vector3D& v);
		Line& operator -= (const Vector3D& v);
		Line& operator *= (Float scale);
		Line& operator /= (Float scale);

		void SetValues(const Point3D& p1, const Point3D& p2);
		void GetValues(Point3D& p1, Point3D& p2) const;
		Point3D * GetValues() { return &p1; }

		Float GetValueAt(Float x) const;
		Float GetSlope() const { return CalculateSlope(p1, p2); }
			 
		Point3D GetMidpoint() const { return (p1 + p2)/2; }

			  Point3D& GetPoint1()       { return p1; }
		const Point3D& GetPoint1() const { return p1; }
			  Point3D& GetPoint2()       { return p2; }
		const Point3D& GetPoint2() const { return p2; }
			  Vector3D& GetDirection()		 { return direction; }
		const Vector3D& GetDirection() const { return direction; }

		Point3D GetRandomPoint() const;
		bool ContainsPoint(const Point3D& point) const;
		void Translate(const Vector3D& v);

		Shape * CreateInstance() const;

	private:
		Point3D p1, p2;
		Vector3D direction;
};

struct LineSegment : public Shape
{
	public:
		LineSegment();
		LineSegment(const Point3D& p1, const Point3D& p2);
		LineSegment(const LineSegment& l);

		operator Line() const;

		bool operator == (const LineSegment& l) const;
		bool operator != (const LineSegment& l) const;

		LineSegment operator + (const Vector3D& v) const;
		LineSegment operator - (const Vector3D& v) const;
		LineSegment operator * (Float scale) const;
		LineSegment operator / (Float scale) const;

		LineSegment& operator = (const LineSegment& l);
		LineSegment& operator += (const Vector3D& v);
		LineSegment& operator -= (const Vector3D& v);
		LineSegment& operator *= (Float scale);
		LineSegment& operator /= (Float scale);

		void SetValues(const Point3D& p1, const Point3D& p2);
		void GetValues(Point3D& p1, Point3D& p2) const;
		Point3D * GetValues() { return &p1; }

		Float GetValueAt(Float x) const;

		Float GetSlope() const { return CalculateSlope(p1, p2); }
		Float GetLength() const { return CalculateDistance(p1, p2); }

		Point3D GetMidpoint() const { return (p1 + p2)/2; }

			  Point3D& GetPoint1()       { return p1; }
		const Point3D& GetPoint1() const { return p1; }
			  Point3D& GetPoint2()       { return p2; }
		const Point3D& GetPoint2() const { return p2; }
			  Vector3D& GetDirection()		 { return direction; }
		const Vector3D& GetDirection() const { return direction; }

		Point3D GetRandomPoint() const;
		bool ContainsPoint(const Point3D& point) const;
		void Translate(const Vector3D& v);

		Shape * CreateInstance() const;

	private:
		Point3D p1, p2;
		Vector3D direction;
};

#endif