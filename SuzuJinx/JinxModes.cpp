#pragma once
#include "Jinx.h"
#include "JinxModes.h"
#include <string>

void JinxModes::Combo()
{
	auto wunit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Jinx::Spells->W->Range());

	if (wunit != nullptr && wunit->IsValidTarget())
	{
		auto mana = Jinx::Menu->SaveMana->Enabled() ? 20 : 0;

		if (Jinx::Menu->ComboQ->Enabled())
		{		
			Jinx::UseQ(wunit, Jinx::Player->GetSpellBook()->GetToggleState(kSlotQ) == 2, false, mana);
		}

		if (Jinx::Menu->ComboW->Enabled())
		{
			Jinx::UseW(wunit, mana);
		}
	}

	auto eunit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Jinx::Spells->E->Range());

	if (eunit != nullptr && eunit->IsValidTarget())
	{
		if (Jinx::Menu->ComboE->Enabled())
		{
			auto mana = Jinx::Menu->SaveMana->Enabled() ? 20 : 0;

			Jinx::UseE(eunit, mana);
		}
	}
}

void JinxModes::Harass()
{
	auto wunit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Jinx::Spells->W->Range());

	if (wunit != nullptr && wunit->IsValidTarget())
	{
		if (Jinx::Spells->Q->IsReady())
		{
			auto mana = Jinx::Menu->HarassQMana->GetInteger();
			Jinx::UseQ(wunit, Jinx::Player->GetSpellBook()->GetToggleState(kSlotQ) == 2, true, mana);
		}

		if (Jinx::Spells->W->IsReady())
		{
			auto mana = Jinx::Menu->HarassWMana->GetInteger();
			Jinx::UseW(wunit, mana);
		}
	}
}

void JinxModes::WaveClear()
{

}

void JinxModes::JungleClear()
{
}

void JinxModes::Flee()
{

}

void JinxModes::Auto()
{
    auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Jinx::Spells->E->Range());

	if (Jinx::Spells->E->IsReady() && unit != nullptr && unit->IsValidTarget())
	{
		auto output = new AdvPredictionOutput();

		if (Jinx::Spells->E->RunPrediction(unit, false, kCollidesWithNothing, output))
		{
			if (output->HitChance == kHitChanceImmobile && Jinx::Menu->AutoEImmobile->Enabled())
			{
				Jinx::Spells->E->CastOnPosition(output->CastPosition);
			}

			if (output->HitChance == kHitChanceDashing && Jinx::Menu->AutoEDashing->Enabled())
			{
				Jinx::Spells->E->CastOnPosition(output->CastPosition);
			}
		}
	}

	if (Jinx::Player->GetSpellBook()->GetToggleState(kSlotQ) == 2 && Jinx::Spells->Q->IsReady() && Jinx::Menu->SwapBackQ->Enabled())
	{
		if (!Jinx::Ex->IsKeyDown(Jinx::Menu->ComboKey) && !Jinx::Ex->IsKeyDown(Jinx::Menu->MixedKey))
		{
			if (!Jinx::Ex->IsKeyDown(Jinx::Menu->WaveClearKey) && !Jinx::Ex->IsKeyDown(Jinx::Menu->JungleClearKey))
			{
				if (Jinx::Player->ManaPercent() <= 10 || !GUtility->IsPositionUnderTurret(Jinx::Player->ServerPosition(), true, false))
				{
					Jinx::Spells->Q->CastOnPlayer();
				}
			}
		}
	}

	if (Jinx::Spells->R->IsReady() && Jinx::Menu->AutoRKillable->Enabled())
	{
		for (auto s : Jinx::Enemies)
		{
			auto pred = new AdvPredictionOutput();

			if (s != nullptr && (s->IsValidTarget() || Jinx::Menu->Debug->Enabled()))
			{
				if (s->GetHealth() <= Jinx::UltDamage(s) && Jinx::CanUlt(s))
				{
					if (Jinx::Spells->R->RunPrediction(s, true, kCollidesWithNothing, pred))
					{
						if (pred->HitChance >= kHitChanceHigh)
						{
							Jinx::Spells->R->CastOnPosition(pred->CastPosition);
						}
					}
				}
			}
		}
	}
}

void JinxModes::OnGapcloser(GapCloserSpell args)
{
	if (Jinx::Spells->E->IsReady() && Jinx::Menu->GapCloser->Enabled())
	{
		if (args.Sender != nullptr && args.Sender->IsValidTarget())
		{
			if (Jinx::Ex->Dist2D(args.Sender) <= 350)
			{
				Jinx::Spells->E->CastOnPosition(args.EndPosition);
			}
		}
	}
}

void JinxModes::Interrupter(InterruptibleSpell args)
{
	if (Jinx::Spells->E->IsReady() && Jinx::Menu->Interrupt->Enabled())
	{

	}
}

void JinxModes::BeforeAttack(IUnit* target)
{
	return;

	if (target != nullptr && target->UnitFlags() == FL_CREEP)
	{
		if (target->IsValidTarget() && Jinx::Ex->Dist2D(target) > 500 && Jinx::Ex->Dist2D(target) <= 1300)
		{
			if (GOrbwalking->GetOrbwalkingMode() == kModeMixed && !Jinx::Player->IsWindingUp())
			{
				if (Jinx::Menu->HarassQMinion->Enabled() && Jinx::Spells->Q->IsReady())
				{
					for (auto minion : GEntityList->GetAllMinions(false, true, true))
					{
						if (minion != nullptr && minion->IsValidTarget())
						{
							if (Jinx::Ex->Dist2D(minion, target) <= Jinx::Player->HasItemId(3085) ? 200 : 100)
							{
								if (Jinx::Player->GetSpellBook()->GetToggleState(kSlotQ) != 2)
								{
									Jinx::Spells->Q->CastOnPlayer();
								}
							}
						}
					}
				}
			}

			if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
			{
				if (Jinx::Spells->Q->IsReady() && !Jinx::Player->IsWindingUp())
				{
					for (auto minion : GEntityList->GetAllMinions(false, true, true))
					{
						if (minion != nullptr && minion->IsValidTarget())
						{
							if (Jinx::Ex->Dist2D(minion, target) <= Jinx::Player->HasItemId(3085) ? 200 : 100)
							{
								if (Jinx::Player->GetSpellBook()->GetToggleState(kSlotQ) != 2)
								{
									Jinx::Spells->Q->CastOnPlayer();
								}
							}
						}
					}
				}

				return;
			}

			if (Jinx::Menu->AutoHarass->Enabled() && Jinx::Player->ManaPercent() > Jinx::Menu->AutoHarassMana->GetInteger())
			{
				if (Jinx::Spells->Q->IsReady() && !Jinx::Player->IsWindingUp())
				{
					for (auto minion : GEntityList->GetAllMinions(false, true, true))
					{
						if (minion != nullptr && minion->IsValidTarget())
						{
							if (Jinx::Ex->Dist2D(minion, target) <= Jinx::Player->HasItemId(3085) ? 200 : 100)
							{
								if (Jinx::Player->GetSpellBook()->GetToggleState(kSlotQ) != 2)
								{
									Jinx::Spells->Q->CastOnPlayer();
								}
							}
						}
					}
				}
			}
		}
	}
}
