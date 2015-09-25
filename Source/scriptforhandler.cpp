#include "type.h"
#include "script.h"
#include "scripthandler.h"
#include "animation.h"
#include "emitter.h"
#include "gameobject.h"
#include "creep.h"
#include "projectile.h"
#include "tower.h"
#include "game.h"

Float ForAllTowers(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * subscriptID = script->GetNextOriginalWord();
		Script * subscript = script->GetSubscript(subscriptID->value);

		if (subscript != NULL)
		{
			bool state = false;

			Grid * grid = WorldGame.GetGrid();
			Tower ** TowerData = grid->GetData();

			for (uint32 i = 1; i < GRID_SIZE - 1; i++)
			{
				for (uint32 j = 1; j < GRID_SIZE - 1; j++)
				{
					Tower * tower = TowerData[(i*GRID_SIZE)+j];
					if ((tower != NULL) && (tower->GetState() == true))
					{
						subscript->SetSource(tower);
						Float result = subscript->ActivateScript(false);

						if (result == true)
						{
							state = true;
						}
						else if (result == RETURN_STOP)
						{
							return true;
						}
					}
				}
			}
			return state;
		}
	}
	return false;
}

Float ForAllProjectiles(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * subscriptID = script->GetNextOriginalWord();
		Script * subscript = script->GetSubscript(subscriptID->value);

		if (subscript != NULL)
		{
			bool state = false;

			List<Projectile *> * ActiveProjectiles = GetActiveProjectiles();
			
			ActiveProjectiles->ToStart();
			for (uint32 i = 0; i < ActiveProjectiles->GetSize(); i++)
			{
				Projectile * projectile = ActiveProjectiles->GetCurrent();
				ActiveProjectiles->ToNext();

				if ((projectile != NULL) && (projectile->GetState() == true))
				{
					subscript->SetSource(projectile);
					Float result = subscript->ActivateScript(false);

					if (result == true)
					{
						state = true;
					}
					else if (result == RETURN_STOP)
					{
						return true;
					}
				}
			}

			return state;
		}
	}
	return false;
}

Float ForAllCreeps(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * subscriptID = script->GetNextOriginalWord();
		Script * subscript = script->GetSubscript(subscriptID->value);

		if (subscript != NULL)
		{
			bool state = false;

			List<Creep *> * ActiveCreeps = GetActiveCreeps();
			
			ActiveCreeps->ToStart();
			for (uint32 i = 0; i < ActiveCreeps->GetSize(); i++)
			{
				Creep * creep = ActiveCreeps->GetCurrent();
				ActiveCreeps->ToNext();

				if ((creep != NULL) && (creep->GetState() == true))
				{
					subscript->SetSource(creep);
					Float result = subscript->ActivateScript(false);

					if (result == true)
					{
						state = true;
					}
					else if (result == RETURN_STOP)
					{
						return true;
					}
				}
			}

			return state;
		}
	}
	return false;
}

Float ForAllGameObjects(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * subscriptID = script->GetNextOriginalWord();
		Script * subscript = script->GetSubscript(subscriptID->value);

		if (subscript != NULL)
		{
			bool state = false;

			List<GameObject *> * ActiveGameObjects = GetActiveGameObjects();
			
			ActiveGameObjects->ToStart();
			for (uint32 i = 0; i < ActiveGameObjects->GetSize(); i++)
			{
				GameObject * gameObj = ActiveGameObjects->GetCurrent();
				ActiveGameObjects->ToNext();

				if ((gameObj != NULL) && (gameObj->GetState() == true))
				{
					subscript->SetSource(gameObj);
					Float result = subscript->ActivateScript(false);

					if (result == true)
					{
						state = true;
					}
					else if (result == RETURN_STOP)
					{
						return true;
					}
				}
			}

			return state;
		}
	}
	return false;
}

Float ForAllAnimations(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * subscriptID = script->GetNextOriginalWord();
		Script * subscript = script->GetSubscript(subscriptID->value);

		if (subscript != NULL)
		{
			bool state = false;

			List<Animation *> * ActiveAnimations = GetActiveAnimations();
			
			ActiveAnimations->ToStart();
			for (uint32 i = 0; i < ActiveAnimations->GetSize(); i++)
			{
				Animation * animation = ActiveAnimations->GetCurrent();
				ActiveAnimations->ToNext();

				if ((animation != NULL) && (animation->GetState() == true))
				{
					subscript->SetSource(animation);
					Float result = subscript->ActivateScript(false);

					if (result == true)
					{
						state = true;
					}
					else if (result == RETURN_STOP)
					{
						return true;
					}
				}
			}

			return state;
		}
	}
	return false;
}

Float ForAllEmitters(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * subscriptID = script->GetNextOriginalWord();
		Script * subscript = script->GetSubscript(subscriptID->value);

		if (subscript != NULL)
		{
			bool state = false;

			List<Emitter *> * ActiveEmitters = GetActiveEmitters();
			
			ActiveEmitters->ToStart();
			for (uint32 i = 0; i < ActiveEmitters->GetSize(); i++)
			{
				Emitter * emitter = ActiveEmitters->GetCurrent();
				ActiveEmitters->ToNext();

				if ((emitter != NULL) && (emitter->GetState() == true))
				{
					subscript->SetSource(emitter);
					Float result = subscript->ActivateScript(false);

					if (result == true)
					{
						state = true;
					}
					else if (result == RETURN_STOP)
					{
						return true;
					}
				}
			}

			return state;
		}
	}
	return false;
}

Float ForAllKnownSpells(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * subscriptID = script->GetNextOriginalWord();
		Script * subscript = script->GetSubscript(subscriptID->value);

		Object * target = script->GetTarget(targetWord);

		if ((subscript != NULL) && (target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true))
		{
			bool state = false;
			SpellObject * spellObj = (SpellObject *)target;

			List<Spell *> * KnownSpells = spellObj->GetKnownSpells();
			
			subscript->SetSourceFlag(true);
			KnownSpells->ToStart();
			for (uint32 i = 0; i < KnownSpells->GetSize(); i++)
			{
				Spell * spell = KnownSpells->GetCurrent();
				KnownSpells->ToNext();

				if ((spell != NULL) && (spell->GetState() == true))
				{
					subscript->SetSource(spell);
					Float result = subscript->ActivateScript(false);

					if (result == true)
					{
						state = true;
					}
					else if (result == RETURN_STOP)
					{
						return true;
					}
				}
			}

			subscript->SetSourceFlag(false);
			return state;
		}
	}
	return false;
}

Float ForAllActiveSpells(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * subscriptID = script->GetNextOriginalWord();
		Script * subscript = script->GetSubscript(subscriptID->value);

		Object * target = script->GetTarget(targetWord);

		if ((subscript != NULL) && (target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true))
		{
			bool state = false;
			SpellObject * spellObj = (SpellObject *)target;

			List<Spell *> * ActiveSpells = spellObj->GetActiveSpells();
			
			subscript->SetSourceFlag(true);
			ActiveSpells->ToStart();
			for (uint32 i = 0; i < ActiveSpells->GetSize(); i++)
			{
				Spell * spell = ActiveSpells->GetCurrent();
				ActiveSpells->ToNext();

				if ((spell != NULL) && (spell->GetState() == true))
				{
					subscript->SetSource(spell);
					Float result = subscript->ActivateScript(false);

					if (result == true)
					{
						state = true;
					}
					else if (result == RETURN_STOP)
					{
						return true;
					}
				}
			}

			subscript->SetSourceFlag(false);
			return state;
		}
	}
	return false;
}