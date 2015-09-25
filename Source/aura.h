#ifndef INC_AURA_H
#define INC_AURA_H

#include "type.h"
#include "list.h"
#include "animation.h"
#include "spellobject.h"
#include "spell.h"

class Aura
{
	public:
		Aura(const Spell * effect, const Spell * auraSpell, const Animation * animation, SpellObject * source, Float radius);
		~Aura();

		const Spell * GetEffect() const { return effect; }
		const Spell * GetAuraSpell() const { return auraSpell; }

		      SpellObject * GetSource()       { return source; }
		const SpellObject * GetSource() const { return source; }

			  Animation * GetAnimation()       { return animation; }
		const Animation * GetAnimation() const { return animation; }

		Float GetRadius() const { return radius; }
		
		void ApplyAura();
		void RemoveAura();

		void Unload();
	private:
		const Spell * effect;
		const Spell * auraSpell;
		Animation * animation;
		SpellObject * source;
		Float radius;
};

List<Aura *> * GetActiveAuras();
void ClearActiveAuras();

void AuraMaintenance();
void RefreshActiveAuras();

#endif