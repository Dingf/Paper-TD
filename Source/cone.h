#ifndef INC_CONE_H
#define INC_CONE_H

#include "type.h"
#include "point.h"
#include "vector.h"
#include "shape.h"

struct Cone : public Shape
{
	public:
		Cone();
		Cone(const Point3D& vertex, const Vector3D& direction, Float height, Float angle);

		Float GetHeight() const { return height; }
		Float GetAngle() const { return angle; }

			  Point3D& GetVertex()       { return vertex; }
		const Point3D& GetVertex() const { return vertex; }

			  Vector3D& GetDirection()		 { return direction; }
		const Vector3D& GetDirection() const { return direction; }

		void SetValues(const Point3D& vertex, const Vector3D& direction, Float height, Float angle);

		Point3D GetRandomPoint() const;
		bool ContainsPoint(const Point3D& point) const;
		void Translate(const Vector3D& v);

		Shape * CreateInstance() const;

	private:
		Float height;
		Float angle;
		Point3D vertex;
		Vector3D direction;
};

#endif