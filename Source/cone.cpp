#include "type.h"
#include "math.h"
#include "point.h"
#include "vector.h"
#include "shapes.h"

Cone::Cone()
{
	SetType(SHAPE_TYPE_CONE);
	SetValues(Point3D(0, 0, 0), Vector3D(0, 0, 0), 0, 0);
}

Cone::Cone(const Point3D& vertex, const Vector3D& direction, Float height, Float angle)
{
	SetType(SHAPE_TYPE_CONE);
	SetValues(vertex, direction, height, angle);
}

void Cone::SetValues(const Point3D& vertex, const Vector3D& direction, Float height, Float angle)
{
	this->vertex = vertex;
	this->direction = direction;
	this->height = height;
	this->angle = angle;
}

Point3D Cone::GetRandomPoint() const
{
	Float randomAngle = RandomRange(direction.GetZeroAngleD() - angle, 
									direction.GetZeroAngleD() + angle);
	Float randomDistance = RandomRange(0, height);

	return Point3D(vertex.GetX() + (CosD(randomAngle) * randomDistance),
				   vertex.GetY() + (SinD(randomAngle) * randomDistance),
				   0);
}

bool Cone::ContainsPoint(const Point3D& point) const
{
	Circle circle(vertex, height);
	if (circle.ContainsPoint(point) == true)
	{
		Vector3D pointDirection(vertex, point);
		Vector3D coneDirection(vertex, direction);

		/*Float pointAngle = pointDirection.GetZeroAngleD();
		Float coneAngle = coneDirection.GetZeroAngleD();*/

		Float angleDif = pointDirection.GetZeroAngleD() - coneDirection.GetZeroAngleD();

		return ((AbsVal(angleDif) < angle) || (AbsVal(angleDif) > (360 - angle)));
	}
	return false;
}

void Cone::Translate(const Vector3D& v)
{
	vertex += v;
}

Shape * Cone::CreateInstance() const
{
	return new Cone(vertex, direction, height, angle);
}