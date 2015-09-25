#ifndef INC_GRID_H
#define INC_GRID_H

#include "type.h"
#include "point.h"
#include "tower.h"

#define GRID_SIZE 20

struct Grid
{
	public:
		Tower ** GetData() { return &data[0][0]; }
		Tower * GetData(const Point2D& coords) const { return data[(uint32)coords.GetX()][(uint32)coords.GetY()]; }

		bool GetPathState() const { return pathState; }
		uint8 * GetPath() { return &path[0][0]; }
		uint8 GetPath(const Point2D& coords) const { return path[(uint32)coords.GetX()][(uint32)coords.GetY()]; }
		void SetPath(const Point2D& coords, uint8 value) { path[(uint32)coords.GetX()][(uint32)coords.GetY()] = value; }

		void FillPath();
		void ClearPath(uint8 value);

		bool FindPath(const Point2D& start, const Point2D& end = NullPoint, const Point2D& toggle = NullPoint);
	private:
		void TracePath(const uint8 * pathMap, const Point2D& coords);

		bool pathState;
		Tower * data[GRID_SIZE][GRID_SIZE];
		uint8 path[GRID_SIZE][GRID_SIZE];
};

#endif