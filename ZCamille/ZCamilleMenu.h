#pragma once
#include "PluginSDK.h"

class ZCamilleMenu {
    public:
        ~ZCamilleMenu();
        ZCamilleMenu(IMenu * menu);

        IMenu * Menu;

        // hotkeys

        IMenuOption * ComboKey;
        IMenuOption * HarassKey;
        IMenuOption * WaveClearKey;
        IMenuOption * JungleClearKey;
        IMenuOption * FleeKey;

        // combo
        IMenuOption * ComboMana;
        IMenuOption * ComboQ;
        IMenuOption * ComboW;
        IMenuOption * ComboE;
        IMenuOption * ComboEPred;
        IMenuOption * ComboR;

        // farming
        IMenuOption * FarmingWithItems;
        IMenuOption * FarmNearEnemies;

        // wave clear
        IMenuOption * WaveClearMana;
        IMenuOption * WaveClearOverride;
        IMenuOption * WaveClearMinHit;
        IMenuOption * WaveClearQ;
        IMenuOption * WaveClearW;

        // jungle clear
        IMenuOption * JungleClearMana;
        IMenuOption * JungleClearQ;
        IMenuOption * JungleClearW;
        IMenuOption * JungleClearE;

        // flee
        IMenuOption * FleeE;

        // harass
        IMenuOption * HarassMana;
        IMenuOption * HarassQ;
        IMenuOption * HarassW;

        // magnet

        IMenuOption * MagnetW;
        IMenuOption * MagnetWCombo;
        IMenuOption * MagnetWHarass;
        IMenuOption * MagnetWClear;

        // events

        IMenuOption * UseAntiGapcloser;
        IMenuOption * UseInterrupter;

        // mechanics
        IMenuOption * DontEUnderTurret;
        IMenuOption * MinimumERange;
        IMenuOption * EnhancedEPrecision;
        IMenuOption * UltOnlySelected;
        IMenuOption * ForceROrbwalk;
        IMenuOption * ExpirimentalCombo;

        // draw
        IMenuOption * DrawW;
        IMenuOption * DrawWColor;
        IMenuOption * DrawE;
        IMenuOption * DrawEColor;
        IMenuOption * DrawR;
        IMenuOption * DrawRColor; };

inline ZCamilleMenu::ZCamilleMenu(IMenu * menu) {
    this->Menu = menu;
    auto menuhk = this->Menu->AddMenu("::Hotkeys");
    this->ComboKey = menuhk->AddKey("Combo [active]", 32);
    this->HarassKey = menuhk->AddKey("Harass [active]", 'G');
    this->WaveClearKey = menuhk->AddKey("WaveClear [active]", 'V');
    this->JungleClearKey = menuhk->AddKey("JungleClear [active]", 'V');
    this->FleeKey = menuhk->AddKey("Flee [active]", 'A');
    auto menucb = this->Menu->AddMenu("::Combo");
    this->ComboMana = menucb->AddInteger("Minimum Mana % ", 0, 100, 0);
    this->ComboQ = menucb->CheckBox("Use Q", true);
    this->ComboW = menucb->CheckBox("Use W", true);
    this->ComboE = menucb->CheckBox("Use E", true);
    this->ComboEPred = menucb->AddInteger("Use E Prediction [0 = Disabled]", 0, 1, 0);
    this->ComboR = menucb->CheckBox("Use R", true);
    auto menuhr = this->Menu->AddMenu("::Harass");
    this->HarassMana = menuhr->AddInteger("Minimum Mana %", 0, 100, 75);
    this->HarassQ = menuhr->CheckBox("Use Q", true);
    this->HarassW = menuhr->CheckBox("Use W", true);
    auto menufl = this->Menu->AddMenu("::Flee");
    this->FleeE = menufl->CheckBox("Use E", true);
    auto menufm = this->Menu->AddMenu("::Farming");
    this->FarmingWithItems = menufm->CheckBox("Farm With Items", true);
    this->FarmNearEnemies = menufm->CheckBox("Farm Near Enemies", false);
    this->WaveClearOverride = menufm->AddInteger(" - Minimum Mana Override %", 0, 100, 35);
    auto menuwc = this->Menu->AddMenu("::WaveClear");
    this->WaveClearMana = menuwc->AddInteger("Minimum Mana %", 0, 100, 65);
    this->WaveClearQ = menuwc->CheckBox("Use Q", true);
    this->WaveClearW = menuwc->CheckBox("Use W", true);
    this->WaveClearMinHit = menuwc->AddInteger("- Min Hit >=", 1, 6, 3);
    auto menujg = this->Menu->AddMenu("::JungleClear");
    this->JungleClearMana = menujg->AddInteger("Minimum Mana %", 0, 100, 35);
    this->JungleClearQ = menujg->CheckBox("Use Q", true);
    this->JungleClearW = menujg->CheckBox("Use W", true);
    this->JungleClearE = menujg->CheckBox("Use E", true);
    auto menuev = this->Menu->AddMenu("::Events");
    this->UseAntiGapcloser = menuev->CheckBox("Anti-Gapcloser", false);
    this->UseInterrupter = menuev->CheckBox("Use Interrupter", true);
    auto menumc = this->Menu->AddMenu("::Mechanics");
    this->ForceROrbwalk = menumc->CheckBox("Orbwalk Focus R Target", true);
    this->UltOnlySelected = menumc->CheckBox("R Selected Target Only", false);
    this->MinimumERange = menumc->AddInteger("Minimum E Range", 0, 1100, 165);
    this->DontEUnderTurret = menumc->CheckBox("Dont E Into Turret/Danger", true);
    this->EnhancedEPrecision = menumc->CheckBox("Use Enhanced E Precision", false);
    this->ExpirimentalCombo = menumc->CheckBox("Use Expiremental Combo [W->E)", false);
    auto menumg = this->Menu->AddMenu("::Magnet-Lock");
    this->MagnetW = menumg->CheckBox("Magnet W [Beta]", true);
    this->MagnetWCombo = menumg->CheckBox("- Combo", true);
    this->MagnetWHarass = menumg->CheckBox("- Harass", true);
    this->MagnetWClear = menumg->CheckBox("- Clear", true);
    auto menurd = this->Menu->AddMenu("::Render");
    this->DrawW = menurd->CheckBox("Enabled W Drawings", false);
    this->DrawWColor = menurd->AddColor("- Color", 121, 77, 255, 200);
    this->DrawE = menurd->CheckBox("Enabled E Drawings", true);
    this->DrawEColor = menurd->AddColor("- Color", 121, 77, 255, 200);
    this->DrawR = menurd->CheckBox("Enabled R Drawings", true);
    this->DrawRColor = menurd->AddColor("- Color", 121, 77, 255, 200);
    //magnetMenu->CheckBox("Magnet Orbwalking", true);
}