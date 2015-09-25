#include "type.h"
#include "math.h"
#include "utility.h"
#include "point.h"
#include "shape.h"
#include "shapes.h"

Rect::Rect()
{
	SetType(SHAPE_TYPE_RECT);
	SetValues(0, 0, 0, 0);
}
Rect::Rect(Float left, Float top, Float right, Float bottom)
{
	SetType(SHAPE_TYPE_RECT);
	SetValues(left, top, right, bottom);
}

Rect::Rect(const Point2D& topLeft, const Point2D& bottomRight)
{
	SetType(SHAPE_TYPE_RECT);
	SetValues(topLeft.GetX(), topLeft.GetY(), bottomRight.GetX(), bottomRight.GetY());
}

Rect::Rect(const Rect& r)
{
	SetType(SHAPE_TYPE_RECT);
	*this = r;
}

bool Rect::operator == (const Rect& r) const
{
	return ((Smooth(left) == Smooth(r.left)) && (Smooth(top) == Smooth(r.top)) && (Smooth(right) == Smooth(r.right)) && (Smooth(bottom) == Smooth(r.bottom)));
}

bool Rect::operator != (const Rect& r) const
{
	return !(*this == r);
}

Rect Rect::operator + (Float scale) const
{
	return Rect(left - scale, top - scale, right + scale, bottom + scale);
}

Rect Rect::operator - (Float scale) const
{
	return Rect(left + scale, top + scale, right - scale, bottom - scale);
}

Rect Rect::operator + (const Vector3D& v) const
{
	return Rect(left + v.GetX(), top + v.GetY(), right + v.GetX(), bottom + v.GetY());
}

Rect Rect::operator - (const Vector3D& v) const
{
	return Rect(left - v.GetX(), top - v.GetY(), right - v.GetX(), bottom - v.GetY());
}

Rect Rect::operator * (Float scale) const
{
	return Rect(left * scale, top * scale, right * scale, bottom * scale);
}

Rect Rect::operator / (Float scale) const
{
	Float invScale = (scale == 0.0) ? 0.0 : 1.0/scale;
	return Rect(left * invScale, top * invScale, right * invScale, bottom * invScale);
}

Rect& Rect::operator = (const Rect& r)
{
	SetValues(r.left, r.top, r.right, r.bottom);
	return *this;
}

Rect& Rect::operator += (Float scale)
{
	SetValues(left - scale, top - scale, right + scale, bottom + scale);
	return *this;
}

Rect& Rect::operator -= (Float scale)
{
	SetValues(left + scale, top + scale, right - scale, bottom - scale);
	return *this;
}

Rect& Rect::operator += (const Vector3D& v)
{
	SetValues(left + v.GetX(), top + v.GetY(), right + v.GetX(), bottom + v.GetY());
	return *this;
}

Rect& Rect::operator -= (const Vector3D& v)
{
	SetValues(left - v.GetX(), top - v.GetY(), right - v.GetX(), bottom - v.GetY());
	return *this;
}

Rect& Rect::operator *= (Float scale)
{
	SetValues(left * scale, top * scale, right * scale, bottom * scale);
	return *this;
}

Rect& Rect::operator /= (Float scale)
{
	Float invScale = (scale == 0.0) ? 0.0 : 1.0/scale;

	SetValues(left * invScale, top * invScale, right * invScale, bottom * invScale);
	return *this;
}

void Rect::SetValues(Float left, Float top, Float right, Float bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	FitRect();
}

void Rect::GetValues(Float& left, Float& top, Float& right, Float& bottom) const
{
	left = this->left;
	top = this->top;
	right = this->right;
	bottom = this->bottom;
}

void Rect::FitRect()
{
	if (left > right)
	{
		Swap(left, right);
	}
	if (top > bottom)
	{
		Swap(top, bottom);
	}
}

Point3D Rect::GetRandomPoint() const
{
	return Point3D(left + RandomRange(0, GetWidth()),
				   top + RandomRange(0, GetHeight()),
				   0);
}

bool Rect::ContainsPoint(const Point3D& point) const
{
	return ((point.GetX() >= left) && (point.GetX() <= right) && (point.GetY() >= top) && (point.GetY() <= bottom));
}

void Rect::Translate(const Vector3D& v)
{
	*this += v;
}

Shape * Rect::CreateInstance() const
{
	return new Rect(left, top, right, bottom);
}