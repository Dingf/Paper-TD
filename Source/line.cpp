#include "type.h"
#include "math.h"
#include "utility.h"
#include "point.h"
#include "shapes.h"

Line::Line()
{
	SetType(SHAPE_TYPE_LINE);
	SetValues(Point3D(0, 0, 0), Point3D(0, 0, 0));
}

Line::Line(const Point3D& p1, const Point3D& p2)
{
	SetType(SHAPE_TYPE_LINE);
	SetValues(p1, p2);
}

Line::Line(const Line& l)
{
	SetType(SHAPE_TYPE_LINE);
	*this = l;
}

Line::operator LineSegment() const
{
	return LineSegment(p1, p2);
}

bool Line::operator == (const Line& l) const
{
	return ((p1 == l.p1) && (p2 == l.p2));
}

bool Line::operator != (const Line& l) const
{
	return !(*this == l);
}

Line Line::operator + (const Vector3D& v) const
{
	return Line(p1 + v, p2 + v);
}

Line Line::operator - (const Vector3D& v) const
{
	return Line(p1 - v, p2 - v);
}

Line Line::operator * (Float scale) const
{
	return Line(p1 * scale, p2 * scale);
}

Line Line::operator / (Float scale) const
{
	return Line(p1/scale, p2/scale);
}

Line& Line::operator = (const Line& l)
{
	SetValues(l.p1, l.p2);
	return *this;
}

Line& Line::operator += (const Vector3D& v)
{
	SetValues(p1 + v, p2 + v);
	return *this;
}

Line& Line::operator -= (const Vector3D& v)
{
	SetValues(p1 - v, p2 - v);
	return *this;
}

Line& Line::operator *= (Float scale)
{
	SetValues(p1 * scale, p2 * scale);
	return *this;
}

Line& Line::operator /= (Float scale)
{
	SetValues(p1/scale, p2/scale);
	return *this;
}

void Line::SetValues(const Point3D& p1, const Point3D& p2)
{
	this->p1 = (p1.GetX() < p2.GetX()) ? p1 : p2;
	this->p2 = (p1.GetX() >= p2.GetX()) ? p1 : p2;
	direction = p2 - p1;
}
void Line::GetValues(Point3D& p1, Point3D& p2) const
{
	p1 = this->p1;
	p2 = this->p2;
}

Float Line::GetValueAt(Float x) const
{
	if (p1.GetX() != p2.GetX())
	{
		Float slope = GetSlope();
		Float intercept = (-1.0 * slope * p1.GetX()) + p1.GetY();
		
		return (slope * x) + intercept;
	}
	return 0;
}

Point3D Line::GetRandomPoint() const
{
	if (p1.GetX() == p2.GetX())
	{
		Float y = RandomRange(p1.GetY(), p2.GetY());

		return Point3D(p1.GetX(), y, 0);
	}
	else
	{
		Float dx = RandomRange(0, p2.GetX() - p1.GetX());
		Float dy = GetSlope() * dx;

		return Point3D(p1.GetX() + dx,
					   p1.GetY() + dy,
					   0);
	}
}

bool Line::ContainsPoint(const Point3D& point) const
{
	Float slope = CalculateSlope(p1, point);
	return (Smooth(slope) == Smooth(GetSlope()));
}

void Line::Translate(const Vector3D& v)
{
	*this += v;
}

Shape * Line::CreateInstance() const
{
	return new Line(p1, p2);
}


LineSegment::LineSegment()
{
	SetType(SHAPE_TYPE_LINE_SEGMENT);
	SetValues(Point3D(0, 0, 0), Point3D(0, 0, 0));
}

LineSegment::LineSegment(const Point3D& p1, const Point3D& p2)
{
	SetType(SHAPE_TYPE_LINE_SEGMENT);
	SetValues(p1, p2);
}

LineSegment::LineSegment(const LineSegment& l)
{
	SetType(SHAPE_TYPE_LINE_SEGMENT);
	*this = l;
}

LineSegment::operator Line() const
{
	return Line(p1, p2);
}

bool LineSegment::operator == (const LineSegment& l) const
{
	return ((p1 == l.p1) && (p2 == l.p2));
}

bool LineSegment::operator != (const LineSegment& l) const
{
	return !(*this == l);
}

LineSegment LineSegment::operator + (const Vector3D& v) const
{
	return Line(p1 + v, p2 + v);
}

LineSegment LineSegment::operator - (const Vector3D& v) const
{
	return Line(p1 - v, p2 - v);
}

LineSegment LineSegment::operator * (Float scale) const
{
	return Line(p1 * scale, p2 * scale);
}

LineSegment LineSegment::operator / (Float scale) const
{
	return Line(p1/scale, p2/scale);
}

LineSegment& LineSegment::operator = (const LineSegment& l)
{
	SetValues(l.p1, l.p2);
	return *this;
}

LineSegment& LineSegment::operator += (const Vector3D& v)
{
	SetValues(p1 + v, p2 + v);
	return *this;
}

LineSegment& LineSegment::operator -= (const Vector3D& v)
{
	SetValues(p1 - v, p2 - v);
	return *this;
}

LineSegment& LineSegment::operator *= (Float scale)
{
	SetValues(p1 * scale, p2 * scale);
	return *this;
}

LineSegment& LineSegment::operator /= (Float scale)
{
	SetValues(p1/scale, p2/scale);
	return *this;
}

void LineSegment::SetValues(const Point3D& p1, const Point3D& p2)
{
	this->p1 = (p1.GetX() < p2.GetX()) ? p1 : p2;
	this->p2 = (p1.GetX() >= p2.GetX()) ? p1 : p2;
	direction = p2 - p1;
}

void LineSegment::GetValues(Point3D& p1, Point3D& p2) const
{
	p1 = this->p1;
	p2 = this->p2;
}

Float LineSegment::GetValueAt(Float x) const
{
	if (p1.GetX() != p2.GetX())
	{
		Float slope = GetSlope();
		Float intercept = (-1.0 * slope * p1.GetX()) + p1.GetY();
		
		return (slope * x) + intercept;
	}
	return 0;
}

Point3D LineSegment::GetRandomPoint() const
{
	if (p1.GetX() == p2.GetX())
	{
		Float y = RandomRange(p1.GetY(), p2.GetY());

		return Point3D(p1.GetX(), y, 0);
	}
	else
	{
		Float dx = RandomRange(0, p2.GetX() - p1.GetX());
		Float dy = GetSlope() * dx;

		return Point3D(p1.GetX() + dx,
					   p1.GetY() + dy,
					   0);
	}
}

bool LineSegment::ContainsPoint(const Point3D& point) const
{
	Rect rect(p1, p2);
	if (rect.ContainsPoint(point) == true)
	{
		Float slope = CalculateSlope(p1, point);
		return (Smooth(slope) == Smooth(GetSlope()));
	}
	return false;
}

void LineSegment::Translate(const Vector3D& v)
{
	*this += v;
}

Shape * LineSegment::CreateInstance() const
{
	return new LineSegment(p1, p2);
}