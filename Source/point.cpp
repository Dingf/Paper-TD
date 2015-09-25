#include "type.h"
#include "math.h"
#include "point.h"
#include "vector.h"
#include "rect.h"

Point3D Origin(0, 0, 0);
Point3D NullPoint(0, 0, 0);

Point2D::Point2D()
{
	SetValues(0, 0);
}

Point2D::Point2D(Float u, Float v)
{
	SetValues(u, v);
}

Point2D::Point2D(const Point2D& p)
{
	*this = p;
}

Point2D::operator Vector3D() const
{
	return Vector3D(u, v, 0.0);
}

Point2D::operator Point3D() const
{
	return Point3D(u, v, 0.0);
}

bool Point2D::operator == (const Point2D& p) const
{
	return ((Smooth(u) == Smooth(p.u)) && (Smooth(v) == Smooth(p.v)));
}

bool Point2D::operator != (const Point2D& p) const
{
	return !(*this == p);
}

Point2D Point2D::operator ! () const
{
	return Point2D(-u, -v);
}

Point2D Point2D::operator + (const Point2D& p) const
{
	return Point2D(u + p.u, v + p.v);
}

Point2D Point2D::operator - (const Point2D& p) const
{
	return Point2D(u - p.u, v - p.v);
}

Point2D Point2D::operator * (Float scale) const
{
	return Point2D(u * scale, v * scale);
}

Point2D Point2D::operator / (Float scale) const
{
	Float invScale = (scale == 0.0) ? 0.0 : 1.0/scale;
	return Point2D(u * invScale, v * invScale);
}

Point2D& Point2D::operator = (const Point2D& p)
{
	SetValues(p.u, p.v);
	return *this;
}

Point2D& Point2D::operator += (const Point2D& p)
{
	SetValues(u + p.u, v + p.v);
	return *this;
}

Point2D& Point2D::operator -= (const Point2D& p)
{
	SetValues(u - p.u, v - p.v);
	return *this;
}

Point2D& Point2D::operator *= (Float scale)
{
	SetValues(u * scale, v * scale);
	return *this;
}

Point2D& Point2D::operator /= (Float scale)
{
	Float invScale = (scale == 0.0) ? 0.0 : 1.0/scale;
	SetValues(u * invScale, v * invScale);
	return *this;
}

void Point2D::SetValues(Float u, Float v)
{
	this->u = u;
	this->v = v;
}

void Point2D::GetValues(Float& u, Float& v) const
{
	u = this->u;
	v = this->v;
}

//bool Point2D::Intersect(const Rect& r)
//{
//	Point3D p = (Point3D)(*this);
//	return p.Intersect(r);
//}
//
//bool Point2D::Intersect(const Circle& c)
//{
//	Point3D p = (Point3D)(*this);
//	return p.Intersect(c);
//}



Point3D::Point3D()
{
	SetValues(0, 0, 0);
}

Point3D::Point3D(Float x, Float y, Float z)
{
	SetValues(x, y, z);
}

Point3D::Point3D(const Point3D& p)
{
	*this = p;
}

Point3D::operator Vector3D() const
{
	return Vector3D(x, y, z);
}

Point3D::operator Point2D() const
{
	return Point2D(x, y);
}

bool Point3D::operator == (const Point3D& p) const
{
	return ((Smooth(x) == Smooth(p.x)) && (Smooth(y) == Smooth(p.y)) && (Smooth(z) == Smooth(p.z)));
}

bool Point3D::operator != (const Point3D& p) const
{
	return !(*this == p);
}

Point3D Point3D::operator ! () const
{
	return Point3D(-x, -y, -z);
}

Point3D Point3D::operator + (const Point3D& p) const
{
	return Point3D(x + p.x, y + p.y, z + p.z);
}

Point3D Point3D::operator - (const Point3D& p) const
{
	return Point3D(x - p.x, y - p.y, z - p.z);
}

Point3D Point3D::operator * (Float scale) const
{
	return Point3D(x * scale, y * scale, z * scale);
}

Point3D Point3D::operator / (Float scale) const
{
	Float invScale = (scale == 0.0) ? 0.0 : 1.0/scale;
	return Point3D(x * invScale, y * invScale, z * invScale);
}	

Point3D& Point3D::operator = (const Point3D& p)
{
	SetValues(p.x, p.y, p.z);
	return *this;
}

Point3D& Point3D::operator += (const Point3D& p)
{
	SetValues(x + p.x, y + p.y, z + p.z);
	return *this;
}

Point3D& Point3D::operator -= (const Point3D& p)
{
	SetValues(x - p.x, y - p.y, z - p.z);
	return *this;
}

Point3D& Point3D::operator *= (Float scale)
{
	SetValues(x * scale, y * scale, z * scale);
	return *this;
}

Point3D& Point3D::operator /= (Float scale)
{
	Float invScale = (scale == 0) ? 0.0 : 1.0/scale;
	SetValues(x * invScale, y * invScale, z * invScale);
	return *this;
}

void Point3D::SetValues(Float x, Float y)
{
	this->x = x;
	this->y = y;
	this->z = 0;
}

void Point3D::SetValues(Float x, Float y, Float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Point3D::GetValues(Float& x, Float& y, Float& z) const
{
	x = this->x;
	y = this->y;
	z = this->z;
}

//bool Point3D::Intersect(const Rect& r)
//{
//	return ((GetX() >= r.GetLeft()) && (GetX() <= r.GetRight()) && (GetY() >= r.GetTop()) && (GetY() <= r.GetBottom()));
//}
//
//bool Point3D::Intersect(const Circle& c)
//{
//	return (CalculateDistance(*this, c.GetCenter()) <= c.GetRadius());
//}

Float CalculateDistance(const Point3D& point1, const Point3D& point2)
{
	Float dx = point1.GetX() - point2.GetX();
	Float dy = point1.GetY() - point2.GetY();
	Float dz = point1.GetZ() - point2.GetZ();

	return Root2((dx * dx) + (dy * dy) + (dz * dz));
}

Float CalculateSlope(const Point3D& point1, const Point3D& point2)
{
	if ((point2.GetX() - point1.GetX()) != 0)
	{
		return (point2.GetY() - point1.GetY())/(point2.GetX() - point1.GetX());
	}
	return NULL;
}