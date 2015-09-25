#include "type.h"
#include "math.h"
#include "utility.h"
#include "point.h"
#include "shapes.h"

bool LineLineIntersect(const Line * line1, const Line * line2)
{
	return ((line1->GetSlope() != line2->GetSlope()) || (line1->GetPoint1() != line2->GetPoint1()));
}

bool LineLineSegIntersect(const Line * line1, const LineSegment * line2)
{
	Point3D point1 = line2->GetPoint1();
	Point3D point2 = line2->GetPoint2();

	Float innerLeft = point1.GetX();
	Float innerRight = point2.GetX();

	return ((line2->GetValueAt(innerLeft) >= line1->GetValueAt(innerLeft)) == (line2->GetValueAt(innerRight) >= line1->GetValueAt(innerRight)));
}

bool LineSegLineSegIntersect(const LineSegment * line1, const LineSegment * line2)
{
	Point3D point1 = line1->GetPoint1();
	Point3D point2 = line2->GetPoint1();
	Point3D point3 = line1->GetPoint2();
	Point3D point4 = line2->GetPoint2();

	Float innerLeft = GetMax(point1.GetX(), point2.GetX());
	Float innerRight = GetMin(point3.GetX(), point4.GetX());

	return ((line1->GetValueAt(innerLeft) >= line2->GetValueAt(innerLeft)) == (line1->GetValueAt(innerRight) >= line2->GetValueAt(innerRight)));
}

bool LineRectIntersect(const Line * line, const Rect * rect)
{
	Point3D point1 = line->GetPoint1();
	Point3D point2 = line->GetPoint2();

	if ((rect->ContainsPoint(point1) == true) || (rect->ContainsPoint(point2) == true))
	{
		return true;
	}
	if (point1.GetX() == point2.GetX())
	{
		return ((rect->GetLeft() <= point1.GetX()) && (rect->GetRight() >= point1.GetX()));
	}

	Point2D corners[4] = 
	{
		Point2D(rect->GetLeft(),	rect->GetTop()),
		Point2D(rect->GetLeft(),	rect->GetBottom()),
		Point2D(rect->GetRight(),	rect->GetTop()),
		Point2D(rect->GetRight(),	rect->GetBottom()),
	};

	int8 counter = 0;
	for (uint8 i = 0; i < 4; i++)
	{
		counter += (corners[i].GetY() > line->GetValueAt(corners[i].GetX())) ? 1 : -1;
	}

	return (AbsVal(counter) < 4);
}

bool LineSegRectIntersect(const LineSegment * line, const Rect * rect)
{
	Point3D point1 = line->GetPoint1();
	Point3D point2 = line->GetPoint2();

	if ((rect->ContainsPoint(point1) == true) || (rect->ContainsPoint(point2) == true))
	{
		return true;
	}
	if (((point1.GetX() > rect->GetRight()) && (point2.GetX() > rect->GetRight())) ||
		((point1.GetX() < rect->GetLeft()) && (point2.GetX() < rect->GetLeft())) ||
		((point1.GetY() > rect->GetBottom()) && (point2.GetY() > rect->GetBottom())) ||
		((point1.GetY() < rect->GetTop()) && (point2.GetY() < rect->GetTop())))
	{
		return false;
	}
	if (point1.GetX() == point2.GetX())
	{
		return ((rect->GetLeft() <= point1.GetX()) && (rect->GetRight() >= point1.GetX()));
	}

	Point2D corners[4] = 
	{
		Point2D(rect->GetLeft(),	rect->GetTop()),
		Point2D(rect->GetLeft(),	rect->GetBottom()),
		Point2D(rect->GetRight(),	rect->GetTop()),
		Point2D(rect->GetRight(),	rect->GetBottom()),
	};

	int8 counter = 0;
	for (uint8 i = 0; i < 4; i++)
	{
		counter += (corners[i].GetY() > line->GetValueAt(corners[i].GetX())) ? 1 : -1;
	}

	return (AbsVal(counter) < 4);
}

bool RectCheck(const Rect * target, const Rect * source)
{
	Point2D corners[4] = 
	{
		Point2D(target->GetLeft(),	target->GetTop()),
		Point2D(target->GetLeft(),	target->GetBottom()),
		Point2D(target->GetRight(),	target->GetTop()),
		Point2D(target->GetRight(),	target->GetBottom()),
	};

	for (uint8 i = 0; i < 4; i++)
	{
		if (source->ContainsPoint(corners[i]) == true)
		{
			return true;
		}
	}
	return false;
}

bool RectRectIntersect(const Rect * rect1, const Rect * rect2)
{
	if (RectCheck(rect1, rect2) == false)
	{
		return RectCheck(rect2, rect1);
	}
	return true;
}

bool LineCircleIntersect(const Line * line, const Circle * circle)
{
	if ((circle->ContainsPoint(line->GetPoint1()) == true) || (circle->ContainsPoint(line->GetPoint2()) == true))
	{
		return true;
	}

	Vector3D v1 = line->GetPoint1();
	Vector3D v2 = line->GetPoint2();
	Vector3D v3 = circle->GetCenter();

	Float r = circle->GetRadius();
	Float a = (v2 - v1).DotProduct(v2 - v1);
	Float b = 2 * (v2 - v1).DotProduct(v1 - v3);
	Float c = v3.DotProduct(v3) + v1.DotProduct(v1) - (2 * v3.DotProduct(v1)) - (r * r);

	Float determinant = (b * b) - (4 * a * c);
	if (determinant >= 0)
	{	
		Float u1 = (-b + Root2(determinant))/(2 * a);
		Float x1 = Smooth(v1.GetX() + (u1 * (v2.GetX() - v1.GetX())));
		Float u2 = (-b - Root2(determinant))/(2 * a);
		Float x2 = Smooth(v1.GetX() + (u2 * (v2.GetX() - v1.GetX())));

		if ((IntervalCheck(x1, v1.GetX(), v2.GetX()) == true) || (IntervalCheck(x2, v1.GetX(), v2.GetX()) == true))
		{
			return true;
		}
	}

	return false;
}

bool LineSegCircleIntersect(const LineSegment * line, const Circle * circle)
{
	if ((circle->ContainsPoint(line->GetPoint1()) == true) || (circle->ContainsPoint(line->GetPoint2()) == true))
	{
		return true;
	}

	Vector3D v1 = line->GetPoint1();
	Vector3D v2 = line->GetPoint2();
	Vector3D v3 = circle->GetCenter();

	Float u = (v3 - v1).DotProduct(v2 - v1)/(v2 - v1).DotProduct(v2 - v1);
	
	if (IntervalCheck(u, 0, 1) == true)
	{
		return LineCircleIntersect((const Line *)line, circle);
	}

	return false;
}

bool RectCircleIntersect(const Rect * rect, const Circle * circle)
{
	if (rect->ContainsPoint(circle->GetCenter()) == true)
	{
		return true;
	}

	Point2D corners[4] = 
	{
		Point2D(rect->GetLeft(),	rect->GetTop()),
		Point2D(rect->GetLeft(),	rect->GetBottom()),
		Point2D(rect->GetRight(),	rect->GetBottom()),
		Point2D(rect->GetRight(),	rect->GetTop()),
	};
	LineSegment lines[4] = 
	{
		LineSegment(corners[0], corners[1]),
		LineSegment(corners[1], corners[2]),
		LineSegment(corners[2], corners[3]),
		LineSegment(corners[3], corners[0]),
	};

	for (uint8 i = 0; i < 4; i++)
	{
		if (Intersect(&lines[i], circle) == true)
		{
			return true;
		}
	}
	return false;
}

bool CircleCircleIntersect(const Circle * circle1, const Circle * circle2)
{
	return (CalculateDistance(circle1->GetCenter(), circle2->GetCenter()) < (circle1->GetRadius() + circle2->GetRadius()));
}

bool LineConeIntersect(const Line * line, const Cone * cone)
{
	Circle circle(cone->GetVertex(), cone->GetHeight());
	if (Intersect(line, &circle) == true)
	{
		if ((cone->ContainsPoint(line->GetPoint1()) == true) || (cone->ContainsPoint(line->GetPoint2()) == true) || (line->ContainsPoint(cone->GetVertex()) == true))
		{
			return true;
		}

		Vector3D direction(cone->GetVertex(), cone->GetVertex() + cone->GetDirection());
		Float angle = direction.GetZeroAngleD();

		Float angle1 = angle + cone->GetAngle();
		Float angle2 = angle - cone->GetAngle();

		Float height = cone->GetHeight();
		Point3D vertex = cone->GetVertex();

		LineSegment line1(vertex, Point3D(vertex.GetX() + (CosD(angle1) * height), vertex.GetY() + (SinD(angle1) * height), 0));
		LineSegment line2(vertex, Point3D(vertex.GetX() + (CosD(angle2) * height), vertex.GetY() + (SinD(angle2) * height), 0));

		return ((Intersect(line, &line1) == true) || (Intersect(line, &line2) == true));
	}
	return false;
}

bool LineSegConeIntersect(const LineSegment * line, const Cone * cone)
{
	Circle circle(cone->GetVertex(), cone->GetHeight());
	if (Intersect(line, &circle) == true)
	{
		if ((cone->ContainsPoint(line->GetPoint1()) == true) || (cone->ContainsPoint(line->GetPoint2()) == true) || (line->ContainsPoint(cone->GetVertex()) == true))
		{
			return true;
		}

		Vector3D direction(cone->GetVertex(), cone->GetVertex() + cone->GetDirection());
		Float angle = direction.GetZeroAngleD();

		Float angle1 = angle + cone->GetAngle();
		Float angle2 = angle - cone->GetAngle();

		Float height = cone->GetHeight();
		Point3D vertex = cone->GetVertex();

		LineSegment line1(vertex, Point3D(vertex.GetX() + (CosD(angle1) * height), vertex.GetY() + (SinD(angle1) * height), 0));
		LineSegment line2(vertex, Point3D(vertex.GetX() + (CosD(angle2) * height), vertex.GetY() + (SinD(angle2) * height), 0));

		return ((Intersect(line, &line1) == true) || (Intersect(line, &line2) == true));
	}
	return false;
}

bool RectConeIntersect(const Rect * rect, const Cone * cone)
{
	Circle circle(cone->GetVertex(), cone->GetHeight());
	if (Intersect(rect, &circle) == true)
	{
		Point2D corners[4] = 
		{
			Point2D(rect->GetLeft(),	rect->GetTop()),
			Point2D(rect->GetLeft(),	rect->GetBottom()),
			Point2D(rect->GetRight(),	rect->GetBottom()),
			Point2D(rect->GetRight(),	rect->GetTop()),
		};

		if (rect->ContainsPoint(cone->GetVertex()) == true)
		{
			return true;
		}
		for (uint8 i = 0; i < 4; i++)
		{
			if (cone->ContainsPoint(corners[i]) == true)
			{
				return true;
			}
		}

		Vector3D direction(cone->GetVertex(), cone->GetVertex() + cone->GetDirection());
		Float angle = direction.GetZeroAngleD();

		Float angle1 = angle + cone->GetAngle();
		Float angle2 = angle - cone->GetAngle();

		Float height = cone->GetHeight();
		Point3D vertex = cone->GetVertex();

		LineSegment line1(vertex, Point3D(vertex.GetX() + (CosD(angle1) * height), vertex.GetY() + (SinD(angle1) * height), 0));
		LineSegment line2(vertex, Point3D(vertex.GetX() + (CosD(angle2) * height), vertex.GetY() + (SinD(angle2) * height), 0));

		return ((Intersect(rect, &line1) == true) || (Intersect(rect, &line2) == true));
	}
	return false;
}

bool CircleConeIntersect(const Circle * c, const Cone * cone)
{
	Circle circle(cone->GetVertex(), cone->GetHeight());
	if (Intersect(c, &circle) == true)
	{
		if ((c->ContainsPoint(cone->GetVertex()) == true) || (cone->ContainsPoint(c->GetCenter()) == true))
		{
			return true;
		}


		Vector3D direction(cone->GetVertex(), cone->GetVertex() + cone->GetDirection());
		Vector3D centerDirection(cone->GetVertex(), c->GetCenter());
		Float angle = direction.GetZeroAngleD();
		Float angleDif = centerDirection.GetZeroAngleD() - angle;

		if (AbsVal(angleDif) < cone->GetAngle())
		{
			return true;
		}

		Float angle1 = angle + cone->GetAngle();
		Float angle2 = angle - cone->GetAngle();

		Float height = cone->GetHeight();
		Point3D vertex = cone->GetVertex();

		LineSegment line1(vertex, Point3D(vertex.GetX() + (CosD(angle1) * height), vertex.GetY() + (SinD(angle1) * height), 0));
		LineSegment line2(vertex, Point3D(vertex.GetX() + (CosD(angle2) * height), vertex.GetY() + (SinD(angle2) * height), 0));

		return ((Intersect(c, &line1) == true) || (Intersect(c, &line2) == true));
	}
	return false;
}

bool Intersect(const Shape * shape1, const Shape * shape2)
{
	/*
	LINE = 2,
	LINE_SEG = 4,
	RECT = 8,
	CIRCLE = 16,
	CONE = 32,
	*/
	if (shape1->GetType() > shape2->GetType())
	{
		Swap(shape1, shape2);
	}

	uint8 intersectID = (1 << shape1->GetType()) + (1 << shape2->GetType());
	switch (intersectID)
	{
		case 4:
		{
			return LineLineIntersect((const Line *)shape1, (const Line *)shape2);
		}
		case 6:
		{
			return LineLineSegIntersect((const Line *)shape1, (const LineSegment *)shape2);
		}
		case 8:
		{
			return LineSegLineSegIntersect((const LineSegment *)shape1, (const LineSegment *)shape2);
		}
		case 10:
		{
			return LineRectIntersect((const Line *)shape1, (const Rect *)shape2);
		}
		case 12:
		{
			return LineSegRectIntersect((const LineSegment *)shape1, (const Rect *)shape2);
		}
		case 16:
		{
			return RectRectIntersect((const Rect *)shape1, (const Rect *)shape2);
		}
		case 18:
		{
			return LineCircleIntersect((const Line *)shape1, (const Circle *)shape2);
		}
		case 20:
		{
			return LineSegCircleIntersect((const LineSegment *)shape1, (const Circle *)shape2);
		}
		case 24:
		{
			return RectCircleIntersect((const Rect *)shape1, (const Circle *)shape2);
		}
		case 32:
		{
			return CircleCircleIntersect((const Circle *)shape1, (const Circle *)shape2);
		}
		case 34:
		{
			return LineConeIntersect((const Line *)shape1, (const Cone *)shape2);
		}
		case 36:
		{
			return LineSegConeIntersect((const LineSegment *)shape1, (const Cone *)shape2);
		}
		case 40:
		{
			return RectConeIntersect((const Rect *)shape1, (const Cone *)shape2);
		}
		case 48:
		{
			return CircleConeIntersect((const Circle *)shape1, (const Cone *)shape2);
		}
		default:
		{
			return false;
		}
	}
}