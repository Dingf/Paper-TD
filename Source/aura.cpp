#include "type.h"
#include "list.h"
#include "spellobject.h"
#include "spell.h"
#include "aura.h"
#include "tower.h"
#include "grid.h"
#include "game.h"

List<Aura *> ActiveAuras;

Aura::Aura(const Spell * effect, const Spell * auraSpell, const Animation * animation, SpellObject * source, Float radius)
{
	this->effect = effect;
	this->auraSpell = auraSpell;
	this->source = source;
	this->radius = radius;

	this->animation = animation->AddAnimation(source);
}

Aura::~Aura()
{
	Unload();
}

void Aura::ApplyAura()
{
	if ((effect != NULL) && (source != NULL) && (effect->GetID() > 0))
	{
		Tower ** gridData = WorldGame.GetGridData();
		Float radius = GetRadius();

		for (uint8 i = 1; i < GRID_SIZE - 1; i++)
		{
			for (uint8 j = 1; j < GRID_SIZE - 1; j++)
			{
				Tower * tower = gridData[(i*GRID_SIZE)+j];
				if ((tower != NULL) && (tower->GetState() == true))
				{
					Float distance = CalculateDistance(tower->GetPosition(), source->GetPosition());
					if ((distance <= radius) && (tower->ActiveSpell(effect->GetID()) == false))
					{
						List<Spell *> * ActiveSpells = tower->GetActiveSpells();

						if (effect->GetCategory() != NULL)
						{
							ActiveSpells->ToStart();
							for (uint32 k = 0; k < ActiveSpells->GetSize(); k++)
							{
								Spell * spell = ActiveSpells->GetCurrent();
								ActiveSpells->ToNext();

								if ((spell->GetCategory() != NULL) && (spell->GetCategory() == effect->GetCategory()) && (spell->GetLevel() < effect->GetLevel()))
								{
									tower->RemoveSpell(k);
								}
							}
						}
						tower->ApplySpell(effect, source);
					}
					else if (distance > radius)
					{
						tower->RemoveSpell(effect, source);
					}
				}
			}
		}
	}
}

void Aura::RemoveAura()
{
	Tower ** gridData = WorldGame.GetGridData();

	for (uint8 i = 1; i < GRID_SIZE - 1; i++)
	{
		for (uint8 j = 1; j < GRID_SIZE - 1; j++)
		{
			Tower * tower = gridData[(i*GRID_SIZE)+j];
			if ((tower != NULL) && (tower->GetState() == true))
			{
				tower->RemoveSpell(effect, source);
			}
		}
	}
}

void Aura::Unload()
{
}

List<Aura *> * GetActiveAuras()
{
	return &ActiveAuras;
}

void AuraMaintenance()
{
	bool state = false;

	ActiveAuras.ToStart();
	for (uint32 i = 0; i < ActiveAuras.GetSize(); i++)
	{
		Aura * aura = ActiveAuras.GetCurrent();
		SpellObject * source = aura->GetSource();
		ActiveAuras.ToNext();

		if ((source == NULL) || (source->GetState() == false) || (source->KnowsSpell(aura->GetAuraSpell()) == false))
		{
			Animation * animation = aura->GetAnimation();

			animation->Unload();
			aura->RemoveAura();
			delete aura;
			state = true;
			ActiveAuras.Remove(i);
			i--;
		}
	}

	if (state == true)
	{
		RefreshActiveAuras();
	}
}

void RefreshActiveAuras()
{
	ActiveAuras.ToStart();
	for (uint32 i = 0; i < ActiveAuras.GetSize(); i++)
	{
		Aura * aura = ActiveAuras.GetCurrent();
		ActiveAuras.ToNext();

		aura->ApplyAura();
	}
}

void ClearActiveAuras()
{
	while (ActiveAuras.GetSize() > 0)
	{
		if (ActiveAuras[0] != NULL)
		{
			delete ActiveAuras[0];
		}
		ActiveAuras.Remove(0);
	}
}
