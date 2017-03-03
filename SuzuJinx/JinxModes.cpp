#pragma once
#include "Jinx.h"
#include "JinxModes.h"

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
	
}

void JinxModes::Interrupter(InterruptibleSpell args)
{
	
}

void JinxModes::BeforeAttack(IUnit* target)
{
	
}
