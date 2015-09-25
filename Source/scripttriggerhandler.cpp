//#include "type.h"
//#include "spellobject.h"
//#include "script.h"
//#include "scripthandler.h"
//#include "game.h"
//
//Float TriggerOnHit(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 1) == true)
//	{
//		Word * subscriptID = script->GetOriginalWord(index + 1);
//		Script * subscript = script->GetSubscript(subscriptID->value);
//
//		SpellObject * target = (SpellObject *)script->GetSource();
//
//		if ((subscript != NULL) && (target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_PROJECTILE) == true))
//		{
//			Spell * spell = (Spell *)script->GetOriginalSource();
//			if (spell->CheckType(OBJ_TYPE_SPELL) == true)
//			{
//				Script * newScript = new Script;
//				newScript->LoadScript(subscript->GetScript(), target);
//
//				for (uint32 i = 0; i < target->GetTriggerAmount(PROJ_TRIGGER_ON_HIT); i++)
//				{
//					target->AddTriggeredScript(PROJ_TRIGGER_ON_HIT, newScript, spell);
//				}
//				delete newScript;
//				return false;
//			}
//		}
//	}
//	return false;
//}
//
//Float TriggerOnExplode(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 1) == true)
//	{
//		Word * subscriptID = script->GetOriginalWord(index + 1);
//		Script * subscript = script->GetSubscript(subscriptID->value);
//
//		SpellObject * target = (SpellObject *)script->GetSource();
//
//		if ((subscript != NULL) && (target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_PROJECTILE) == true))
//		{
//			Spell * spell = (Spell *)script->GetOriginalSource();
//			if (spell->CheckType(OBJ_TYPE_SPELL) == true)
//			{
//				Script * newScript = new Script;
//				newScript->LoadScript(subscript->GetScript(), target);
//
//				for (uint32 i = 0; i < target->GetTriggerAmount(PROJ_TRIGGER_ON_EXPLODE); i++)
//				{
//					target->AddTriggeredScript(PROJ_TRIGGER_ON_EXPLODE, newScript, spell);
//				}
//				delete newScript;
//				return false;
//			}
//		}
//	}
//	return false;
//}
//
//Float TriggerOnFire(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 1) == true)
//	{
//		Word * subscriptID = script->GetOriginalWord(index + 1);
//		Script * subscript = script->GetSubscript(subscriptID->value);
//
//		SpellObject * target = (SpellObject *)script->GetSource();
//
//		if ((subscript != NULL) && (target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_TOWER) == true))
//		{
//			Spell * spell = (Spell *)script->GetOriginalSource();
//			if (spell->CheckType(OBJ_TYPE_SPELL) == true)
//			{
//				Script * newScript = new Script;
//				newScript->LoadScript(subscript->GetScript(), target);
//
//				for (uint32 i = 0; i < target->GetTriggerAmount(TOWER_TRIGGER_ON_FIRE); i++)
//				{
//					target->AddTriggeredScript(TOWER_TRIGGER_ON_FIRE, newScript, spell);
//				}
//				delete newScript;
//				return false;
//			}
//		}
//	}
//	return false;
//}
//
//Float TriggerOnUpgrade(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 1) == true)
//	{
//		Word * subscriptID = script->GetOriginalWord(index + 1);
//		Script * subscript = script->GetSubscript(subscriptID->value);
//
//		SpellObject * target = (SpellObject *)script->GetSource();
//
//		if ((subscript != NULL) && (target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_TOWER) == true))
//		{
//			Spell * spell = (Spell *)script->GetOriginalSource();
//			if (spell->CheckType(OBJ_TYPE_SPELL) == true)
//			{
//				Script * newScript = new Script;
//				newScript->LoadScript(subscript->GetScript(), target);
//
//				for (uint32 i = 0; i < target->GetTriggerAmount(TOWER_TRIGGER_ON_UPGRADE); i++)
//				{
//					target->AddTriggeredScript(TOWER_TRIGGER_ON_UPGRADE, newScript, spell);
//				}
//				delete newScript;
//				return false;
//			}
//		}
//	}
//	return false;
//}
//
//Float TriggerOnSell(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 1) == true)
//	{
//		Word * subscriptID = script->GetOriginalWord(index + 1);
//		Script * subscript = script->GetSubscript(subscriptID->value);
//
//		SpellObject * target = (SpellObject *)script->GetSource();
//
//		if ((subscript != NULL) && (target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_TOWER) == true))
//		{
//			Spell * spell = (Spell *)script->GetOriginalSource();
//			if (spell->CheckType(OBJ_TYPE_SPELL) == true)
//			{
//				Script * newScript = new Script;
//				newScript->LoadScript(subscript->GetScript(), target);
//
//				for (uint32 i = 0; i < target->GetTriggerAmount(TOWER_TRIGGER_ON_SELL); i++)
//				{
//					target->AddTriggeredScript(TOWER_TRIGGER_ON_SELL, newScript, spell);
//				}
//				delete newScript;
//				return false;
//			}
//		}
//	}
//	return false;
//}
//
////Float TriggerOnClick(Script * script, uint32 index)
////{
////	if (VerifyArguments(script, index, 1) == true)
////	{
////
////	}
////}
//
//Float TriggerOnKill(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 1) == true)
//	{
//		Word * subscriptID = script->GetOriginalWord(index + 1);
//		Script * subscript = script->GetSubscript(subscriptID->value);
//
//		SpellObject * target = (SpellObject *)script->GetSource();
//
//		if ((subscript != NULL) && (target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_TOWER) == true))
//		{
//			Spell * spell = (Spell *)script->GetOriginalSource();
//			if (spell->CheckType(OBJ_TYPE_SPELL) == true)
//			{
//				Script * newScript = new Script;
//				newScript->LoadScript(subscript->GetScript(), target);
//
//				for (uint32 i = 0; i < target->GetTriggerAmount(TOWER_TRIGGER_ON_SELL); i++)
//				{
//					target->AddTriggeredScript(TOWER_TRIGGER_ON_KILL, newScript, spell);
//				}
//				delete newScript;
//				return false;
//			}
//		}
//	}
//	return false;
//}
//
//Float TriggerOnDeath(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 1) == true)
//	{
//		Word * subscriptID = script->GetOriginalWord(index + 1);
//		Script * subscript = script->GetSubscript(subscriptID->value);
//
//		SpellObject * target = (SpellObject *)script->GetSource();
//
//		if ((subscript != NULL) && (target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_CREEP) == true))
//		{
//			Spell * spell = (Spell *)script->GetOriginalSource();
//			if (spell->CheckType(OBJ_TYPE_SPELL) == true)
//			{
//				Script * newScript = new Script;
//				newScript->LoadScript(subscript->GetScript(), target);
//
//				for (uint32 i = 0; i < target->GetTriggerAmount(CREEP_TRIGGER_ON_DEATH); i++)
//				{
//					target->AddTriggeredScript(CREEP_TRIGGER_ON_DEATH, newScript, spell);
//				}
//				delete newScript;
//				return false;
//			}
//		}
//	}
//	return false;
//}
//
//Float TriggerOnClick(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 1) == true)
//	{
//		Word * subscriptID = script->GetOriginalWord(index + 1);
//		Script * subscript = script->GetSubscript(subscriptID->value);
//
//		SpellObject * target = (SpellObject *)script->GetSource();
//
//		if ((subscript != NULL) && (target != NULL) && (target->GetState() == true))
//		{
//			Spell * spell = (Spell *)script->GetOriginalSource();
//			if (spell->CheckType(OBJ_TYPE_SPELL) == true)
//			{
//				Script * newScript = new Script;
//				newScript->LoadScript(subscript->GetScript(), target);
//
//				for (uint32 i = 0; i < target->GetTriggerAmount(GAME_TRIGGER_ON_CLICK); i++)
//				{
//					WorldGame.AddTriggeredScript(GAME_TRIGGER_ON_CLICK, newScript, spell);
//				}
//				delete newScript;
//				return false;
//			}
//		}
//	}
//	return false;
//}