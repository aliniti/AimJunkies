#pragma once
#include "JinxMenu.h"
#include "PluginSDK.h"
#include "Jinx.h"

PluginSetup("SuzuJinx"); 

IUnit *Jinx::Player = nullptr;
JinxMenu *Jinx::Menu = nullptr;
JinxModes *Jinx::Modes = new JinxModes();
JinxSpells *Jinx::Spells = new JinxSpells();
JinxExtensions *Jinx::Ex = new JinxExtensions();

std::vector<IUnit*> Jinx::Allies = std::vector<IUnit*>();
std::vector<IUnit*> Jinx::Enemies = std::vector<IUnit*>();
std::vector<IUnit*> Jinx::NeutralMinions = std::vector<IUnit*>();
std::vector<IUnit*> Jinx::AllyMinions = std::vector<IUnit*>();
std::vector<IUnit*> Jinx::EnemyMinions = std::vector<IUnit*>();

PLUGIN_EVENT(void) OnRender()
{
	Jinx::OnDraw();
}

PLUGIN_EVENT(void) OnGapCloser(GapCloserSpell const& args)
{
	Jinx::Modes->OnGapcloser(args);
}

PLUGIN_EVENT(void) OnInterruptible(InterruptibleSpell const& args)
{
	Jinx::Modes->Interrupter(args);
}

PLUGIN_EVENT(void) OnOrbwalkBeforeAttack(IUnit* target)
{
	Jinx::Modes->BeforeAttack(target);
}

PLUGIN_EVENT(void) OnGameUpdate()
{
	if (!GUtility->IsLeagueWindowFocused())
	{
		return;
	}

	Jinx::Modes->Auto();
	 
	switch (GOrbwalking->GetOrbwalkingMode())
	{
		case kModeCombo:
			Jinx::Modes->Combo();
			break;
		case kModeMixed:
			Jinx::Modes->Harass();
			break;

		case kModeLaneClear:
			Jinx::Modes->WaveClear();
			break;
	}
}

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);

	Jinx::Player = GEntityList->Player();
	Jinx::Menu = new JinxMenu(GPluginSDK->AddMenu("SuzuJinx"));

	Jinx::Spells->CreateSpells();
	Jinx::Spells->SetPrediction();

	Jinx::GetHeroes();
	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->AddEventHandler(kEventOrbwalkBeforeAttack, OnOrbwalkBeforeAttack);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);
	GEventManager->AddEventHandler(kEventOnInterruptible, OnInterruptible);
	GGame->PrintChat("<font color=\"#00CCCC\"><b>SuzuJinx</b></font><b><font color=\"#FFFFFF\">++</font></b> <b><font color=\"#FFFFFF\">Loaded!</font></b>");
	GGame->PrintChat("<b><font color=\"#FFFFFF\">Beta (No Drawings Or Farming Yet)</font></b>");
}

PLUGIN_API void OnUnload()
{
	Jinx::Allies.clear();
	Jinx::Enemies.clear();
	Jinx::EnemyMinions.clear();
	Jinx::AllyMinions.clear();
	Jinx::NeutralMinions.clear();

	Jinx::Menu->Menu->Remove();

	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->RemoveEventHandler(kEventOrbwalkBeforeAttack, OnOrbwalkBeforeAttack);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(kEventOnInterruptible, OnInterruptible);
}
