#include "type.h"
#include "point.h"
#include "tower.h"
#include "grid.h"
#include "utility.h"
#include "rendergame.h"

void Grid::FillPath()
{
	uint32 filled = 0;

	while (true)
	{
		for (uint8 i = 1; i < GRID_SIZE - 1; i++)
		{
			for (uint8 j = 1; j < GRID_SIZE - 1; j++)
			{
				if (path[i][j] == false)
				{
					uint8 counter = 0;
					for (uint8 k = i - 1; k <= i + 1; k++)
					{
						for (uint8 l = j - 1; l <= j + 1; l++)
						{
							uint32 index = (k * GRID_SIZE) + l;

							if (((k == i) || (l == j)) && (path[i][j] == true))
							{
								counter++;
							}
						}
					}

					if (counter >= 3)
					{
						path[i][j] = true;
						filled++;
					}
				}
			}
		}

		if (filled == 0)
		{
			break;
		}
		else
		{
			filled = 0;
		}
	}
}

void Grid::ClearPath(uint8 value)
{
	for (uint8 i = 0; i < GRID_SIZE; i++)
	{
		for (uint8 j = 0; j < GRID_SIZE; j++)
		{
			path[i][j] = value;
			if ((data[i][j] != NULL) && (data[i][j]->GetState() == true) && (data[i][j]->GetInt32Value(TOWER_VAR_GHOST_STATE) == false))
			{
				path[i][j] = 1;
			}
		}
	}
	pathState = false;
}

void Grid::TracePath(const uint8 * pathMap, const Point2D& coords)
{
	uint32 x = coords.GetX();
	uint32 y = coords.GetY();

	switch (pathMap[(x*GRID_SIZE)+y])
	{
		case 2:
		{
			TracePath(pathMap, Point2D(x, y - 1));
			break;
		}
		case 3:
		{
			TracePath(pathMap, Point2D(x + 1, y));
			break;
		}
		case 4:
		{
			TracePath(pathMap, Point2D(x, y + 1));
			break;
		}
		case 5:
		{
			TracePath(pathMap, Point2D(x - 1, y));
			break;
		}
	}

	path[x][y] = pathMap[(x*GRID_SIZE)+y];
}

bool Grid::FindPath(const Point2D& start, const Point2D& end, const Point2D& toggle)
{
	uint8 newMap[GRID_SIZE][GRID_SIZE];
	uint8 pathMap[GRID_SIZE][GRID_SIZE];

	Point2D starts[2] = { start, NullPoint };
	Point2D ends[2] = { end, NullPoint };
	
	if (start == NullPoint)
	{
		starts[0] = Point2D(GRID_SIZE/2, 1);
		starts[1] = Point2D(GRID_SIZE/2 - 1, 1);
	}
	if (end == NullPoint)
	{
		ends[0] = Point2D(GRID_SIZE/2, GRID_SIZE - 1);
		ends[1] = Point2D(GRID_SIZE/2 - 1, GRID_SIZE - 1);
	}

	ClearPath(0);

	if (toggle != NullPoint)
	{
		SetPath(toggle, 1);
	}

	FillPath();
	for (uint8 i = 0; i < GRID_SIZE; i++)
	{
		for (uint8 j = 0; j < GRID_SIZE; j++)
		{
			pathMap[i][j] = GetPath(Point2D(i, j));
			newMap[i][j] = pathMap[i][j];
		}
	}

	//9,18 + 10,18
	for (uint8 i = 0; i < 2; i++)
	{
		if (ends[i] != NullPoint)
		{
			uint8 x = ends[i].GetX();
			uint8 y = ends[i].GetY();

			pathMap[x][y] = (pathMap[x][y] == 0) ? 6 : pathMap[x][y];
		}
	}

	while (true)
	{
		uint8 counter = 0;
		for (uint8 i = 1; i < GRID_SIZE - 1; i++)
		{
			for (uint8 j = 1; j < GRID_SIZE - 1; j++)
			{
				if (pathMap[i][j] == 0)
				{
					if (pathMap[i][j-1] > 1)
					{
						newMap[i][j] = 2;
						counter++;
					}
					else if (pathMap[i+1][j] > 1)
					{
						newMap[i][j] = 3;
						counter++;
					}
					else if (pathMap[i][j+1] > 1)
					{
						newMap[i][j] = 4;
						counter++;
					}
					else if (pathMap[i-1][j] > 1)
					{
						newMap[i][j] = 5;
						counter++;
					}
				}
			}
		}

		for (uint8 i = 0; i < GRID_SIZE; i++)
		{
			for (uint8 j = 0; j < GRID_SIZE; j++)
			{
				if (pathMap[i][j] == 0)
				{
					pathMap[i][j] = newMap[i][j];
				}
			}
		}

		for (uint8 i = 0; i < 2; i++)
		{
			if (starts[i] != NullPoint)
			{
				uint8 x = starts[i].GetX();
				uint8 y = starts[i].GetY();

				if (pathMap[x][y] > 1)
				{
					TracePath((uint8 *)pathMap, starts[i]);
					pathState = true;
				}
			}
		}

		if (pathState == true)
		{
			return true;
		}
		
		if (counter == 0)
		{
			break;
		}
	}

	return false;
}

