#include "opengl.h"
#include "tinyxml.h"
#include "type.h"
#include "utility.h"
#include "point.h"
#include "object.h"
#include "mouse.h"
#include "script.h"
#include "animation.h"
#include "rendergame.h"
#include "emitter.h"
#include "gameobject.h"
#include "spell.h"
#include "spellobject.h"
#include "aura.h"
#include "creep.h"
#include "wave.h"
#include "projectile.h"
#include "tower.h"
#include "selection.h"
#include "grid.h"
#include "game.h"

Game WorldGame;

void Game::ClearHotkeys()
{
	for (uint8 i = 0; i < 10; i++)
	{
		hotkeys[i].Clear();
	}
}

uint32 GetGameTime()
{
	return glutGet(GLUT_ELAPSED_TIME);
}

void InitGame()
{
	WorldGame.SetType(NULL);
	WorldGame.AllocateInt32Values(GAME_INT32_VARS);
	WorldGame.AllocateUInt32Values(GAME_UINT32_VARS);
	WorldGame.AllocateFloatValues(GAME_FLOAT_VARS);
}

bool LoadGame(const char * filename)
{
	FILE * file = fopen(filename, "r");

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

	TiXmlDocument GameXML;
	GameXML.Parse(fileData);

	TiXmlNode * node = GameXML.FirstChild("WorldGameData");
	TiXmlElement * element = node->ToElement();

	node = element->FirstChild();
	element = node->ToElement();

	node = element->FirstChild();
	TiXmlElement * secondaryElement = node->ToElement();

	WorldGame.SetName(element->Attribute("Name"));

	if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
	{
		const char * text = secondaryElement->GetText();
		WorldGame.assetsPath = StringCopy(text);

		LoadAnimationData();
		LoadGUIScripts();
		LoadEmitterData();
		LoadGameObjectData();
		LoadSpellData();
		LoadProjectileData();
		LoadTowerData();
	}
	secondaryElement = secondaryElement->NextSiblingElement();

	for (uint8 i = GAME_VAR_INK; i <= GAME_VAR_CREEP_BOUNTY_SCALE; i++)
	{
		if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
		{
			const char * text = secondaryElement->GetText();
			Float word = LookupWord(text, LOOKUP_VALUE);
			if (word == NOT_FOUND)
			{
				word = atof(text);
			}

			WorldGame.SetFloatValue(i, word);	
		}
		secondaryElement = secondaryElement->NextSiblingElement();
	}

	if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
	{
		const char * text = secondaryElement->GetText();
		Float word = LookupWord(text, LOOKUP_VALUE);
		if (word == NOT_FOUND)
		{
			word = atof(text);
		}

		WorldGame.SetInt32Value(GAME_VAR_LIGHT_COUNTER, word);	
	}
	secondaryElement = secondaryElement->NextSiblingElement();

	for (uint8 i = GAME_VAR_WAVE_SIZE; i <= GAME_VAR_SELL_TIMER_INCREMENT; i++)
	{
		if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
		{
			const char * text = secondaryElement->GetText();
			Float word = LookupWord(text, LOOKUP_VALUE);
			if (word == NOT_FOUND)
			{
				word = atof(text);
			}

			WorldGame.SetUInt32Value(i, word);	
		}
		secondaryElement = secondaryElement->NextSiblingElement();
	}

	if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
	{
		const char * text = secondaryElement->GetText();
		//MIA Layout Code
	}
	secondaryElement = secondaryElement->NextSiblingElement();

	GameXML.Clear();
	Delete(fileData);

	WorldGame.SetState(true);

	/*WorldGame.SetUInt32Value(GAME_VAR_WAVE_SIZE, 25);
	WorldGame.SetUInt32Value(GAME_VAR_WAVE_INTERVAL, 250);
	WorldGame.SetUInt32Value(GAME_VAR_WAVE_TIME_DIFFERENCE, 30000);
	WorldGame.SetUInt32Value(GAME_VAR_WAVE_PRELOAD_COUNT, 10);

	WorldGame.SetFloatValue(GAME_VAR_MAX_INK, 9999999);
	WorldGame.SetFloatValue(GAME_VAR_INK, 1E10);
	WorldGame.SetInt32Value(GAME_VAR_LIGHT_COUNTER, 50);
	WorldGame.SetFloatValue(GAME_VAR_BASE_CREEP_HP, 50.0);
	WorldGame.SetFloatValue(GAME_VAR_CREEP_HP_SCALE, 1.09318161);
	WorldGame.SetFloatValue(GAME_VAR_CREEP_ARMOR_SCALE, 1.0);
	WorldGame.SetFloatValue(GAME_VAR_CREEP_BOUNTY_SCALE, 1.75);*/

	WorldGame.AddInk(1E10);
	WorldGame.SetUInt32Value(GAME_VAR_SCREEN, SCREEN_GAMEPLAY);
	WorldGame.SetUInt32Value(GAME_VAR_INSTANT_CAST_FLAG, true);
	for (uint8 i = 0; i < GRID_SIZE; i++)
	{
		for (uint8 j = 0; j < GRID_SIZE; j++)
		{
			if (((i == 0) || (j == 0) || (i == GRID_SIZE - 1) || (j == GRID_SIZE - 1)) && ((i != GRID_SIZE/2) && (i != GRID_SIZE/2 - 1)))
			{
				BuildTower(1, Point2D(i, j), true);
			}
		}
	}

	Grid * grid = WorldGame.GetGrid();
	grid->FindPath(NullPoint);

	WorldGame.ClearSelection();
	WorldGame.ClearHotkeys();

	return true;

	/*WorldGame.SetFloatValue(GAME_VAR_INK, WorldGame.GetFloatValue(GAME_VAR_MAX_INK));*/

	//WorldGame.ClearGridPath();
}

void UpdateGame()
{
	if (WorldGame.GetInk() > WorldGame.GetMaxInk())
	{
		WorldGame.SetFloatValue(GAME_VAR_INK, WorldGame.GetMaxInk());
	}

	WorldGame.SetInt32Value(GAME_VAR_LIGHT_COUNTER, IntervalFit(WorldGame.GetInt32Value(GAME_VAR_LIGHT_COUNTER), 0, 100));

	if (WorldGame.GetInt32Value(GAME_VAR_LIGHT_COUNTER) != WorldGame.GetInt32Value(GAME_VAR_PREV_LIGHT_COUNTER))
	{
		WorldGame.SetInt32Value(GAME_VAR_PREV_LIGHT_COUNTER, WorldGame.GetInt32Value(GAME_VAR_LIGHT_COUNTER));
		WorldGame.SetFloatValue(GAME_VAR_LC_MULTIPLIER_DAMAGE, ExpN(4.0, ((WorldGame.GetInt32Value(GAME_VAR_LIGHT_COUNTER) - 50)/50.0)));
		WorldGame.SetFloatValue(GAME_VAR_LC_MULTIPLIER_BOUNTY, ExpN(4.0, ((WorldGame.GetInt32Value(GAME_VAR_LIGHT_COUNTER) - 50)/-50.0)));
	}

	WorldGame.SpellMaintenance();
	EmitterMaintenance();
	AuraMaintenance();
	GameObjectMaintenance();
	TowerMaintenance();
	ProjectileMaintenance();
	WaveMaintenance();
	CreepMaintenance();
}

void UnloadGame()
{
	WorldGame.SetState(false);
	WorldGame.ClearSelection();
	WorldGame.ClearHotkeys();

	delete [] WorldGame.assetsPath;

	Grid * grid = WorldGame.GetGrid();
	for (uint8 i = 0; i < GRID_SIZE; i++)
	{
		for (uint8 j = 0; j < GRID_SIZE; j++)
		{
			Tower * tower = grid->GetData(Point2D(i, j));
			if (tower != NULL)
			{
				tower->Unload();
				delete tower;
			}
		}
	}
//
//std::vector<Animation *> * ActiveAnimations = GetActiveAnimations();
//std::vector<Projectile *> * ActiveProjectiles = GetActiveProjectiles();
//std::vector<Creep *> * ActiveCreeps = GetActiveCreeps();

	ClearActiveAuras();
	ClearActiveCreeps();
	ClearActiveWaves();
	ClearActiveAnimations();
	ClearActiveProjectiles();
	ClearActiveGameObjects();
	ClearActiveEmitters();
	
	//UnloadCreepData();
	UnloadProjectileData();
	UnloadTowerData();
	UnloadSpellData();
	UnloadGameObjectData();
	UnloadEmitterData();
	UnloadAnimationData();
}


