#pragma once
#include "JinxSpells.h"
#include "PluginSDK.h"

void JinxSpells::CreateSpells()
{
	this->Q = GPluginSDK->CreateSpell(kSlotQ, 600);
	this->W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, true, false, kCollidesWithHeroes);
	this->E = GPluginSDK->CreateSpell2(kSlotE, kCircleCast, false, false, kCollidesWithNothing);
	this->R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, true, kCollidesWithHeroes);
}

void JinxSpells::SetPrediction() 
{
	this->E->SetSkillshot(1.2, 120, 2300, 900);
	this->W->SetSkillshot(0.6, 60, 3300, 1500);
	this->R->SetSkillshot(0.6, 180, 1700, 3000);
}

