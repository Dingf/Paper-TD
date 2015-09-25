#include "type.h"
#include "math.h"
#include "utility.h"
#include "point.h"
#include "shapes.h"

Circle::Circle()
{
	SetType(SHAPE_TYPE_CIRCLE);
	SetValues(Point3D(0, 0, 0), 0);
}

Circle::Circle(const Point3D& center, Float radius)
{
	SetType(SHAPE_TYPE_CIRCLE);
	SetValues(center, radius);
}

Circle::Circle(const Circle& r)
{
	SetType(SHAPE_TYPE_CIRCLE);
	*this = r;
}

bool Circle::operator == (const Circle& c) const
{
	return ((center == c.center) && (radius == c.radius));
}

bool Circle::operator != (const Circle& c) const
{
	return !(*this == c);
}

Circle Circle::operator + (Float scale) const
{
	return Circle(center, radius + scale);
}

Circle Circle::operator - (Float scale) const
{
	return Circle(center, ZeroFit(radius - scale));
}

Circle Circle::operator + (const Vector3D& v) const
{
	return Circle(center + v, radius);
}

Circle Circle::operator - (const Vector3D& v) const
{
	return Circle(center - v, radius);
}

Circle Circle::operator * (Float scale) const
{
	return Circle(center, radius * AbsVal(scale));
}

Circle Circle::operator / (Float scale) const
{
	Float invScale = (scale == 0.0) ? 0.0 : 1.0/scale;
	return Circle(center, radius * AbsVal(invScale));
}

Circle& Circle::operator = (const Circle& c)
{
	SetValues(c.center, c.radius);
	return *this;
}

Circle& Circle::operator += (Float scale)
{
	SetValues(center, radius + scale);
	return *this;
}

Circle& Circle::operator -= (Float scale)
{
	SetValues(center, ZeroFit(radius - scale));
	return *this;
}

Circle& Circle::operator += (const Vector3D& v)
{
	SetValues(center + v, radius);
	return *this;
}

Circle& Circle::operator -= (const Vector3D& v)
{
	SetValues(center - v, radius);
	return *this;
}

Circle& Circle::operator *= (Float scale)
{
	SetValues(center, radius * AbsVal(scale));
	return *this;
}

Circle& Circle::operator /= (Float scale)
{
	Float invScale = (scale == 0.0) ? 0.0 : 1.0/scale;
	SetValues(center, radius * AbsVal(invScale));
	return *this;
}

void Circle::SetValues(const Point3D& center, Float radius)
{
	this->center = center;
	this->radius = radius;
}

void Circle::GetValues(Point3D& center, Float& radius) const
{
	center = this->center;
	radius = this->radius;
}

Point3D Circle::GetRandomPoint() const
{
	Float angle = RandomRange(0, 360);
	Float distance = RandomRange(0, radius);

	return Point3D(center.GetX() + (CosD(angle) * distance),
				   center.GetY() + (SinD(angle) * distance),
				   0);
}

bool Circle::ContainsPoint(const Point3D& point) const
{
	return (CalculateDistance(point, center) <= radius);
}

void Circle::Translate(const Vector3D& v)
{
	*this += v;
}

Shape * Circle::CreateInstance() const
{
	return new Circle(center, radius);
}