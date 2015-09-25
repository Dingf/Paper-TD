#include "tinyxml.h"
#include "type.h"
#include "utility.h"
#include "point.h"
#include "rect.h"
#include "list.h"
#include "script.h"
#include "spell.h"
#include "gameobject.h"
#include "spellobject.h"
#include "aura.h"
#include "creep.h"
#include "projectile.h"
#include "tower.h"
#include "grid.h"
#include "game.h"

TowerBase * TowerData = NULL;
uint32 towerDataSize = 0;

List<Tower *> RemovedTowers;

Tower::Tower()
{
	SetState(true);
	SetType(OBJ_TYPE_TOWER | OBJ_TYPE_SPELL_OBJ);
	AllocateInt32Values(TOWER_INT32_VARS);
	AllocateUInt32Values(TOWER_UINT32_VARS);
	AllocateFloatValues(TOWER_FLOAT_VARS);

	target = NULL;
	castTarget = NULL;

	SetUInt32Value(TOWER_VAR_LEVEL, 1);
	SetFloatValue(TOWER_VAR_DAMAGE_MULTIPLIER, 1.0);
	SetFloatValue(TOWER_VAR_RANGE_MULTIPLIER, 1.0);
	SetFloatValue(TOWER_VAR_BAT_MULTIPLIER, 1.0);

	projectile = new Projectile;

	//projectile->SetUInt32Value(PROJ_VAR_FLAGS, PFLAG_LINEAR);
}

Tower::~Tower()
{
	Unload();
}

//void Tower::Init(uint8 type, const Point2D& coords)
//{
//	Copy(TowerData[type]);
//
//	SetPosition((coords.GetX() * 32) + 80, (coords.GetY() * 32) + 80);
//
//	Projectile * projectile = GetProjectile();
//	Projectile * baseProj = TowerData[type].GetProjectile();
//
//	projectile->Copy(*baseProj);
//
//	uint32 baseSize = projectile->GetUInt32Value(PROJ_VAR_BASE_SIZE);
//	projectile->SetPosition(position.GetX() + 16 - baseSize/2, position.GetY() + 16 - baseSize/2);
//
//	const std::vector<Spell *> * baseSpells = TowerData[type].GetKnownSpells();
//	for (uint32 i = 0; i < baseSpells->size(); i++)
//	{
//		Spell * spell = baseSpells->at(i);
//		LearnSpell(spell, this);
//		//knownSpells.push_back(spell->CreateInstance(this));
//		ApplySpell(spell, this);
//	}
//	RefreshUpgradeSpells();
//}

TowerBase::~TowerBase()
{
	Delete(description);
}

Spell * Tower::GetVisibleSpell(bool row, uint8 col)
{
	List<Spell *> * spellList = (row == 0) ? GetUpgradeSpells() : GetKnownSpells();

	Spell * spell = NULL;
	uint8 counter = 0;

	spellList->ToStart();
	for (uint32 i = 0; (counter <= col) && (i < spellList->GetSize()); i++)
	{
		Spell * currentSpell = spellList->GetCurrent();
		spellList->ToNext();

		if (currentSpell->GetUInt32Value(SPELL_VAR_VISIBILITY) == true)
		{
			if (((currentSpell->GetMode() == SPELL_MODE_LEARN) && (row == 0) && (GetTowerLevel() < MAX_TOWER_LEVEL)) || ((currentSpell->GetMode() != SPELL_MODE_LEARN) && (row == 1)))
			{
				spell = currentSpell;
				counter++;
			}
		}
	}

	return spell;
}

void Tower::PerformAction()
{
	if ((target != NULL) && (target->GetState() == false))
	{
		ClearAttackTarget();
	}
	if (projectile->GetState() == false)
	{
		delete projectile;
		projectile = NULL;

		uint32 projID = GetUInt32Value(TOWER_VAR_PROJECTILE_ID);

		if (projID < GetProjectileDataSize())
		{
			const Projectile * ProjectileData = GetProjectileData();
			projectile = new Projectile(ProjectileData[projID]);

			uint32 baseSize = projectile->GetUInt32Value(PROJ_VAR_BASE_SIZE);
			projectile->SetPosition(position.GetX() + 16 - baseSize/2, position.GetY() + 16 - baseSize/2);
			RefreshActiveSpells();
		}
	}
	SpellMaintenance();
	projectile->SpellMaintenance();
	if (GetCastTarget() == NULL)
	{
		UpdateState();
		FireProjectile();
	}
}

void Tower::UpdateState()
{
	if ((target != NULL) && ((target->GetState() == false) || (CalculateDistance(position, target->GetPosition()) > GetRange())))
	{
		target = NULL;
	}
	AcquireAttackTarget();
}

void Tower::AcquireAttackTarget()
{
	if ((GetInt32Value(TOWER_VAR_CAN_FIRE) == true) && (target == NULL))
	{
		List<Creep *> * ActiveCreeps = GetActiveCreeps();

		ActiveCreeps->ToStart();
		for (uint32 i = 0; i < ActiveCreeps->GetSize(); i++)
		{
			Creep * creep = ActiveCreeps->GetCurrent();
			ActiveCreeps->ToNext();

			if (SetAttackTarget(creep) == true)
			{
				break;
			}
		}
	}
}

bool Tower::SetAttackTarget(Creep * creep)
{
	if ((creep->GetState() == true) && (CalculateDistance(position, creep->GetPosition()) <= GetRange()))
	{
		target = creep;
		return true;
	}
	return false;
}

void Tower::AddKill()
{
	ModUInt32Value(TOWER_VAR_CREEP_KILLS, 1);
	RefreshActiveSpells();
	ActivateTriggeredScript(TOWER_TRIGGER_ON_KILL);
}

void Tower::FireProjectile()
{
	if ((GetState() == true) && ((bool)GetInt32Value(TOWER_VAR_CAN_FIRE) == true) && ((bool)GetUInt32Value(TOWER_VAR_WAITING) == false) && (target != NULL) && (activeSpell != NULL))
	{
		uint32 time = GetGameTime();
		int32 lastTime = GetUInt32Value(TOWER_VAR_LAST_ATTACK_TIME);

		if (((time - lastTime) >= GetAttackTime()) || (lastTime == NULL))
		{
			if (lastTime != NULL)
			{
				SetUInt32Value(TOWER_VAR_ATTACK_TIME_DIF, time - lastTime);
			}
			SetUInt32Value(TOWER_VAR_LAST_ATTACK_TIME, time);
			RefreshActiveSpells();
			ActivateTriggeredScript(TOWER_TRIGGER_ON_FIRE);
			AddProjectile(this);
		}
	}
}

//bool Tower::CheckSpellPrerequisites(uint32 spellID) const
//{
//	for (uint32 i = 0; i < knownSpells.size(); i++)
//	{
//		if (knownSpells[i]->GetUInt32Value(SPELL_VAR_ID) == spellID)
//		{
//			return true;
//		}
//	}
//	return false;
//}

void Tower::ClearUpgradeSpells()
{
	while (upgradeSpells.GetSize() > 0)
	{
		if (upgradeSpells[0] != NULL)
		{
			delete upgradeSpells[0];
		}
		upgradeSpells.Remove(0);
	}
}

void Tower::RefreshUpgradeSpells()
{
	const Spell * SpellData = GetSpellData();

	ClearUpgradeSpells();

	for (uint32 i = 0; i < GetSpellDataSize(); i++)
	{
		const Spell * baseSpell = &SpellData[i];
		if ((baseSpell->GetMode() == SPELL_MODE_LEARN) && (KnowsSpell(baseSpell->GetUInt32Value(SPELL_VAR_PREREQUISITE)) > 0))//CheckSpellPrerequisites(SpellData[i].GetUInt32Value(SPELL_VAR_PREREQUISITE)) == true))
		{
			if (KnowsSpell(baseSpell) == false)
			{
				upgradeSpells.Append(baseSpell->CreateInstance(this, this));
			}
		}
	}
}

//bool Tower::ActivateSpell(bool spellList, uint16 spellID)
//{
//	std::vector<Spell *> * spells = (spellList == TOWER_SPELL_LIST_KNOWN) ? &knownSpells : &upgradeSpells;
//	if (spellID < spells->size())
//	{
//		Spell * spell = spells->at(spellID);
//
//		if (spell->GetCastTime() > 0)
//		{
//			if (spell->GetInt32Value(SPELL_VAR_LAST_CAST_TIME) == 0)
//			{
//				spell->SetInt32Value(SPELL_VAR_LAST_CAST_TIME, GetGameTime());
//				castTarget = spell;
//				return true;
//			}
//			else
//			{
//				return false;
//			}
//		}
//		knownSpells.push_back(spell);
//
//		ApplySpell(spell);
//		Clear();
//		ReloadSpells();
//		RefreshUpgradeSpells();
//		return true;
//	}
//	return false;
//}

//bool Tower::ApplySpell(uint16 spellID)
//{
//	if (spellID < GetSpellDataSize())
//	{
//	castTarget->SetUInt32Value(SPELL_VAR_LAST_CAST_TIME, 0);
//	if (castTarget->GetMode() == SPELL_MODE_LEARN)
//	{
//		knownSpells.push_back(castTarget->CreateInstance(castTarget->GetSource()));
//	}
//
//	ApplySpell(castTarget);
//	RefreshUpgradeSpells();
//	Clear();
//	ReloadSpells();
//	castTarget = NULL;
//}

//bool Tower::LearnSpell(uint16 spellID)
//{
//	if (spellID < GetSpellDataSize())
//	{
//		for (uint32 i = 0; i < knownSpells.size(); i++)
//		{
//			if (knownSpells[i]->GetID() == spellID)
//			{
//				return false;
//			}
//		}
//
//		const Spell * SpellData = GetSpellData();
//		knownSpells.push_back(SpellData[spellID].CreateInstance(this));
//		return true;
//	}
//	return false;
//}

//bool Tower::UnlearnSpell(uint16 spellID)
//{
//	bool result = false;
//	for (int32 i = 0; i < knownSpells.size(); i++)
//	{
//		if (knownSpells[i]->GetID() == spellID)
//		{
//			knownSpells.erase(knownSpells.begin() + i);
//			result = true;
//			i--;
//		}
//	}
//	return result;
//}

void Tower::Clear()
{
	ClearGameObjects();
	
	TowerBase * base = &TowerData[GetTowerType()];

	int32ValuesCount = base->int32ValuesCount;
	floatValuesCount = base->floatValuesCount;
	memcpy(int32Values, base->int32Values, (base->int32ValuesCount * sizeof(int32)));
	memcpy(floatValues, base->floatValues, (base->floatValuesCount * sizeof(Float)));

	projectile->Clear();
	projectile->ClearActiveSpells();

	triggeredScripts.ToStart();
	for (uint32 i = 0; i < triggeredScripts.GetSize(); i++)
	{
		TriggeredScript * triggeredScript = triggeredScripts.GetCurrent();
		triggeredScripts.ToNext();

		triggeredScript->SetState(false);
	}
}

void Tower::OnCast(Spell * spell)
{
	if (GetState() == true)
	{
		ActivateTriggeredScript(SPELL_OBJ_TRIGGER_ON_CAST);
		if ((spell != NULL) && (spell->GetMode() == SPELL_MODE_LEARN) && (GetTowerLevel() < MAX_TOWER_LEVEL))
		{
			uint32 towerLevel = GetTowerLevel();

			ModUInt32Value(TOWER_VAR_LEVEL, 1);
			ModUInt32Value(TOWER_VAR_BASE_COST, spell->GetInt32Value(SPELL_VAR_COST));

			RefreshUpgradeSpells();

			const TowerBase * TowerData = GetTowerData();
			const TowerBase * BaseTower = &TowerData[GetTowerType()];

			ModUInt32Value(TOWER_VAR_BASE_DAMAGE, BaseTower->GetUpgradeStats(UPGRADE_DAMAGE, towerLevel - 1));
			ModUInt32Value(TOWER_VAR_BASE_RANGE, BaseTower->GetUpgradeStats(UPGRADE_RANGE, towerLevel - 1));
			ModUInt32Value(TOWER_VAR_BASE_ATTACK_TIME, BaseTower->GetUpgradeStats(UPGRADE_BAT, towerLevel - 1));
		}
	}
	if ((projectile != NULL) && (projectile->GetState() == true))
	{
		projectile->RefreshActiveSpells();
	}
	//RefreshActiveAuras();
}

void Tower::Copy(const Tower& obj)
{
	if (obj.CheckType(OBJ_TYPE_TOWER) == true)
	{
		Point2D position = GetPosition();
		Tower * tower = (Tower *)&obj;

		projectile->Copy(*tower->projectile);
		uint32 baseSize = projectile->GetUInt32Value(PROJ_VAR_BASE_SIZE);
		projectile->SetPosition(position.GetX() + 16 - baseSize/2, position.GetY() + 16 - baseSize/2);

		SpellObject::Copy(obj);
		SetPosition(position);

		RefreshUpgradeSpells();
	}
}

void Tower::Unload()
{
	SetState(false);
	target = NULL;

	uint8 type = GetTowerType();
	if (projectile != NULL)
	{
		delete projectile;
		projectile = NULL;
	}
	ClearUpgradeSpells();
}

const TowerBase * GetTowerData()
{
	return TowerData;
}

uint32 GetTowerDataSize()
{
	return towerDataSize;
}

bool LoadTowerData()
{
	if (TowerData == NULL)
	{
		FILE * file = fopen("Assets\\XML\\tower.xml", "r");

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

		TiXmlDocument TowerXML;
		TowerXML.Parse(fileData);

		TiXmlNode * node = TowerXML.FirstChild("TowerData");
		TiXmlElement * element = node->ToElement();

		node = element->FirstChild();
		element = node->ToElement();

		TiXmlElement * lastElement = element;
		for (uint32 i = 0;; i++)
		{
			if (lastElement == NULL)
			{
				if (i > 0)
				{
					towerDataSize = i;
					TowerData = new TowerBase[towerDataSize];
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

		for (uint32 i = 0; i < towerDataSize; i++)
		{
			int32 ID = 0;
			element->Attribute("ID", &ID);

			TowerData[i].SetName(element->Attribute("Name"));
			TowerData[i].SetUInt32Value(TOWER_VAR_TYPE, ID);

			for (uint8 j = TOWER_VAR_BASE_DAMAGE; j <= TOWER_VAR_CAN_BUILD; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					Float word = LookupWord(text, LOOKUP_VALUE);
					if (word == NOT_FOUND)
					{
						word = atof(text);
					}

					TowerData[i].SetUInt32Value(j, word);	
				}
				secondaryElement = secondaryElement->NextSiblingElement();
			}
			for (uint8 j = TOWER_VAR_CAN_FIRE; j <= TOWER_VAR_GHOST_STATE; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					Float word = LookupWord(text, LOOKUP_VALUE);
					if (word == NOT_FOUND)
					{
						word = atof(text);
					}

					TowerData[i].SetInt32Value(j, word);	
				}
				secondaryElement = secondaryElement->NextSiblingElement();
			}
			for (uint8 j = TOWER_VAR_SPELL_COEFFICIENT; j <= TOWER_VAR_SPELL_COEFFICIENT; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					Float word = LookupWord(text, LOOKUP_VALUE);
					if (word == NOT_FOUND)
					{
						word = atof(text);
					}

					TowerData[i].SetFloatValue(j, word);	
				}
				secondaryElement = secondaryElement->NextSiblingElement();
			}
			for (uint8 j = 0; j < 3; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					sscanf(text, "%d %d %d %d", &TowerData[i].upgradeStats[j][0], &TowerData[i].upgradeStats[j][1], &TowerData[i].upgradeStats[j][2], &TowerData[i].upgradeStats[j][3]);
				}
				secondaryElement = secondaryElement->NextSiblingElement();
			}
			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				List<uint32> knownSpellIDs;

				int32 lastSpace = -1;
				for (uint32 j = 0; j < strlen(text); j++)
				{
					if ((text[j] == ' ') || (j == strlen(text) - 1))
					{
						j = (j == strlen(text) - 1) ? j + 1 : j;

						char * wordString = new char[j-lastSpace];
						for (int32 k = lastSpace + 1; k < j; k++)
						{
							wordString[k-lastSpace-1] = text[k];
						}
						wordString[j-lastSpace-1] = '\0';
						Float word = LookupWord(wordString, LOOKUP_VALUE);
						if (word == NOT_FOUND)
						{
							word = atof(wordString);
						}

						knownSpellIDs.Append(word);
						lastSpace = j;

						delete wordString;
					}
				}

				for (uint32 j = 0; j < knownSpellIDs.GetSize(); j++)
				{
					uint32 spellID = knownSpellIDs[j];
					if (spellID < GetSpellDataSize())
					{
						const Spell * spell = GetSpell(spellID);

						TowerData[i].knownSpells.Append(spell->CreateInstance(&TowerData[i], &TowerData[i]));
						if ((spell->GetMode() != SPELL_MODE_TRIGGERED) && (spell->GetMode() != SPELL_MODE_LEARN))
						{
							TowerData[i].activeSpells.Append(spell->CreateInstance(&TowerData[i], &TowerData[i]));
						}
					}
				}
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				Float word = LookupWord(text, LOOKUP_VALUE);
				if ((word == NOT_FOUND) && (text != NULL))
				{
					word = text[0];
				}

				TowerData[i].hotkey = word;
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				Float word = LookupWord(text, LOOKUP_VALUE);
				if (word == NOT_FOUND)
				{
					word = atof(text);
				}

				TowerData[i].SetUInt32Value(TOWER_VAR_ICON_ID, word);
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				Float word = LookupWord(text, LOOKUP_VALUE);
				if (word == NOT_FOUND)
				{
					word = atof(text);
				}

				if (word < GetProjectileDataSize())
				{
					TowerData[i].SetUInt32Value(TOWER_VAR_PROJECTILE_ID, word);

					const Projectile * ProjectileData = GetProjectileData();
					Projectile * projectile = TowerData[i].GetProjectile();

					projectile->Copy(ProjectileData[(uint32)word]);
					projectile->SetSource(&TowerData[i]);
					
				}
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			const char * text = secondaryElement->GetText();
			if (text != NULL)
			{
				TowerData[i].description = StringCopy(text);
					
				/*	new char[strlen(text)+1];
				for (uint32 j = 0; j < strlen(text); j++)
				{
					TowerData[i].description[j] = text[j];
				}
				TowerData[i].description[strlen(text)] = '\0';*/
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

		TowerXML.Clear();
		Delete(fileData);

		return true;
	}
	return false;
}

void TowerMaintenance()
{
	while (RemovedTowers.GetSize() > 0)
	{
		if (RemovedTowers[0] != NULL)
		{
			delete RemovedTowers[0];
		}
		RemovedTowers.Remove(0);
	}

	Tower ** gridData = WorldGame.GetGridData();
	for (uint8 i = 1; i < GRID_SIZE - 1; i++)
	{
		for (uint8 j = 1; j < GRID_SIZE - 1; j++)
		{
			Tower * tower = gridData[(i*GRID_SIZE)+j];

			if (tower != NULL)
			{
				if (tower->GetState() == true)
				{
					tower->PerformAction();
				}
				else if (tower->GetState() == false)
				{
					Selection * selection = WorldGame.GetSelection();
					selection->Remove(tower);
					RefreshActiveAuras();

					RemovedTowers.Append(tower);
					gridData[(i*GRID_SIZE)+j] = NULL;
					
					Grid * grid = WorldGame.GetGrid();
					grid->ClearPath(0);
				}
			}
		}
	}
}

bool AddTowerKill(Object * object)
{
	if ((object != NULL) && (object->GetState() == true))
	{
		if (object->CheckType(OBJ_TYPE_TOWER) == true)
		{
			Tower * tower = (Tower *)object;
			tower->AddKill();
			return true;
		}
		else if (object->CheckType(OBJ_TYPE_PROJECTILE) == true)
		{
			Projectile * projectile = (Projectile *)object;
			Tower * tower = projectile->GetSource();
			if ((tower != NULL) && (tower->GetState() == true) && (tower->CheckType(OBJ_TYPE_TOWER) == true))
			{
				tower->AddKill();
				return true;
			}
		}
		else if (object->CheckType(OBJ_TYPE_GAME_OBJECT) == true)
		{
			GameObject * gameObject = (GameObject *)object;
			Tower * tower = (Tower *)gameObject->GetSource();
			if ((tower != NULL) && (tower->GetState() == true))
			{
				if (tower->CheckType(OBJ_TYPE_TOWER) == true)
				{
					tower->AddKill();
					return true;
				}
				else
				{
					return AddTowerKill(tower);
				}
			}
		}
	}
	return false;
}
//void RefreshActiveTowers()
//{
//	Tower ** gridData = WorldGame.GetGridData();
//	for (uint8 i = 1; i < GRID_SIZE - 1; i++)
//	{
//		for (uint8 j = 1; j < GRID_SIZE - 1; j++)
//		{
//			Tower * tower = gridData[(i*GRID_SIZE)+j];
//
//			/*if ((tower != NULL) && (tower->GetState() == true))
//			{
//				tower->Clear();
//				tower->RefreshActiveSpells();
//			}*/
//		}
//	}
//}

bool BuildTower(uint8 type, const Point2D& coords, bool active)
{
	if (WorldGame.GetInk() > TowerData[type].GetUInt32Value(TOWER_VAR_BASE_COST))
	{
		Grid * grid = WorldGame.GetGrid();

		bool ghostState = TowerData[type].GetInt32Value(TOWER_VAR_GHOST_STATE);

		bool passable = grid->FindPath(Origin, Origin, (ghostState == true) ? NullPoint : coords);
		grid->ClearPath(0);
		grid->FindPath(Origin, Origin, NullPoint);

		if (passable == true)
		{
			Tower ** gridData = WorldGame.GetGridData();
			List<Creep *> * ActiveCreeps = GetActiveCreeps();

			uint32 index = (coords.GetX() * GRID_SIZE) + coords.GetY();

			Rect towerSpot((coords.GetX() * 32) + 80, (coords.GetY() * 32) + 80, (coords.GetX() * 32) + 111.9999, (coords.GetY() * 32) + 111.9999);

			ActiveCreeps->ToStart();
			for (uint32 i = 0; i < ActiveCreeps->GetSize(); i++)
			{
				Creep * creep = ActiveCreeps->GetCurrent();
				ActiveCreeps->ToNext();

				if (creep->GetState() == true)
				{
					Point2D lastPos = creep->GetLastPos();
					Point2D moveTarget = creep->GetMoveTarget();
					Point2D position = creep->GetPosition();

					Float x = (int32)(lastPos.GetX() - 80)/32;
					Float y = (int32)(lastPos.GetY() - 80)/32;

					Point2D creepCoords(x, y);
					Rect creepSpot(position.GetX(), position.GetY(), position.GetX() + 31.9999, position.GetY() + 31.9999);

					bool creepPath = true;
					if (active == true)
					{
						creepPath = grid->FindPath(creepCoords, Origin, coords);
						grid->FindPath(Origin, Origin, NullPoint);
					}

					if (((ghostState == false) && (Intersect(&creepSpot, &towerSpot) == true)) || ((towerSpot.ContainsPoint(moveTarget)) && (ghostState == false)) || ((IntervalCheck(x, 0, GRID_SIZE) == true) && (IntervalCheck(y, 0, GRID_SIZE) == true) && (grid->GetPath(creepCoords) == 0) && (creepPath == false)))
					{
						return false;
					}
				}
			}

			if (gridData[index] == NULL)
			{
				if (active == true)
				{
					gridData[index] = new Tower;
					gridData[index]->SetPosition((coords.GetX() * 32) + 80, (coords.GetY() * 32) + 80);
					gridData[index]->Copy(TowerData[type]);
					gridData[index]->SetUInt32Value(TOWER_VAR_BUILD_TIME, GetGameTime());

					if ((IntervalCheck(coords.GetX(), 1, GRID_SIZE - 2) == true) && (IntervalCheck(coords.GetY(), 1, GRID_SIZE - 2) == true))
					{
						WorldGame.AddInk(-1 * (int32)TowerData[type].GetUInt32Value(TOWER_VAR_BASE_COST));
					}

					if (gridData[index]->GetInt32Value(TOWER_VAR_GHOST_STATE) == false)
					{
						grid->SetPath(coords, 1);
					}
					grid->FindPath(NullPoint);
					RefreshActiveAuras();
				}
				return true;
			}
		}
	}
	return false;
}

void UnloadTowerData()
{
	while (RemovedTowers.GetSize() > 0)
	{
		if (RemovedTowers[0] != NULL)
		{
			delete RemovedTowers[0];
		}
		RemovedTowers.Remove(0);
	}
	delete [] TowerData;
	towerDataSize = 0;
}



