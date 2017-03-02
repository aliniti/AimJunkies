#pragma once
#include "JinxMenu.h"
#include "PluginSDK.h"
#include "Jinx.h"
#include <algorithm>

PluginSetup("SuzuJinx"); 

IUnit *Jinx::Player = nullptr;
JinxMenu *Jinx::Menu = nullptr;
JinxExtensions *Jinx::Ex = new JinxExtensions();
JinxSpells *Jinx::Spells = new JinxSpells();
JinxModes *Jinx::Modes = new JinxModes();

std::vector<IUnit*> Jinx::Allies = std::vector<IUnit*>();
std::vector<IUnit*> Jinx::Enemies = std::vector<IUnit*>();

PLUGIN_EVENT(void) OnGameUpdate()
{
	Jinx::Modes->Auto();

	switch (GOrbwalking->GetOrbwalkingMode())
	{
		case kModeCombo:
			Jinx::Modes->Combo();
			break;
		case kModeMixed:
			Jinx::Modes->Harass();
			break;
		default: ;
	}
}

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);

	Jinx::Player = GEntityList->Player();
	Jinx::Menu = new JinxMenu(GPluginSDK->AddMenu("Clean Jinx+"));

	Jinx::Spells->CreateSpells();
	Jinx::Spells->SetPrediction();

	Jinx::GetHeroes();
	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GGame->PrintChat("<font color=\"#00CCCC\"><b>SuzuJinx</b></font> <b><font color=\"#FFFFFF\">Loaded++!</font></b>");
}

PLUGIN_API void OnUnload()
{
	Jinx::Menu->Menu->Remove();
	Jinx::Allies.clear();
	Jinx::Enemies.clear();
	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
}
