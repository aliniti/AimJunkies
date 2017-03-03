#pragma once
#include "JinxMenu.h"
#include "JinxSpells.h"
#include "JinxExtensions.h"
#include "JinxModes.h"

class Jinx
{
public:
	~Jinx();

	static void OnDraw();
	static void GetHeroes();
	static int RocketRange();

	static IUnit* Player;
	static JinxMenu* Menu;
	static JinxSpells* Spells;
	static JinxModes* Modes;
	static JinxExtensions* Ex;

	static std::vector<IUnit*> Allies;
	static std::vector<IUnit*> Enemies;

	static void UseQ(IUnit* unit, bool rockets, bool minion, int mana);
	static void UseW(IUnit* unit, int mana);
	static void UseE(IUnit* unit, int mana);

	static bool CanUlt(IUnit* unit);
	static float UltDamage(IUnit* unit);
};

inline void Jinx::OnDraw()
{
	
}

inline void Jinx::GetHeroes()
{
	for (auto entry : GEntityList->GetAllUnits())
	{
		if (entry->UnitFlags() == FL_HERO && entry->GetTeam() != Player->GetTeam())
		{
			Enemies.push_back(entry);
		}
	}

	for (auto entry : GEntityList->GetAllUnits())
	{
		if (entry->UnitFlags() == FL_HERO && entry->GetTeam() == Player->GetTeam())
		{
			Allies.push_back(entry);
		}
	}
}

inline int Jinx::RocketRange()
{
	int xtradist[6] = {75, 75, 100, 125, 150, 175};
	return xtradist[Player->GetSpellBook()->GetLevel(kSlotQ)];
}


inline void Jinx::UseQ(IUnit* unit, bool rockets, bool minion, int mana)
{
	auto lethal = Player->GetAutoAttack().Damage_ * 4 > unit->GetHealth();

	if (Spells->Q->IsReady() && !Player->IsWindingUp())
	{
		if (!rockets)
		{
			if (Player->ManaPercent() > mana || lethal)
			{
				if (Ex->Dist2D(unit) > 525)
				{
					/* todo: get minion
					 * 
					 * 
					 */

					Spells->Q->CastOnPlayer();
				}
			}
		}
		else
		{
			if (Ex->Dist2D(unit) > 525)
			{
				for (auto i : Enemies)
				{
					if (i != nullptr && i->IsValidTarget())
					{
						if (i->GetNetworkId() != unit->GetNetworkId() && Ex->Dist2D(i) <= 525 + RocketRange())
						{
							Spells->Q->CastOnPlayer();
						}
					}
				}
			}

			if (Ex->Dist2D(unit) <= 525)
			{
				Spells->Q->CastOnPlayer();
			}

			if (Player->ManaPercent() < mana && !lethal)
			{
				Spells->Q->CastOnPlayer();
			}
		}
	}
}

inline void Jinx::UseW(IUnit* unit, int mana)
{
	if (Spells->W->IsReady() && !Player->IsWindingUp())
	{
		if (Player->ManaPercent() > mana || GDamage->GetSpellDamage(Player, unit, kSlotW, 0) >= unit->GetHealth())
		{  
			if (Ex->Dist2D(unit) > 500 + RocketRange())
			{
				if (Player->GetAutoAttack().Damage_ * 3 > unit->GetHealth())
				{
					if (Ex->Dist2D(unit) <= 525 + RocketRange())
					{
						return;
					}
				}

				if (Player->HasBuff("JinxPassiveKillAttackSpeed"))
				{
					int incr[3] = {2, 2, 3};
					auto xtra = incr[min(18, Player->GetLevel()) / 3];

					if (Player->GetAutoAttack().Damage_ * (2 + xtra) > unit->GetHealth())
					{
						if (Ex->Dist2D(unit) <= 525 + RocketRange() + (25 * xtra))
						{
							return;
						}
					}
				}

				auto pred = new AdvPredictionOutput();

				// dunno how prediction is kappa
				if (Spells->W->RunPrediction(unit, false, kCollidesWithHeroes, pred))
				{
					if (pred->HitChance >= kHitChanceHigh)
					{
						Spells->W->CastOnPosition(pred->CastPosition);
					}
				}
			}
		}
	}
}

inline void Jinx::UseE(IUnit* unit, int mana)
{
	if (Spells->E->IsReady() && !Player->IsWindingUp())
	{
		if (Player->ManaPercent() > mana)
		{
			if (unit != nullptr && unit->IsValidTarget())
			{
				auto pred = new AdvPredictionOutput();

				if (Spells->E->RunPrediction(unit, true, kCollidesWithNothing, pred))
				{
					if (pred->HitChance >= kHitChanceVeryHigh)
					{
						Spells->E->CastOnPosition(pred->CastPosition);
					}
				}
			}
		}
	}
}

// overkill r protection (needs work?)
inline bool Jinx::CanUlt(IUnit* unit)
{
	if (Menu->Debug->Enabled() && strcmp(unit->GetObjectName(), "Target Dummy") == 0)
	{
		return true;
	}

	// common valid checks
	if (unit->IsInvulnerable() || unit->IsValidTarget() == false || unit->IsValidObject())
	{
		return false;
	}

	// invulnerabilty checks
	if (unit->HasBuffOfType(BUFF_Invulnerability) || unit->HasBuffOfType(BUFF_PhysicalImmunity) ||
		unit->HasBuffOfType(BUFF_SpellImmunity) || unit->HasBuffOfType(BUFF_SpellShield))
	{
		return false;
	}

	// target can die to zap
	if (Ex->Dist2D(unit) <= Spells->W->Range() && Spells->W->IsReady())
	{
		if (GDamage->GetSpellDamage(Player, unit, kSlotW, 0) >= unit->GetHealth())
		{
			return false;
		}
	}

	// target will die in few autos
	if (Ex->Dist2D(unit) <= 535 + RocketRange())
	{
		if (Player->GetAutoAttack().Damage_ * 2 >= unit->GetHealth())
		{
			return false;
		}
	}

	// dont ult if we go zoooooooooooom
	if (Player->HasBuff("JinxPassiveKillAttackSpeed"))
	{
		if (Ex->Dist2D(unit) <= 525 + RocketRange())
		{
			// ult if below 50%
			if (Player->HealthPercent() > 50)
			{
				return false;
			}
		}
	}

	for (auto i : Allies)
	{
		// check if valid
		if (!i->IsDead() && i->IsValidObject())
		{
			// check if ally near target and ally is safe
			if (Ex->Dist2D(i, unit) <= 500 && i->HealthPercent() > unit->HealthPercent())
			{
				// if ally bellow 25 return true (to close to call)
				// if extra enemies near target return true (aoe damage)
				if (unit->HealthPercent() > 25 && Ex->CountEnemiesInRange(unit, 450) < 2)
				{
					return false;
				}
			}
		}
	}

	// else return true
	return true;
}

inline float Jinx::UltDamage(IUnit* unit)
{
	float dmg = 0;

	if (unit != nullptr && Spells->R->IsReady())
	{
		auto units = std::vector<IUnit*>();
		auto maxdist = Ex->Dist2D(unit) > 750;
		auto maxrdist = Menu->MaxUltRange->GetInteger() * 2;
		auto radius = 200 + 225; // estimate missile width + splash radius ?

		if (Ex->UnitsInR(Player, unit, radius, maxrdist, units, false) > 1)
		{
			for (auto i : units)
			{
				if (i->GetNetworkId() != unit->GetNetworkId())
				{
					if (Ex->Dist2D(i, unit) <= 225 + 100)
					{
						// dunno if damage is working
						dmg = maxdist ? GDamage->GetSpellDamage(Player, unit, kSlotR, 1) : GDamage->GetSpellDamage(Player, unit, kSlotR, 0);
					}
				}
			}
		}
		else
		{
			// damage lib bik?
			dmg = maxdist ? GDamage->GetSpellDamage(Player, unit, kSlotR, 1) : GDamage->GetSpellDamage(Player, unit, kSlotR, 0);
		}

		return dmg;
	}

	return dmg;
}

