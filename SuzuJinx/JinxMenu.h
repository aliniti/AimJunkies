#pragma once
#include "PluginSDK.h"

class JinxMenu
{
public:
	~JinxMenu();
	JinxMenu(IMenu *menu);

	IMenu *Menu;

	// hotkeys
	IMenuOption *ComboKey;
	IMenuOption *MixedKey;
	IMenuOption *WaveClearKey;
	IMenuOption *JungleClearKey;
	IMenuOption *FleeKey;

	// combo
	IMenuOption *ComboQ;
	IMenuOption *ComboQMinion;
	IMenuOption *ComboW;
	IMenuOption *ComboE;
	IMenuOption *ComboR;

	// mixed
	IMenuOption *HarassQ;
	IMenuOption *HarassQMinion;
	IMenuOption *HarassQMana;
	IMenuOption *HarassW;
	IMenuOption *HarassWMana;
	IMenuOption *AutoHarass;
	IMenuOption *AutoHarassMana;

	// farming
	IMenuOption *FarmQ;
	IMenuOption *SwapBackQ;
	IMenuOption *MinimumQFarm;
	IMenuOption *FarmQMana;
	IMenuOption *FarmOnlyKill;
	IMenuOption *ManaNearEnemy;
	IMenuOption *ManaNoEnemies;

	// mechanics
	IMenuOption *AutoRKillable;
	IMenuOption *AutoEImmobile;
	IMenuOption *AutoEDashing;
	IMenuOption *AutoETeleport;
	IMenuOption *MaxUltRange;
	IMenuOption *SaveMana;

	// events
	IMenuOption *Interrupt;
	IMenuOption *GapCloser;
	IMenuOption *SkinChanger;
	IMenuOption *SkinChangerId;

	// drawings
	IMenuOption *DrawQ;
	IMenuOption *DrawW;
	IMenuOption *DrawE;
	IMenuOption *DrawR;

	IMenuOption *Debug;
};

inline JinxMenu::JinxMenu(IMenu *initMenu)
{
	this->Menu = initMenu;

	auto menuhk = this->Menu->AddMenu("Hotkeys");
	this->ComboKey = menuhk->AddKey("Combo Key", 32);
	this->MixedKey = menuhk->AddKey("Harass Key", 'G');
	this->WaveClearKey = menuhk->AddKey("WaveClear", 'V');
	this->JungleClearKey = menuhk->AddKey("Jungle Clear", 'V');
	this->FleeKey = menuhk->AddKey("Flee Key", 'A');

	auto menucb = this->Menu->AddMenu("Combo");
	this->ComboQ = menucb->CheckBox("Use Q", true);
	this->ComboQMinion = menucb->CheckBox("-> On Minion", false);
	this->ComboW = menucb->CheckBox("Use W", true);
	this->ComboE = menucb->CheckBox("Use E", false);
	this->ComboR = menucb->CheckBox("Use R", true);

	auto menuhr = this->Menu->AddMenu("Harass");
	this->HarassQ = menuhr->CheckBox("Use Q", true);
	this->HarassQMinion = menuhr->CheckBox("-> On Minion", true);
	this->HarassQMana = menuhr->AddInteger("Minimum Mana %", 0, 100, 65);
	this->HarassW= menuhr->CheckBox("Use W", true);
	this->HarassWMana = menuhr->AddInteger("Minimum Mana %", 0, 100, 55);
	this->AutoHarass = menuhr->CheckBox("Auto Q Minion Harass", true);
	this->AutoHarass = menuhr->AddInteger("Minimum Mana %", 0, 100, 45);

	auto menufm = this->Menu->AddMenu("Farming");
	this->FarmQ = menufm->CheckBox("Use Q", true);
	this->SwapBackQ = menufm->CheckBox("Auto Swap to Minigun", true);
	this->MinimumQFarm = menufm->AddInteger("Minimum minion count", 2, 6, 3);
	this->ManaNoEnemies = menufm->AddInteger("Minimum Mana % No Enemies", 0, 100, 45);
	this->ManaNearEnemy = menufm->AddInteger("Minimum Mana % Near Enemies", 0, 100, 75);

	auto menuat = this->Menu->AddMenu("Mechanics");
	this->MaxUltRange = menuat->AddInteger("Max Ult Range", 100, 2000, 1300);
	this->SaveMana = menuat->CheckBox("Conserve Mana", true);
	this->AutoEImmobile = menuat->CheckBox("Auto E Immobile", true);
	this->AutoEDashing = menuat->CheckBox("Auto E Dashing", true);
	this->GapCloser = menuat->CheckBox("Auto E Gapcloser", true);
	this->AutoRKillable = menuat->CheckBox("Auto R!", true);

	auto menuev = this->Menu->AddMenu("Events");
	this->Interrupt = menuev->CheckBox("Interrupt", true);

	auto menudr = this->Menu->AddMenu("Draw");
	this->DrawQ = menudr->CheckBox("Draw Q", true);
	this->DrawQ = menudr->CheckBox("Draw W", false);
	this->Debug = menudr->CheckBox("DEBUG", false);
}
