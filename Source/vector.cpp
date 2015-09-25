#include "type.h"
#include "math.h"
#include "point.h"
#include "vector.h"

Vector3D::Vector3D()
{
	SetValues(0, 0, 0);
}

Vector3D::Vector3D(Float x, Float y, Float z)
{
	SetValues(x, y, z);
}

Vector3D::Vector3D(Point3D p1, Point3D p2)
{
	SetValues(p1, p2);
}

Vector3D::Vector3D(const Vector3D& v)
{
	*this = v;
}

Vector3D::operator Point3D() const
{
	return Point3D(x, y, z);
}

Vector3D::operator Point2D() const
{
	return Point2D(x, y);
}

bool Vector3D::operator == (const Vector3D& v) const
{
	return ((Smooth(x) == Smooth(v.x)) && (Smooth(y) == Smooth(v.y)) && (Smooth(z) == Smooth(v.z)));
}

bool Vector3D::operator != (const Vector3D& v) const
{
	return !(*this == v);
}

Vector3D Vector3D::operator ! () const
{
	return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::operator + (const Vector3D& v) const
{
	return Vector3D(x + v.x, y + v.y, z + v.z);
}

Vector3D Vector3D::operator - (const Vector3D& v) const
{
	return Vector3D(x - v.x, y - v.y, z - v.z);
}

Vector3D Vector3D::operator * (Float scale) const
{
	return Vector3D(x * scale, y * scale, z * scale);
}

Vector3D Vector3D::operator / (Float scale) const
{
	Float invScale = (scale == 0.0) ? 0.0 : 1.0/scale;
	return Vector3D(x * invScale, y * invScale, z * invScale);
}

Vector3D& Vector3D::operator = (const Vector3D& v)
{
	SetValues(v.x, v.y, v.z);
	return *this;
}

Vector3D& Vector3D::operator += (const Vector3D& v)
{
	SetValues(x + v.x, y + v.y, z + v.z);
	return *this;
}

Vector3D& Vector3D::operator -= (const Vector3D& v)
{
	SetValues(x - v.x, y - v.y, z - v.z);
	return *this;
}

Vector3D& Vector3D::operator *= (Float scale)
{
	SetValues(x * scale, y * scale, z * scale);
	return *this;
}

Vector3D& Vector3D::operator /= (Float scale)
{
	Float invScale = (scale == 0.0) ? 0.0 : 1.0/scale;
	SetValues(x * invScale, y * invScale, z * invScale);
	return *this;
}

bool Vector3D::IsZero() const
{
	return (GetLength() == 0);
}

Vector3D Vector3D::CrossProduct(const Vector3D& v) const
{
	return Vector3D((y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x)); 
}

Float Vector3D::DotProduct(const Vector3D& v) const
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

Float Vector3D::GetLength() const
{
	return Root2(DotProduct(*this));
}

Float Vector3D::GetZeroAngleD() const
{
	if (GetLength() != 0)
	{
		Float baseAngle = ArcCosD(AbsVal(x)/GetLength());

		if ((x < 0) && (y >= 0))
		{
			baseAngle += 180;
		}
		else if ((x < 0) && (y < 0))
		{
			baseAngle = 180 - baseAngle;
		}
		else if ((x >= 0) && (y >= 0))
		{
			baseAngle = 360 - baseAngle;
		}

		return fmod(baseAngle, 360);
	}
	return 0;
}

Vector3D Vector3D::GetUnitVector() const
{
	return *this/GetLength();
}

void Vector3D::SetUnitVector()
{
	*this /= GetLength();
}
/*
Vector3D Vector3D::RotateD(Float angle) const
{
	if (IsZero() == false)
	{
		return Vector3D((x * CosD(angle)) - (y * SinD(angle)) + (z * SinD(angle)),
						(x * SinD(angle)) + (y * CosD(angle)) - (z * SinD(angle)),
						(-x * SinD(angle)) + (y * SinD(angle)) + (z * CosD(angle)));
	}
	return Vector3D(0, 0, 0);
}*/

void Vector3D::SetValues(Float x, Float y, Float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3D::SetValues(Point3D p1, Point3D p2)
{
	x = p2.GetX() - p1.GetX();
	y = p2.GetY() - p1.GetY();
	z = p2.GetZ() - p1.GetZ();
}

void Vector3D::GetValues(Float& x, Float& y, Float& z) const
{
	x = this->x;
	y = this->y;
	z = this->z;
}