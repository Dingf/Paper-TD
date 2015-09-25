#include "type.h"
#include "utility.h"
#include "point.h"
#include "rect.h"
#include "mouse.h"
#include "sprite.h"
#include "font.h"
//#include "cursor.h"
#include "rendergui.h"
#include "selection.h"
#include "game.h"

namespace RenderInfobox
{
	Sprite Castbar;
	Sprite Cross;
	Sprite Check;
}
using namespace RenderInfobox;


void InfoboxDisplayInit()
{
	Castbar.LoadSprite("Assets\\GUI\\castbar.tga");
	Cross.LoadSprite("Assets\\GUI\\cross.tga");
	Check.LoadSprite("Assets\\GUI\\check.tga");
}

void DrawSelectionTowers()
{
	const TowerBase * TowerData = GetTowerData();
	Point2D mousePos = WorldMouse.GetPosition();

	Selection * selection = WorldGame.GetSelection();
	for (uint32 i = 0; i < selection->GetSize(); i++)
	{
		Tower * tower = selection->GetTower(i);

		DrawGameIcon(tower->GetTowerType(), Point2D(792 + ((i % 4) * 54), 561 + ((i/4) * 57)));

		Rect iconBounds(792 + ((i % 4) * 54), 561 + ((i/4) * 57), 840 + ((i % 4) * 54), 609 + ((i/4) * 57));
		if (iconBounds.ContainsPoint(mousePos) == true)
		{
			char text[1024];

			Projectile * projectile = tower->GetProjectile();
			sprintf(text, "%s &cffff0000Lv%d&cff000000\n\n%s\n\nDamage: %.0f (%.0f spell)\nRange: %.0fpx\nAttack delay: %.2f sec.\nSplash radius: %.2fpx\nKills: %d", 
					tower->GetName(), 
					tower->GetTowerLevel(),
					TowerData[tower->GetTowerType()].GetDescription(),
					(tower->GetDamage() + projectile->GetInt32Value(PROJ_VAR_DAMAGE_FLAT)) * projectile->GetFloatValue(PROJ_VAR_DAMAGE_MULTIPLIER) * WorldGame.GetFloatValue(GAME_VAR_LC_MULTIPLIER_DAMAGE),
					tower->GetSpellDamage(),
					tower->GetRange(),
					tower->GetAttackTime()/1000.0,
					projectile->GetSplash(),
					tower->GetUInt32Value(TOWER_VAR_CREEP_KILLS));

			DrawTooltip(text, FeltPen[4]);
		}
	}
}

void DrawSingleTowerTooltip(const Spell * spell)
{
	char text[1024];
	const char * spellName = GetHotkeyedText(spell);

	Selection * selection = WorldGame.GetSelection();
	Tower * tower = selection->GetTower(0);
	const Spell * activeSpell = tower->GetActiveSpell();

	sprintf(text, "%s Lv%d",
			spellName, 
			spell->GetLevel());

	strcat(text, ((activeSpell != NULL) && (spell->GetID() == activeSpell->GetID())) ? " (&cffff0000Active&cff000000)\n\n" : "\n\n");

	delete [] spellName;

	if (tower->GetCastTarget() == spell)
	{
		int32 remainingTime = spell->GetCastTime() - (GetGameTime() - spell->GetLastCastTime());
		if (remainingTime > 0)
		{
			char castString[64];

			sprintf(castString, "&cffff0000Casting...&cff000000 %.1f sec. left.\n\n",
					remainingTime/1000.0f);

			strcat(text, castString);
		}
	}
	else 
	{
		if (spell->GetInt32Value(SPELL_VAR_COST) != 0)
		{
			char costString[64];

			int32 spellCost = spell->GetInt32Value(SPELL_VAR_COST);
			sprintf(costString, "%sCost: %d ink&cff000000\n\n", 
					(spellCost >= WorldGame.GetInk()) ? "&cffff0000" : "&cff00c000",
					spellCost);
			
			strcat(text, costString);
		}
		if (spell->GetCastTime() > 0)
		{
			char castString[64];

			sprintf(castString, "%&cffff0000Cast Time: %.1f sec.&cff000000\n\n", 
					(WorldGame.GetUInt32Value(GAME_VAR_INSTANT_CAST_FLAG) == true) ? 0 : (Float)spell->GetCastTime()/1000.0);
			
			strcat(text, castString);
		}
	}

	if (spell->CheckCooldown() == false)
	{
		char cdString[64];

		int32 remainingTime = spell->GetCooldown() - (GetGameTime() - spell->GetLastCastTime());
		if (remainingTime >= 60000)
		{
			sprintf(cdString, "&cffff0000Cooldown:&cff000000 %u min. %u sec.\n\n",
					(uint32)(remainingTime/60000.0), (uint32)((remainingTime % 60000)/1000.0));
		}
		else if (remainingTime > 0)
		{
			sprintf(cdString, "&cffff0000Cooldown:&cff000000 %u sec.\n\n",
					(uint32)(remainingTime/1000.0));
		}
		strcat(text, cdString);
	}
	if (spell->GetInt32Value(SPELL_VAR_CHARGES) > 0)
	{
		char chargeString[64];

		sprintf(chargeString, "&cffff0000Charges:&cff000000 %u\n\n",
					spell->GetUInt32Value(SPELL_VAR_REMAINING_CHARGES));

		strcat(text, chargeString);
	}

	strcat(text, spell->GetText());
	DrawTooltip(text, FeltPen[4]);
}

void DrawSingleTowerInfo()
{
	Point2D mousePos = WorldMouse.GetPosition();
	Selection * selection = WorldGame.GetSelection();
	Tower * tower = selection->GetTower(0); 

	Point2D iconPos;
	Rect iconBounds;

	iconPos.SetValues(846, 561);
	iconBounds.SetValues(iconPos.GetX(), iconPos.GetY(), iconPos.GetX() + 48, iconPos.GetY() + 48);
	DrawGameIcon(10, iconPos);

	if ((tower->GetInt32Value(TOWER_VAR_CAN_FIRE) == false) || (tower->GetActiveSpell() == NULL))
	{
		Cross.DrawSprite(Rect(0, 0, 48, 48), iconPos, NullPoint, NullPoint, 1.0, 0.0, White);
	}
	if (iconBounds.ContainsPoint(mousePos) == true)
	{
		if (tower->GetInt32Value(TOWER_VAR_CAN_FIRE) == false)
		{
			DrawTooltip("Cannot Attack\n\nThis tower cannot attack due to an effect.", FeltPen[4]);
		}
		else if (tower->GetActiveSpell() == NULL)
		{
			DrawTooltip("Cannot Attack\n\nThis tower cannot attack because it does not know an active spell.", FeltPen[4]);
		}
		else
		{
			DrawTooltip("&cffff0000A&cff000000ttack\n\nAttacks a selected creep with projectiles.", FeltPen[4]);
		}
	}

	iconPos.SetValues(900, 561);
	iconBounds.SetValues(iconPos.GetX(), iconPos.GetY(), iconPos.GetX() + 48, iconPos.GetY() + 48);
	DrawGameIcon(11, iconPos);

	if (tower->GetInt32Value(TOWER_VAR_CAN_SELL) == false)
	{
		Cross.DrawSprite(Rect(0, 0, 48, 48), iconPos, NullPoint, NullPoint, 1.0, 0.0, White);
	}
	if (iconBounds.ContainsPoint(mousePos) == true)
	{
		if (tower->GetInt32Value(TOWER_VAR_CAN_SELL) == false)
		{
			DrawTooltip("Cannot be Sold\n\nThis tower cannot be sold.", FeltPen[4]);
		}
		else
		{
			char text[256] = "";
			sprintf(text, "&cffff0000S&cff000000ell\n\nSell this tower for half of its current value. (%d ink)", tower->GetUInt32Value(TOWER_VAR_BASE_COST)/2.0);
			DrawTooltip(text, FeltPen[4]);
		}
	}

	iconPos.SetValues(954, 561);
	iconBounds.SetValues(iconPos.GetX(), iconPos.GetY(), iconPos.GetX() + 48, iconPos.GetY() + 48);
	DrawGameIcon(12, iconPos);

	if ((tower->GetInt32Value(TOWER_VAR_CAN_FIRE) == false) || (tower->GetActiveSpell() == NULL))
	{
		Cross.DrawSprite(Rect(0, 0, 48, 48), iconPos, NullPoint, NullPoint, 1.0, 0.0, White);
	}
	else if (tower->GetUInt32Value(TOWER_VAR_WAITING) == true)
	{
		Check.DrawSprite(Rect(0, 0, 48, 48), iconPos, NullPoint, NullPoint, 1.0, 0.0, White);
	}
	if (iconBounds.ContainsPoint(mousePos) == true)
	{
		if ((tower->GetInt32Value(TOWER_VAR_CAN_FIRE) == false) || (tower->GetActiveSpell() == NULL))
		{
			DrawTooltip("Cannot Wait\n\nThis tower cannot be ordered to wait because it cannot attack.", FeltPen[4]);
		}
		else if (tower->GetUInt32Value(TOWER_VAR_WAITING) == true)
		{
			DrawTooltip("Stop &cffff0000W&cff000000aiting\n\nOrders this tower to begin attacking creeps again.", FeltPen[4]);
		}
		else
		{
			DrawTooltip("&cffff0000W&cff000000ait\n\nOrders this tower to stop attacking creeps until ordered to again.", FeltPen[4]);
		}
	}
//
//	
//const char * defaultIconMessages[] = 
//{
//	"Cannot Attack\n\nThis tower cannot attack.",
//	"&cffff0000A&cff000000ttack\n\nAttacks a selected creep.",
//	"Cannot be Sold\n\nThis tower cannot be sold.",
//	"&cffff0000S&cff000000ell\n\nSell this tower for %.0f ink.",
//	"Cannot Wait\n\nThis tower cannot wait.",
//	"&cffff0000W&cff000000ait\n\nThis tower will not attack again unless ordered to.",
//};
//
//
//	for (uint8 i = 0; i < 3; i++)
//	{
//		iconPos.SetValues(846 + (54 * i), 561);
//		DrawGameIcon(10 + i, iconPos);
//
//		bool unavailable = (i == 2) ? (tower->GetInt32Value(TOWER_VAR_CAN_FIRE) == false) : (tower->GetInt32Value(TOWER_VAR_CAN_FIRE + i) == false);
//		if (unavailable == true)
//		{
//			Cross.DrawSprite(Rect(0, 0, 48, 48), iconPos, NullPoint, NullPoint, 1.0, 0.0, White);
//		}
//		iconBounds.SetValues(iconPos.GetX(), iconPos.GetY(), iconPos.GetX() + 48, iconPos.GetY() + 48);
//		if (iconBounds.ContainsPoint(mousePos) == true)
//		{
//			char text[1024];
//			sprintf(text, (unavailable == true) ? defaultIconMessages[i*2] : defaultIconMessages[(i*2)+1], tower->GetUInt32Value(TOWER_VAR_BASE_COST)/2.0);
//			DrawTooltip(text, FeltPen12);
//		}
//	}

	List<Spell *> * spellLists[] = { tower->GetUpgradeSpells(), tower->GetKnownSpells() };
	for (uint8 j = 0; j <= 1; j++)
	{
		uint8 iconCount = 0;
		spellLists[j]->ToStart();
		for (uint8 k = 0; (k < spellLists[j]->GetSize()) && (iconCount < 4); k++)
		{
			Spell * spell = spellLists[j]->GetCurrent();
			spellLists[j]->ToNext();

			if ((spell->GetUInt32Value(SPELL_VAR_VISIBILITY) == true) && (((j == 0) && (spell->GetMode() == SPELL_MODE_LEARN) && (tower->GetTowerLevel() < MAX_TOWER_LEVEL)) || ((j == 1) && (spell->GetMode() != SPELL_MODE_LEARN))))
			{
				iconPos.SetValues(792 + (iconCount * 54), 618 + (57 * j));
				if ((tower->GetCastTarget() == spell) && (spell->GetCastTime() > 0))
				{
					DrawGameIcon(0, iconPos);
					Castbar.DrawSprite(Rect(0, 0, 32, 32), Point2D(iconPos.GetX() + 8, iconPos.GetY() + 8), NullPoint, NullPoint, 1.0, 0.0, White);

					Float barCount = ((GetGameTime() - spell->GetLastCastTime())/(Float)spell->GetCastTime()) * 180.0;
					for (uint32 i = 0; i < barCount; i++)
					{
						Castbar.DrawSprite(Rect(63, 60, 64, 64), Point2D(iconPos.GetX() + 24, iconPos.GetY() + 8), NullPoint, Point2D(iconPos.GetX() + 24, iconPos.GetY() + 21.5), 1.0, i * 2.0, White);
					}

					char percentWord[4];
					sprintf(percentWord, "%.0f", MaxFit((GetGameTime() - spell->GetLastCastTime())/(Float)spell->GetCastTime() * 100.0, 100));
					FeltPen[4]->PrintText(percentWord, Point2D(iconPos.GetX(), iconPos.GetY() + 28), Rect(0, 0, 48, 16), FORMAT_CENTER);
				}
				else
				{
					DrawGameIcon(spell->GetInt32Value(SPELL_VAR_ICON_ID), iconPos);

					const Spell * activeSpell = tower->GetActiveSpell();
					if ((spell->CheckCooldown() == false) || (spell->CheckCharges() == false))
					{
						Cross.DrawSprite(Rect(0, 0, 48, 48), iconPos, NullPoint, NullPoint, 1.0, 0.0, White);
					}
					else if ((activeSpell != NULL) && (spell->GetID() == activeSpell->GetID()))
					{
						Check.DrawSprite(Rect(0, 0, 48, 48), iconPos, NullPoint, NullPoint, 1.0, 0.0, White);
					}
				}
				iconBounds.SetValues(iconPos.GetX(), iconPos.GetY(), iconPos.GetX() + 48, iconPos.GetY() + 48);
				iconCount++;

				if (iconBounds.ContainsPoint(mousePos) == true)
				{
					DrawSingleTowerTooltip(spell);
				}
			}
		}
	}
}

void DrawBuildInfo()
{
	DrawGameIcon(WorldGame.GetBuild(), Point2D(792, 561));

	const TowerBase * TowerData = GetTowerData();
	const TowerBase * SelectionTower = &TowerData[WorldGame.GetBuild()];
	const Projectile * SelectionProjectile = SelectionTower->GetProjectile();

	FeltPen[4]->PrintText(SelectionTower->GetName(), Point2D(852, 592), Rect(0, 0, 144, 1), FORMAT_LEFT);

	DrawBorderLine(153, Point2D(847, 600));

	char costStr[64];
	sprintf(costStr, "Cost: %d ink", SelectionTower->GetUInt32Value(TOWER_VAR_BASE_COST));
	FeltPen[4]->PrintText(costStr, Point2D(792, 640), Rect(0, 0, 207, 1), FORMAT_LEFT);
	char damageStr[64];
	sprintf(damageStr, "Damage: %.0f", SelectionTower->GetDamage());
	FeltPen[4]->PrintText(damageStr, Point2D(792, 664), Rect(0, 0, 207, 1), FORMAT_LEFT);
	char rangeStr[64];
	sprintf(rangeStr, "Range: %.0fpx", SelectionTower->GetRange());
	FeltPen[4]->PrintText(rangeStr, Point2D(792, 688), Rect(0, 0, 207, 1), FORMAT_LEFT);
	char attackTimeStr[64];
	sprintf(attackTimeStr, "Attack delay: %.2f sec.", SelectionTower->GetAttackTime()/1000.0);
	FeltPen[4]->PrintText(attackTimeStr, Point2D(792, 712), Rect(0, 0, 207, 1), FORMAT_LEFT);
}

void DrawInfobox()
{
	Selection * selection = WorldGame.GetSelection();

	DrawSelectionTowers();
	if (selection->GetSize() == 1)
	{
		DrawSingleTowerInfo();
	}
	else if ((selection->GetSize() == 0) && (WorldGame.GetBuild() != 0))
	{
		DrawBuildInfo();
	}
}

void InfoboxDisplayUnload()
{
	Castbar.Unload();
	Cross.Unload();
	Check.Unload();
}