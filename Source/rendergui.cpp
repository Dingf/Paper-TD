#include "type.h"
#include "utility.h"
#include "point.h"
#include "rect.h"
#include "mouse.h"
#include "sprite.h"
#include "font.h"
#include "cursor.h"
#include "rendergui.h"
#include "selection.h"
#include "wave.h"
#include "game.h"

Sprite Background;

Sprite Icons;
Sprite GUISpriteboard;
Sprite BorderLine;
Sprite Inkbar;
Sprite WaveTimer;
Sprite CreepSprite;

void GUIDisplayInit()
{
	Background.LoadSprite("Assets\\GUI\\Background.tga");
	Icons.LoadSprite("Assets\\GUI\\icons.tga");
	GUISpriteboard.LoadSprite("Assets\\GUISprites.tga");
	BorderLine.LoadSprite("Assets\\GUI\\borderline.tga");
	Inkbar.LoadSprite("Assets\\GUI\\inkbar.tga");
	WaveTimer.LoadSprite("Assets\\GUI\\wavetimer.tga");
	CreepSprite.LoadSprite("Assets\\Sprites\\creeps.tga");
	CursorDisplayInit();
	InfoboxDisplayInit();
	TooltipDisplayInit();	
}

void DrawGameIcon(uint8 icon, const Point2D& position, uint32 size, const ColorARGB& color)
{
	uint32 iconsPerRow = Icons.GetWidth()/48;

	uint32 x = (icon % iconsPerRow) * 48;
	uint32 y = (icon/iconsPerRow) * 48;

	Icons.DrawSprite(Rect(x, y, x + 48, y + 48), position, position, NullPoint, size/48.0, 0.0, color);
}

void DrawBorderLine(uint32 length, const Point2D& position)
{
	BorderLine.DrawSprite(Rect(0, 0, 3, 8), position, NullPoint, NullPoint, 1.0, 0.0, White);

	uint32 pieceAmount = (length - 6)/2;
	for (uint32 i = 0; i < pieceAmount; i++)
	{
		BorderLine.DrawSprite(Rect(3, 0, 5, 8), Point2D(position.GetX() + 3 + (i * 2), position.GetY()), NullPoint, NullPoint, 1.0, 0.0, White);
	}

	if ((length % 2) != 0)
	{
		BorderLine.DrawSprite(Rect(3, 0, 4, 8), Point2D(position.GetX() + 3 + (pieceAmount * 2), position.GetY()), NullPoint, NullPoint, 1.0, 0.0, White);
		BorderLine.DrawSprite(Rect(5, 0, 8, 8), Point2D(position.GetX() + 4 + (pieceAmount * 2), position.GetY()), NullPoint, NullPoint, 1.0, 0.0, White);
	}
	else
	{
		BorderLine.DrawSprite(Rect(5, 0, 8, 8), Point2D(position.GetX() + 3 + (pieceAmount * 2), position.GetY()), NullPoint, NullPoint, 1.0, 0.0, White);
	}
}

//void DrawBackground()
//{
//	Background.DrawSprite(Rect(0, 0, 1024, 768), Origin, NullPoint, NullPoint, 1.0, 0.0, White);
//}
//
//void DrawInkbar()
//{
//	uint32 inkAmount = WorldGame.GetInk();
//	uint32 maxInk = WorldGame.GetMaxInk();
//
//	if (maxInk != 0)
//	{
//		Float endSpace = 90.0 * ((inkAmount < (1.0/18.0 * maxInk)) ? (Float)inkAmount/(Float)maxInk : (1.0/18.0));
//
//		Inkbar.DrawSprite(Rect(0, 0, endSpace, 16), Point2D(836, 94), NullPoint, NullPoint, 1.0, 0.0, White);
//
//		uint32 tickCount = ZeroFit(((Float)inkAmount/(Float)maxInk * 180.0) - 10);
//		for (uint32 i = 0; i < tickCount; i++)
//		{
//			Inkbar.DrawSprite(Rect(5, 0, 6, 16), Point2D(836 + endSpace + i, 94), NullPoint, NullPoint, 1.0, 0.0, White);
//		}
//
//		Inkbar.DrawSprite(Rect(16 - endSpace, 0, 16, 16), Point2D(836 + endSpace + tickCount, 94), NullPoint, NullPoint, 1.0, 0.0, White);
//	}
//
//	char inkStr[32];
//	sprintf(inkStr, "%d/%d", inkAmount, maxInk);
//	FeltPen[4]->PrintText(inkStr, Point2D(836, 129), Rect(0, 0, 180, 1), FORMAT_CENTER);
//}

void DrawWaveInfo()
{
	List<Wave *> * ActiveWaves = GetActiveWaves();

	char waveStr[32] = "";
	sprintf(waveStr, "%d", WorldGame.GetUInt32Value(GAME_VAR_WAVE_NUMBER));
	FeltPen[10]->PrintText(waveStr, Point2D(780, 214), Rect(0, 0, 64, 1), FORMAT_CENTER);

	int32 nextWaveTime = 0;

	ActiveWaves->ToStart();
	for (uint32 i = 0; i < ActiveWaves->GetSize(); i++)
	{
		Wave * wave = ActiveWaves->GetCurrent();
		ActiveWaves->ToNext();

		if ((wave->GetCreepCount() == 0) && (wave->GetUInt32Value(WAVE_VAR_ADD_TIME) > GetGameTime()))
		{
			nextWaveTime = wave->GetUInt32Value(WAVE_VAR_ADD_TIME) - GetGameTime();
			break;
		}
	}

	if (WorldGame.GetUInt32Value(GAME_VAR_WAVE_TIME_DIFFERENCE) > 0)
	{
		WaveTimer.DrawSprite(Rect(0, 0, 18, 5), Point2D(813, 206.5), NullPoint, Point2D(804.5, 206.5), 1.0, -90.0, White);
		WaveTimer.DrawSprite(Rect(0, 0, 18, 5), Point2D(813, 206.5), NullPoint, Point2D(804.5, 206.5), 1.0, ((nextWaveTime/(Float)WorldGame.GetUInt32Value(GAME_VAR_WAVE_TIME_DIFFERENCE)) * 360) - 90, White);
	}

	uint8 waveCounter = 0;
	uint32 creepsPerRow = CreepSprite.GetWidth()/32;

	Rect creepRects[2] = { Rect(0, 0, 32, 32),
						   Rect(0, 0, 32, 32) };

	ActiveWaves->ToStart();
	for (uint32 i = 0; (i < ActiveWaves->GetSize()) && (waveCounter < 2); i++)
	{
		Wave * wave = ActiveWaves->GetCurrent();
		ActiveWaves->ToNext();

		if ((wave->GetCreepCount() == 0) && (wave->GetUInt32Value(WAVE_VAR_ADD_TIME) > GetGameTime()))
		{
			int8 type = wave->GetWaveType() - 1;
			uint32 x = (type % creepsPerRow) * 32;
			uint32 y = (type/creepsPerRow) * 32;

			creepRects[waveCounter].SetValues(x, y, x + 32, y + 32);
			waveCounter++;
		}
	}

	CreepSprite.DrawSprite(creepRects[0], Point2D(970, 185), Point2D(970, 185), NullPoint, 0.8, 0, White);
	CreepSprite.DrawSprite(creepRects[1], Point2D(980, 209), Point2D(980, 209), NullPoint, 0.8, 0, White);
}

void DrawGUI()
{
	Point2D mousePos = WorldMouse.GetPosition();

	char scoreStr[64];
	sprintf(scoreStr, "%d", WorldGame.GetInt32Value(GAME_VAR_LIGHT_COUNTER));

	FeltPen[10]->PrintText(scoreStr, Point2D(860, 159), Rect(0, 0, 180, 1), FORMAT_LEFT);

	for (uint8 i = 0; i < 3; i++)
	{
		for (uint8 j = 0; j < 3; j++)
		{
			DrawGameIcon((i * 3) + j + 1, Point2D(789 + (84 * j), 447 - (72 * i)));
		}
	}

	for (uint8 i = 0; i < 3; i++)
	{
		for (uint8 j = 0; j < 3; j++)
		{
			Rect iconBounds(789 + (84 * j), 447 - (72 * i), 837 + (84 * j), 495 - (72 * i));
			if (iconBounds.ContainsPoint(mousePos) == true)
			{
				const TowerBase * TowerData = GetTowerData();

				char text[1024];
				const char * towerName = GetHotkeyedText(TowerData[(i*3)+j+1].GetName(), TowerData[(i*3)+j+1].GetHotkey());	

				sprintf(text, "%s \n\n%s", 
						towerName, 
						TowerData[(i*3)+j+1].GetDescription());

				DrawTooltip(text, FeltPen[4]);

				delete [] towerName;
				break;
			}
		}
	}
}

void GUIDisplayUnload()
{
	CreepSprite.Unload();
	WaveTimer.Unload();
	Inkbar.Unload();
	BorderLine.Unload();
	GUISpriteboard.Unload();
	Icons.Unload();
	Background.Unload();
	TooltipDisplayUnload();
	InfoboxDisplayUnload();
	CursorDisplayUnload();
}