#include <algorithm>
#include <vector>
#include "type.h"
#include "point.h"
#include "rect.h"
#include "mouse.h"
#include "mousehandler.h"
#include "tower.h"
#include "selection.h"
#include "game.h"

Point2D doubleClickPoint;

bool MouseNamespace::BuildTower(uint8 button, const Point2D& position)
{
	Rect bounds(113, 113, 685, 685);
	if ((WorldGame.GetBuild() != NULL) && (bounds.ContainsPoint(WorldMouse.GetPosition()) == true))
	{
		uint32 x = (WorldMouse.GetX() - ((uint32)(WorldMouse.GetX() - 16) % 32) - 80)/32;
		uint32 y = (WorldMouse.GetY() - ((uint32)(WorldMouse.GetY() - 16) % 32) - 80)/32;
		
		return BuildTower(WorldGame.GetBuild(), Point2D(x, y), true);
	}
	return false;
}

bool MouseNamespace::ToggleSelectTower(uint8 button, const Point2D& position)
{
	if (WorldGame.GetBuild() == 0)
	{
		Tower ** gridData = WorldGame.GetGridData();
		Selection * selection = WorldGame.GetSelection();

		for (uint8 i = 1; i < GRID_SIZE - 1; i++)
		{
			for (uint8 j = 1; j < GRID_SIZE - 1; j++)
			{
				Tower * tower = gridData[(i*GRID_SIZE)+j];

				if ((tower != NULL) && (tower->GetState() == true))
				{
					uint32 x = (i * 32) + 80;
					uint32 y = (j * 32) + 80;
					Rect towerArea(x, y, x + 32, y + 32);

					if (towerArea.ContainsPoint(WorldMouse.GetPosition()) == true)
					{
						if (selection->GetIndex(tower) == NOT_FOUND)
						{
							return selection->Add(tower);
						}
						else
						{
							return selection->Remove(tower);
						}
					}
				}
			}
		}
	}

	return false;
}

bool DistanceSort(const Tower * t1, const Tower * t2)
{
	return (CalculateDistance(t1->GetPosition(), doubleClickPoint) <
			CalculateDistance(t2->GetPosition(), doubleClickPoint));
}

bool DoubleClickSelect(const Tower * tower)
{
	Tower ** gridData = WorldGame.GetGridData();
	Selection * selection = WorldGame.GetSelection();

	if (tower != NULL)
	{
		doubleClickPoint = tower->GetPosition();

		std::vector<Tower *> nearbyTowers;
		for (uint8 i = 1; i < GRID_SIZE - 1; i++)
		{
			for (uint8 j = 1; j < GRID_SIZE - 1; j++)
			{
				Tower * nearbyTower = gridData[(i*GRID_SIZE)+j];

				if ((nearbyTower != NULL) && (nearbyTower->GetState() == true) && (nearbyTower->GetTowerType() == tower->GetTowerType()))
				{
					nearbyTowers.push_back(nearbyTower);
				}
			}
		}
		sort(nearbyTowers.begin(), nearbyTowers.end(), DistanceSort);

		for (uint8 i = 0; (i < nearbyTowers.size()) && (selection->GetSize() < SELECTION_SIZE); i++)
		{
			selection->Add(nearbyTowers[i]);
		}
		return true;
	}
	return false;
}

bool MouseNamespace::NewSelectTower(uint8 button, const Point2D& position)
{
	if (WorldGame.GetBuild() == 0)
	{
		Tower ** gridData = WorldGame.GetGridData();
		Selection * selection = WorldGame.GetSelection();

		for (uint8 i = 1; i < GRID_SIZE - 1; i++)
		{
			for (uint8 j = 1; j < GRID_SIZE - 1; j++)
			{
				Tower * tower = gridData[(i*GRID_SIZE)+j];

				if ((tower != NULL) && (tower->GetState() == true))
				{
					uint32 x = (i * 32) + 80;
					uint32 y = (j * 32) + 80;
					Rect towerArea(x, y, x + 32, y + 32);

					if (towerArea.ContainsPoint(WorldMouse.GetPosition()) == true)
					{
						selection->Clear();
						if (WorldMouse.CheckDoubleClick(BUTTON_LEFT) == true)
						{
							return DoubleClickSelect(tower);
						}
						else
						{
							return selection->Add(tower);
						}
					}
				}
			}
		}
	}

	return false;
}

bool MouseNamespace::ToggleInfoboxSelection(uint8 button, const Point2D& position)
{
	Selection * selection = WorldGame.GetSelection();

	if (selection->GetSize() > 1)
	{
		for (uint32 i = 0; i < selection->GetSize(); i++)
		{
			Tower * tower = selection->GetTower(i);
			Rect bounds(792 + ((i % 4) * 54), 561 + ((i/4) * 57), 840 + ((i % 4) * 54), 609 + ((i/4) * 57));

			if ((tower != NULL) && (bounds.ContainsPoint(WorldMouse.GetPosition()) == true))
			{
				return selection->Remove(i);
			}
		}
	}
	return false;
}

bool MouseNamespace::NewInfoboxSelection(uint8 button, const Point2D& position)
{
	Selection * selection = WorldGame.GetSelection();

	if (selection->GetSize() > 1)
	{
		for (uint32 i = 0; i < selection->GetSize(); i++)
		{
			Tower * tower = selection->GetTower(i);
			Rect bounds(792 + ((i % 4) * 54), 561 + ((i/4) * 57), 840 + ((i % 4) * 54), 609 + ((i/4) * 57));

			if ((tower != NULL) && (tower->GetState() == true) && (bounds.ContainsPoint(WorldMouse.GetPosition()) == true))
			{
				selection->Clear();
				return selection->Add(tower);
			}
		}
	}
	return false;
}

bool MouseNamespace::BuildNoAction(uint8 button, const Point2D& position)
{
	return (WorldGame.GetBuild() != 0);
}

bool MouseNamespace::ToggleTowerWait(uint8 button, const Point2D& position)
{
	Selection * selection = WorldGame.GetSelection();
	if (selection->GetSize() > 0)
	{
		for (uint32 i = 0; i < selection->GetSize(); i++)
		{
			Tower * tower = selection->GetTower(i);
			if ((tower != NULL) && (tower->GetState() == true) && (tower->GetInt32Value(TOWER_VAR_CAN_FIRE) == true) && (tower->GetActiveSpell() != NULL))
			{
				tower->SetUInt32Value(TOWER_VAR_WAITING, 1 - tower->GetUInt32Value(TOWER_VAR_WAITING));
			}
		}
		return true;
	}
	return false;
}