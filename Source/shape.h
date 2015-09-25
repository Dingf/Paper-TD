#ifndef INC_SHAPE_H
#define INC_SHAPE_H

#include "type.h"
#include "rand.h"
#include "point.h"
#include "vector.h"

enum ShapeTypes
{
	SHAPE_TYPE_NONE,
	SHAPE_TYPE_LINE,
	SHAPE_TYPE_LINE_SEGMENT,
	SHAPE_TYPE_RECT,
	SHAPE_TYPE_CIRCLE,
	SHAPE_TYPE_CONE,
};

class Shape
{
	public:
		Shape();

		void SetType(uint8 type) { this->type = type; }

		uint8 GetType() const { return type; }

		virtual Point3D GetRandomPoint() const = 0;
		virtual bool ContainsPoint(const Point3D& point) const = 0;

		virtual void Translate(const Vector3D& v) = 0;

		virtual Shape * CreateInstance() const = 0;
	private:
		uint8 type;
};

bool Intersect(const Shape * shape1, const Shape * shape2);

#endif