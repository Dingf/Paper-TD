#include "type.h"
#include "utility.h"
#include "keyboard.h"
#include "keyboardhandler.h"
#include "spell.h"
#include "tower.h"
#include "selection.h"
#include "game.h"

bool KeyboardNamespace::NumberHotkeys()
{
	WorldGame.SetBuild(GetLastNormalKey() - 48);
	return true;
}

bool KeyboardNamespace::SingleTowerSpellHotkeys()
{
	Selection * selection = WorldGame.GetSelection();
	uint8 key = ToUpper(GetLastNormalKey());
	
	if (selection->GetSize() == 1)
	{
		Tower * tower = selection->GetTower(0);
		//const List<Spell *> * spellLists[] = { tower->GetUpgradeSpells(), tower->GetKnownSpells() };

		for (uint8 i = 0; i <= 1; i++)
		{
			for (uint8 j = 0; j < 4; j++)
			{
				Spell * spell = tower->GetVisibleSpell(i, j);

				if ((spell != NULL) && (spell->CheckCooldown() == true) && (spell->CheckCharges() == true))
				{
					uint8 hotkey = spell->GetInt32Value(SPELL_VAR_HOTKEY);
					if (key == hotkey)
					{
						if (spell == tower->GetCastTarget())
						{
							Float percent = 1.00 - (GetGameTime() - spell->GetLastCastTime())/(Float)spell->GetCastTime();
							WorldGame.AddInk(spell->GetSpellCost() * percent);

							tower->InterruptCast();
							return true;
						}
						else if ((tower->GetCastTarget() == NULL) && (spell->GetMode() != SPELL_MODE_PASSIVE))
						{
							return tower->CastSpell(spell);
						}
					}
				}
			}
		}

	}
	return false;
}

bool KeyboardNamespace::TowerHotkeys()
{
	const TowerBase * TowerData = GetTowerData();
	uint8 key = ToUpper(GetLastNormalKey());

	for (uint32 i = 0; i < GetTowerDataSize(); i++)
	{
		uint8 hotkey = TowerData[i].GetHotkey();
		if ((hotkey == key) && (IntervalCheck(i, 1, GetTowerDataSize()) == true))
		{
			WorldGame.SetBuild(i);
			return true;
		}
	}
	return false;
}