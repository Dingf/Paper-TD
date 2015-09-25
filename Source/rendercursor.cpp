#include "type.h"
#include "point.h"
#include "sprite.h"
#include "cursor.h"
#include "tower.h"
#include "game.h"

Cursor GameCursor(&WorldMouse);

namespace RenderCursor
{
	Sprite Cross;
	Sprite Check;
	Sprite RangeArrow;
}
using namespace RenderCursor;

void CursorDisplayInit()
{
	GameCursor.LoadSprite("Assets\\GUI\\Pen.tga", Point2D(21, 328), 35.0);
	GameCursor.SetShadow(true, Point2D(-512, -512));
	Cross.LoadSprite("Assets\\GUI\\cross.tga");
	Check.LoadSprite("Assets\\GUI\\check.tga");
	RangeArrow.LoadSprite("Assets\\GUI\\rangearrow.tga");
}

void DrawCursor()
{
	Mouse * mouse = GameCursor.GetSource();
	Point2D mousePos = mouse->GetPosition();

	if (WorldGame.GetScreen() == SCREEN_GAMEPLAY)
	{
		Rect bounds(113, 115, 685, 687);
		if ((WorldGame.GetBuild() != NULL) && (bounds.ContainsPoint(mousePos) == true))
		{
			const TowerBase * TowerData = GetTowerData();
			const TowerBase * tower = &TowerData[WorldGame.GetBuild()];

			bool ghostState = tower->GetInt32Value(TOWER_VAR_GHOST_STATE);

			uint32 x = mousePos.GetX() - ((uint32)(mousePos.GetX() - 16) % 32) - 2;
			uint32 y = mousePos.GetY() - ((uint32)(mousePos.GetY() - 16) % 32) + 2;
			
			if (BuildTower(WorldGame.GetUInt32Value(GAME_VAR_BUILD), Point2D((x - 78)/32, (y - 82)/32), false) == false)
			{
				Cross.DrawSprite(Rect(0, 0, 48, 48), Point2D(x, y), Point2D(x, y), NullPoint, 0.6666, 0.0, ColorARGB(128, 0, 0, 0));
			}
			else
			{
				Check.DrawSprite(Rect(0, 0, 48, 48), Point2D(x, y), Point2D(x, y), NullPoint, 0.6666, 0.0, ColorARGB(128, 0, 0, 0));
				if (tower->GetInt32Value(TOWER_VAR_SPECIAL_RANGE) == 0)
				{
					uint32 arrowCount = tower->GetRange()/10.0;

					Float turnAmount = (Float)(GetGameTime() % 144000)/400.0;
					for (uint32 i = 0; i < arrowCount; i++)
					{
						RangeArrow.DrawSprite(Rect(0, 0, 16, 16), Point2D(x, y + 16 - tower->GetRange()), NullPoint, Point2D(x + 8, y + 8), 1.0, ((Float)i * (360.0/(Float)arrowCount)) + turnAmount, White);
					}
				}
			}
		}
	}

	GameCursor.DrawCursor();
}

void CursorDisplayUnload()
{
	GameCursor.Unload();
	Cross.Unload();
	Check.Unload();
	RangeArrow.Unload();
}