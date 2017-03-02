#pragma once
#include "Jinx.h"
#include "JinxModes.h"


void JinxModes::Combo()
{
	auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Jinx::Spells->W->Range());

	if (unit != nullptr && unit->IsValidTarget())
	{
		auto mana = Jinx::Menu->SaveMana->Enabled() ? 20 : 0;

		if (Jinx::Menu->ComboQ->Enabled())
		{
			
			Jinx::UseQ(unit, Jinx::Player->GetSpellBook()->GetToggleState(kSlotQ) == 2, false, mana);
		}

		if (Jinx::Menu->ComboW->Enabled())
		{
			Jinx::UseW(unit, mana);
		}
	}
	
	if (Jinx::Spells->W->IsReady() && unit != nullptr)
	{
		if (unit->IsValidTarget())
		{
			auto output = new AdvPredictionOutput();

			if (!Jinx::Player->IsWindingUp() && Jinx::Player->ManaPercent() > 25)
			{
				if (Jinx::Ex->Dist2D(unit) <= 565 && Jinx::Player->ManaPercent() > 20)
				{
					if (Jinx::Player->HasBuff("JinxPassiveKillAttackSpeed") && Jinx::Player->GetAutoAttack().Damage_ * 4 > unit->GetHealth())
					{
						return;
					}
				}

				if (Jinx::Ex->Dist2D(unit) > 500)
				{
					if (Jinx::Player->HasBuff("JinxPassiveKillAttackSpeed") && Jinx::Player->GetAutoAttack().Damage_ * 4 > unit->GetHealth())
					{
						return;
					}

					if (Jinx::Spells->W->RunPrediction(unit, false, kCollidesWithNothing, output))
					{
						Jinx::Spells->W->CastOnPosition(unit->ServerPosition());            
					}
				}
			}
		}
	}
}

void JinxModes::Harass()
{
	auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Jinx::Spells->W->Range());

	if (Jinx::Player->ManaPercent() > Jinx::Menu->HarassQMana->GetInteger())
	{
		if (Jinx::Spells->W->IsReady() && unit != nullptr)
		{
			if (unit->IsValidTarget())
			{
				auto output = new AdvPredictionOutput();

				if (!Jinx::Player->IsWindingUp() && Jinx::Player->ManaPercent() > 25)
				{
					if (Jinx::Ex->Dist2D(unit) <= 600 && Jinx::Player->ManaPercent() > 20)
					{
						if (Jinx::Player->HasBuff("JinxPassiveKillAttackSpeed") && Jinx::Player->GetAutoAttack().Damage_ * 4 > unit->GetHealth())
						{
							return;
						}
					}

					if (Jinx::Ex->Dist2D(unit) > 600)
					{
						if (Jinx::Spells->W->RunPrediction(unit, false, kCollidesWithHeroes, output))
						{
							if (output->HitChance >= kHitChanceVeryHigh)
							{
								Jinx::Spells->W->CastOnPosition(output->CastPosition);
							}
						}
					}
				}
			}
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

	if (Jinx::Spells->E->IsReady() && unit != nullptr)
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
						if (pred->HitChance >= kHitChanceMedium)
						{
							Jinx::Spells->R->CastOnPosition(pred->CastPosition);
						}
					}
				}
			}
		}
	}
}
