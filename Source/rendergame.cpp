#include "opengl.h"
#include "tinyxml.h"
#include "type.h"
#include "math.h"
#include "point.h"
#include "rect.h"
#include "script.h"
#include "color.h"
#include "sprite.h"
#include "font.h"
#include "animation.h"
#include "rendergui.h"
#include "creep.h"
#include "projectile.h"
#include "tower.h"
#include "selection.h"
#include "grid.h"
#include "game.h"

#include "cone.h"
#include "shapes.h"
#include "mouse.h"
#include "gameobject.h"

namespace RenderGame
{
	Sprite CreepSprite;
	Sprite Overlay;
	Sprite RangeArrow;
	Sprite Castbar;
	Sprite LevelDot;
}

using namespace RenderGame;

void GameDisplayInit()
{
	CreepSprite.LoadSprite("Assets\\Sprites\\GUI\\creeps.tga");
	Overlay.LoadSprite("Assets\\GUI\\overlay.tga");
	RangeArrow.LoadSprite("Assets\\GUI\\rangearrow.tga");
	Castbar.LoadSprite("Assets\\GUI\\castbar.tga");
	LevelDot.LoadSprite("Assets\\GUI\\level.tga");
}

bool LoadGUIScripts()
{
	FILE * file = fopen("Assets\\XML\\gui.xml", "r");

	if (file == NULL)
	{
		return false;
	}

	fseek(file, 0, SEEK_END);
	uint32 fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char * fileData = new char[fileSize];

	fread(fileData, fileSize, 1, file);

	fclose(file);

	TiXmlDocument GUIXML;
	GUIXML.Parse(fileData);

	TiXmlNode * node = GUIXML.FirstChild("GUIData");
	TiXmlElement * element = node->ToElement();

	node = element->FirstChild();
	element = node->ToElement();

	uint32 GUIXMLDataSize;
	TiXmlElement * lastElement = element;
	for (uint32 i = 0;; i++)
	{
		if (lastElement == NULL)
		{
			if (i > 0)
			{
				GUIXMLDataSize = i;
				break;
			}
			else
			{
				return false;
			}
		}
		lastElement = lastElement->NextSiblingElement();
	}

	node = element->FirstChild();
	TiXmlElement * secondaryElement = node->ToElement();

	for (uint32 i = 0; i < GUIXMLDataSize; i++)
	{
		if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
		{
			const char * text = secondaryElement->GetText();

			Script * script = new Script;
			script->LoadScript(text, &WorldGame);
			script->ActivateScript(true);

			delete script;
		}
		secondaryElement = secondaryElement->NextSiblingElement();

		element = element->NextSiblingElement();
		if (element != NULL)
		{
			node = element->FirstChild();
			secondaryElement = node->ToElement();
		}
		else
		{
			break;
		}
	}

	GUIXML.Clear();
	Delete(fileData);

	return true;
}

/*void DrawGrid()
{
	Selection * selection = WorldGame.GetSelection();
	Tower ** gridData = WorldGame.GetGridData();

	Grid * grid = WorldGame.GetGrid();

	for (uint8 i = 1; i < GRID_SIZE - 1; i++)
	{
		for (uint8 j = 1; j < GRID_SIZE - 1; j++)
		{
			Tower * tower = gridData[(i*GRID_SIZE)+j];

			if ((tower != NULL) && (tower->GetState() == true))
			{
				Point2D iconPos(110 + (32 * (i - 1)), 114 + (32 * (j - 1)));

				const Spell * castTarget = tower->GetCastTarget();
				ColorARGB color = (tower->GetInt32Value(TOWER_VAR_GHOST_STATE) == true) ? ColorARGB(96, 255, 255, 255) : White;
				if (castTarget != NULL)
				{
					DrawGameIcon(0, iconPos, 32, color);

					Castbar.DrawSprite(Rect(0, 0, 32, 32), Point2D(iconPos.GetX() + 6, iconPos.GetY() + 6),  Point2D(iconPos.GetX() + 6, iconPos.GetY() + 6), NullPoint, 0.66, 0.0, color);

					Float barCount = ((GetGameTime() - castTarget->GetLastCastTime())/(Float)castTarget->GetCastTime()) * 180.0;
					for (uint32 i = 0; i < barCount; i++)
					{
						Castbar.DrawSprite(Rect(63, 60, 64, 64), Point2D(iconPos.GetX() + 16, iconPos.GetY() + 6), Point2D(iconPos.GetX() + 16, iconPos.GetY() + 6), Point2D(iconPos.GetX() + 16, iconPos.GetY() + 14), 0.6666, i * 2.0, color);
					}
				}
				else
				{
					DrawGameIcon(tower->GetUInt32Value(TOWER_VAR_ICON_ID), iconPos, 32, color);
					if (tower->GetTowerType() != 1)
					{
						iconPos.SetValues(iconPos.GetX() + 2, iconPos.GetY() + 25);
						for (uint32 i = 0; ((i < tower->GetTowerLevel()) && (i < MAX_TOWER_LEVEL)); i++)
						{
							LevelDot.DrawSprite(Rect(0, 0, 5, 5), iconPos, NullPoint, NullPoint, 1.0, 0.0, color);
							iconPos.SetValues(iconPos.GetX() + 6, iconPos.GetY());
						}
					}
				}

				if (selection->GetIndex(tower) != -1)
				{
					Overlay.DrawSprite(Rect(0, 0, 64, 64), Point2D(110 + (32 * (i - 1)), 114 + (32 * (j - 1))),  Point2D(110 + (32 * (i - 1)), 114 + (32 * (j - 1))), NullPoint, 0.5, 0.0, ColorARGB(128, 255, 255, 255));

					if ((selection->GetSize() == 1) && (tower->GetInt32Value(TOWER_VAR_SPECIAL_RANGE) == 0))
					{
						uint32 arrowCount = tower->GetRange()/10.0;

						Float turnAmount = (Float)(GetGameTime() % 144000)/400.0;
						for (uint32 k = 0; k < arrowCount; k++)
						{
							RangeArrow.DrawSprite(Rect(0, 0, 16, 16), Point2D(110 + (32 * (i - 1)), 130 + (32 * (j - 1)) - tower->GetRange()), NullPoint, Point2D(118 + (32 * (i - 1)), 122 + (32 * (j - 1))), 1.0, ((Float)k * (360.0/(Float)arrowCount)) + turnAmount, White);
						}
					}
				}
			}
		}
	}

	for (uint8 i = 0; i < GRID_SIZE; i++)
	{
		for (uint8 j = 0; j < GRID_SIZE; j++)
		{
			Grid * grid = WorldGame.GetGrid();
			uint32 val = grid->GetPath(Point2D(i, j));
			
			ColorARGB color;
			switch (val)
			{
				case 0:
				{
					color = White;
					break;
				}
				case 1:
				{
					color = Red;
					break;
				}
				case 2:
				{
					color = Green;
					break;
				}
				case 3:
				{
					color = Blue;
					break;
				}
				case 4:
				{
					color = Yellow;
					break;
				}
				case 5:
				{
					color = Violet;
					break;
				}
				case 6:
				{
					color = Teal;
					break;
				}
			}
			color.SetAlpha(128);
			Overlay.DrawSprite(Rect(0, 0, 64, 64), Point2D(78 + (32 * i), 82 + (32 * j)),  Point2D(78 + (32 * i), 82 + (32 * j)), NullPoint, 0.5, 0.0, color);
		}
	}
}*/

void DrawCreeps()
{
	//Animation * source = (Animation *)script->GetSource();

	//if ((source != NULL) && (source->GetState() == true) && (source->CheckType(OBJ_TYPE_ANIMATION) == true))
	{
		List<Creep *> * ActiveCreeps = GetActiveCreeps();

		ActiveCreeps->ToStart();
		for (uint32 i = 0; i < ActiveCreeps->GetSize(); i++)
		{
			Creep * creep = ActiveCreeps->GetCurrent();
			ActiveCreeps->ToNext();

			if ((creep != NULL) && (creep->GetState() == true))
			{
				Point2D creepPos = creep->GetPosition();
				creepPos.SetValues(creepPos.GetX(), creepPos.GetY() + 32);

				char text[256] = "";

				sprintf(text, "%.0f/%.0f/%.0f/%.0f", creep->GetHitpoints(), creep->GetFloatValue(CREEP_VAR_MAX_HITPOINTS), creep->GetBounty(), creep->GetArmor());
				FeltPen[4]->PrintText(text, creepPos, Rect(0, 0, 256, 1), 0);
			}
		}
	}
}

void DrawGameplay()
{
	//DrawBackground();
	//DrawInkbar();
	//DrawWaveInfo();
	//DrawGrid();
	AnimationMaintenance();
	DrawGUI();
	DrawCreeps();
	DrawInfobox();
	DrawCursor();

	/*List<GameObject *> * ActiveGameObjects = GetActiveGameObjects();
	for (uint32 a = 0; a < ActiveGameObjects->GetSize(); a++)
	{
		GameObject * go = ActiveGameObjects->GetData(a); 
		Shape * shape = go->GetShape();
		Shape * newShape = shape->CreateInstance();
		newShape->Translate(go->GetPosition());
		

		Point2D rotatePosition(512, 512);
		LineSegment line(Point2D(400, 400), Point2D(512, 512));

					Vector3D lineDirection(rotatePosition, line.GetMidpoint()); 
					Vector3D direction(rotatePosition, WorldMouse.GetPosition());
					Float baseAngle = lineDirection.GetZeroAngleD() - direction.GetZeroAngleD();

					Vector3D v1(rotatePosition, line.GetPoint1());
					Vector3D v2(rotatePosition, line.GetPoint2());

					Float angle1 = v1.GetZeroAngleD() - baseAngle;
					Float angle2 = v2.GetZeroAngleD() - baseAngle;

					Point2D p1(rotatePosition.GetX() + (v1.GetLength() * CosD(angle1)),
							   rotatePosition.GetY() + (v1.GetLength() * SinD(angle1)));
					Point2D p2(rotatePosition.GetX() + (v2.GetLength() * CosD(angle2)),
							   rotatePosition.GetY() + (v2.GetLength() * SinD(angle2)));

					line.SetValues(p1, p2);

		for (uint32 i = 0; i < 64; i++)
		{
			for (uint32 j = 0; j < 48; j++)
			{
				Point2D x(i * 16, j * 16);
				Rect rect(i * 16, j * 16, i * 16 + 16, j * 16 + 16);

				if (Intersect(&rect, newShape))
				{
					LevelDot.DrawSprite(Rect(0, 0, 5, 5), x, NullPoint, NullPoint, 1.0, 0.0, White);
				}
			}
		}
		delete newShape;
	}*/

	//Very lagging cone testing. Please ignore
	/*Cone cone(Point2D(512, 512), WorldMouse.GetPosition(), 256, 30);

	for (uint32 i = 0; i < 64; i++)
	{
		for (uint32 j = 0; j < 48; j++)
		{
			Point2D x(i * 16, j * 16);

			if (cone.ContainsPoint(x))
			{
				LevelDot.DrawSprite(Rect(0, 0, 5, 5), x, NullPoint, NullPoint, 1.0, 0.0, White);
			}
		}
	}*/
}


void GameDisplayUnload()
{
	LevelDot.Unload();
	Castbar.Unload();
	RangeArrow.Unload();
	Overlay.Unload();
	CreepSprite.Unload();
}
