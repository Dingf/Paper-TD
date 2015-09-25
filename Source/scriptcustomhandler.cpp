#include <string>
#include "type.h"
#include "rand.h"
#include "list.h"
#include "script.h"
#include "scriptdef.h"
#include "font.h"
#include "spell.h"
#include "spellobject.h"
#include "creep.h"
#include "tower.h"
#include "game.h"
//
//Float GetTextHeight(Script * script)
//{
//	if (script->VerifyArguments(2) == true)
//	{
//		Word * sizeWord = script->GetNextWord();
//		Word * widthWord = script->GetNextWord();
//
//		std::string string;
//		//GetScriptText(script, index + 2, string);
//
//		uint8 size = IntervalFit(sizeWord->value, 8, 47);
//		return FeltPen[size-8]->CalculateTextHeight(string.c_str(), widthWord->value);
//	}
//	return false;
//}

//Float WhiteoutPurge(Script * script)
//{
//	SpellObject * target = (SpellObject *)script->GetSource();
//
//	if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true))
//	{
//		target->ClearActiveSpells();
//		target->Clear();
//		return true;
//	}
//	return false;
//}

Float Spellsnatch(Script * script)
{
	SpellObject * selection = (SpellObject *)script->GetSelection();
	SpellObject * target = (SpellObject *)script->GetSource();

	if ((selection != NULL) && (target != NULL) && (selection->GetState() == true) && (target->GetState() == true) && (selection->CheckType(OBJ_TYPE_SPELL_OBJ) == true) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true))
	{
		List<Spell *> * KnownSpells = target->GetKnownSpells();
		List<Spell *> availableSpells;

		Word * levelWord = script->GetVariable("MAX_LEVEL");
		uint8 level = levelWord->value;

		KnownSpells->ToStart();
		for (uint32 i = 0; i < KnownSpells->GetSize(); i++)
		{
			Spell * spell = KnownSpells->GetCurrent();
			KnownSpells->ToNext();

			if ((spell->GetLevel() <= level) && (spell->GetLevel() > 1) && (spell->GetMode() != SPELL_MODE_LEARN) && (spell->GetUInt32Value(SPELL_VAR_VISIBILITY) == true))
			{
				if (selection->KnowsSpell(spell) == false)
				{
					const Spell * baseSpell = GetSpell(spell->GetID());
					availableSpells.Append(baseSpell->CreateInstance(selection, selection));

					Spell * spellInst = availableSpells.GetLast();
					if (spellInst->GetInt32Value(SPELL_VAR_HOTKEY) != NULL)
					{
						spellInst->SetInt32Value(SPELL_VAR_HOTKEY, 'C');
					}
				}
			}
		}

		if (availableSpells.GetSize() > 0)
		{
			uint32 index = RandomRange(0, availableSpells.GetSize());
			bool result = selection->LearnSpell(availableSpells[index], selection);

			while (availableSpells.GetSize() > 0)
			{
				delete availableSpells[0];
				availableSpells.Remove(0);
			}
			return result;
		}
	}
	return false;
}

Float Wonder(Script * script)
{
	SpellObject * source = (SpellObject *)script->GetSource();

	if ((source != NULL) && (source->GetState() == true) && (source->CheckType(OBJ_TYPE_SPELL_OBJ) == true))
	{
		WorldGame.RemoveTriggeredScript(GAME_TRIGGER_ON_CLICK);

		List<uint32> possibleSpells;

		possibleSpells.Append(64);		//Tweak Weapon Lv5
		possibleSpells.Append(85);		//Overflow Lv5
		possibleSpells.Append(94);		//Flashflood Lv5
		possibleSpells.Append(124);		//Stasis Lv5
		possibleSpells.Append(137);		//Celerity Lv5
		possibleSpells.Append(146);		//Time Restraint Lv5
		possibleSpells.Append(150);		//Chronosphere Lv5
		possibleSpells.Append(176);		//Worldbreaker Lv5
		possibleSpells.Append(177);		//Updraft Lv1
		possibleSpells.Append(182);		//Lightning Bolt Lv5
		possibleSpells.Append(186);		//Discharge Lv5
		possibleSpells.Append(189);		//Turbulence Lv5
		possibleSpells.Append(209);		//Blizzard Lv5
		possibleSpells.Append(215);		//Fimbulwinter Lv5
		possibleSpells.Append(228);		//Memento Mori Lv5
		possibleSpells.Append(231);		//Borrowed Time Lv5
		possibleSpells.Append(272);		//Prominence Lv5
		possibleSpells.Append(295);		//Hallowing Light Lv5
		possibleSpells.Append(298);		//Apotheosis Lv5
		possibleSpells.Append(300);		//Stigmata Lv1
		possibleSpells.Append(320);		//Life Tap Lv5
		possibleSpells.Append(367);		//Repel Lv5
		possibleSpells.Append(369);		//Singularity Lv5

		/*NOTE:
		  *Whiteout is not on the list because its effect stops many other beneficial effects
		  (such as Fimbulwinter, Worldbreaker, and any other non-spellarea effect)
		  *Redemption and Truefaith are not on the list of possible spells because
		  their effects would work against many of the other triggered abilities that deal
		  damage/kill creeps.
		  *Teleport is not on the list of possible spells because its effect is usually not 
		  selected when casting Wonder (only when casting on an empty square), and its effect
		  may not always be desired.
		  *The Night Tower chain spells (except for Life Tap) are not on the list of possible
		  spells because they require that the casting tower both know the prerequisite spell 
		  and have said spell on cooldown.
		  *Soul Eraser is not on the list of possible spells because its effect is highly
		  unlikely to occur, even if the user selects a creep with Wonder.
		  *Spellsnatch is not on the list of possible spells because it would result in a free
		  spell for the casting tower, potentially crowding out other spells as well.
		  *Mirage is not on the list of possible spells to prevent prevent unintended reuse of
		  charged spells (including Mirage itself). 
		  *Wonder is not on the list of possible spells for obvious reasons.
		*/

		List<Spell *> spellInsts;
		for (uint8 i = 0; i < 5; i++)
		{
			uint32 index = RandomRange(0, possibleSpells.GetSize());
			const Spell * spell = GetSpell(possibleSpells[index]);
			possibleSpells.Remove(index);

			Spell * spellInst = spell->CreateInstance(source, source);
			spellInsts.Append(spellInst);

			spellInst->Activate();
		}

		WorldGame.ActivateTriggeredScript(GAME_TRIGGER_ON_CLICK);
		WorldGame.RemoveTriggeredScript(GAME_TRIGGER_ON_CLICK);

		while (spellInsts.GetSize() > 0)
		{
			delete spellInsts[0];
			spellInsts.Remove(0);
		}
		possibleSpells.Unload();

		return true;
	}
	return false;
}

Float Polymorph(Script * script)
{
	SpellObject * target = (SpellObject *)script->GetSource();

	if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_CREEP) == true))
	{
		target->SetUInt32Value(CREEP_VAR_TYPE, RandomRange(1, 10));
		return true;
	}
	return false;
}
//
//Float DrawCreeps(Script * script, uint32 index)
//{
//	Animation * source = (Animation *)script->GetSource();
//
//	if ((source != NULL) && (source->GetState() == true) && (source->CheckType(OBJ_TYPE_ANIMATION) == true))
//	{
//		List<Creep *> * ActiveCreeps = GetActiveCreeps();
//
//		ActiveCreeps->ToStart();
//		for (uint32 i = 0; i < ActiveCreeps->GetSize(); i++)
//		{
//			Creep * creep = ActiveCreeps->GetCurrent();
//			ActiveCreeps->ToNext();
//
//			if ((creep != NULL) && (creep->GetState() == true))
//			{
//				Point2D moveTarget = creep->GetMoveTarget();
//				Point2D creepPos = creep->GetPosition();
//
//				Vector3D vector(creepPos, moveTarget);
//				Float angle = 270 - vector.GetZeroAngleD();
//
//				int8 type = creep->GetCreepType() - 1;
//
//				Sprite * CreepSprite = source->GetSprite();
//				uint32 creepsPerRow = CreepSprite->GetWidth()/32;
//
//				uint32 x = (type % creepsPerRow) * 32;
//				uint32 y = (type/creepsPerRow) * 32;
//
//				source->SetBounds(Rect(x, y, x + 32, y + 32));
//				source->ModFloatValue(ANIM_VAR_ROTATION, angle);
//
//				source->DrawAnimation(creepPos);
//			}
//		}
//		return true;
//	}
//	return false;
//}
//
//Float DrawTowers(Script * script, uint32 index)
//{
//	Animation * source = (Animation *)script->GetSource();
//
//	if ((source != NULL) && (source->GetState() == true) && (source->CheckType(OBJ_TYPE_ANIMATION) == true))
//	{
//		Selection * selection = WorldGame.GetSelection();
//		Tower ** gridData = WorldGame.GetGridData();
//
//		Grid * grid = WorldGame.GetGrid();
//
//		for (uint8 i = 1; i < GRID_SIZE - 1; i++)
//		{
//			for (uint8 j = 1; j < GRID_SIZE - 1; j++)
//			{
//				Tower * tower = gridData[(i*GRID_SIZE)+j];
//
//				if ((tower != NULL) && (tower->GetState() == true))
//				{
//					Point2D iconPos(110 + (32 * (i - 1)), 114 + (32 * (j - 1)));
//
//					const Spell * castTarget = tower->GetCastTarget();
//					if (tower->GetInt32Value(TOWER_VAR_GHOST_STATE) == true)
//					{
//						source->ModFloatValue(ANIM_VAR_OPACITY, -255);
//					}
//
//					if (castTarget != NULL)
//					{
//						source->SetBounds(Rect(0, 0, 48, 48));
//
//						/*Castbar.DrawSprite(Rect(0, 0, 32, 32), Point2D(iconPos.GetX() + 6, iconPos.GetY() + 6),  Point2D(iconPos.GetX() + 6, iconPos.GetY() + 6), NullPoint, 0.66, 0.0, color);
//
//						Float barCount = ((GetGameTime() - castTarget->GetLastCastTime())/(Float)castTarget->GetCastTime()) * 180.0;
//						for (uint32 i = 0; i < barCount; i++)
//						{
//							Castbar.DrawSprite(Rect(63, 60, 64, 64), Point2D(iconPos.GetX() + 16, iconPos.GetY() + 6), Point2D(iconPos.GetX() + 16, iconPos.GetY() + 6), Point2D(iconPos.GetX() + 16, iconPos.GetY() + 14), 0.6666, i * 2.0, color);
//						}*/
//					}
//					else
//					{
//						Sprite * IconSprite = source->GetSprite();
//						uint32 iconsPerRow = IconSprite->GetWidth()/32;
//
//						Float x = (int32)(tower->GetUInt32Value(TOWER_VAR_ICON_ID) % iconsPerRow) * 48;
//						Float y = (int32)(tower->GetUInt32Value(TOWER_VAR_ICON_ID) / iconsPerRow) * 48;
//
//						source->SetBounds(Rect(x, y, x + 48, y + 48));
//						
//						/*DrawGameIcon(, iconPos, 32, color);
//						if (tower->GetTowerType() != 1)
//						{
//							iconPos.SetValues(iconPos.GetX() + 2, iconPos.GetY() + 25);
//							for (uint32 i = 0; ((i < tower->GetTowerLevel()) && (i < MAX_TOWER_LEVEL)); i++)
//							{
//								LevelDot.DrawSprite(Rect(0, 0, 5, 5), iconPos, NullPoint, NullPoint, 1.0, 0.0, color);
//								iconPos.SetValues(iconPos.GetX() + 6, iconPos.GetY());
//							}
//						}*/
//					}
//					source->DrawAnimation(iconPos);
//				}
//			}
//		}
//		return true;
//	}
//	return false;
//}

Word CustomScriptTable[] =
{
	{"NULL",							FLOAT_VAR,			0,									&TakeNoAction,					NULL					},
	{"SPELLSNATCH",						FLOAT_VAR,			1,									&Spellsnatch,					NULL					},
	{"WONDER",							FLOAT_VAR,			2,									&Wonder,						NULL					},
	{"POLYMORPH",						FLOAT_VAR,			3,									&Polymorph,						NULL					},
	
	/*
	//{"WHITEOUT",						FLOAT_VAR,			1,									&WhiteoutPurge,					NULL					},
	{5,								"DRAW_CREEPS",			NORMAL,				&DrawCreeps					},
	{6,								"DRAW_TOWERS",			NORMAL,				&DrawTowers					},*/
};

Float ActivateCustomScript(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		uint16 listSize = sizeof(CustomScriptTable)/sizeof(Word);

		if (value->value < listSize)
		{
			return CustomScriptTable[(uint32)value->value].handler(script);
		}
	}
	return false;
}