#include "type.h"
#include "point.h"
#include "rect.h"
#include "mouse.h"
#include "mousehandler.h"
#include "game.h"

//To be deleted later
#include "creep.h"

bool MouseNamespace::CastInfoboxSpell(uint8 button, const Point2D& position)
{
	Selection * selection = WorldGame.GetSelection();
	Tower * tower = selection->GetTower(0);

	if (selection->GetSize() == 1)
	{
		List<Spell *> * spellLists[] = { tower->GetUpgradeSpells(), tower->GetKnownSpells() };

		for (uint8 i = 0; i <= 1; i++)
		{
			for (uint8 j = 0; j < 4; j++)
			{
				Rect bounds(792 + (j * 54), 618 + (i * 57), 840 + (j * 54), 666 + (i * 57));
				if ((j < spellLists[i]->GetSize()) && (bounds.ContainsPoint(WorldMouse.GetPosition()) == true))
				{
					Spell * spell = tower->GetVisibleSpell(i, j);
					if ((spell != NULL) && (spell->CheckCooldown() == true) && (spell->CheckCharges() == true))
					{
						if (spell == tower->GetCastTarget())
						{
							Float percent = 1.00 - (GetGameTime() - spell->GetLastCastTime())/(Float)spell->GetCastTime();
							WorldGame.AddInk(spell->GetSpellCost() * percent);

							tower->InterruptCast();
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

bool MouseNamespace::BuildSelect(uint8 button, const Point2D& position)
{
	Point2D mousePos = WorldMouse.GetPosition();
	for (uint8 i = 0; i < 3; i++)
	{
		for (uint8 j = 0; j < 3; j++)
		{
			Rect bounds(789 + (84 * j), 447 - (72 * i), 837 + (84 * j), 495 - (72 * i));
			if (bounds.ContainsPoint(mousePos) == true)
			{
				WorldGame.SetBuild((i * 3) + j + 1);
				return true;
			}
		}
	}
	return false;
}

bool MouseNamespace::ClearBuildSelection(uint8 button, const Point2D& position)
{
	WorldGame.ClearBuild();
	return true;
}