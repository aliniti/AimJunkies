#pragma once
#include "PluginSDK.h"
#include <map>

class ZCamilleMenu {
    public:
        ~ZCamilleMenu();
        ZCamilleMenu(IMenu * menu);

        IMenu * Menu;
        IMenuOption * ComboKey;
        IMenuOption * HarassKey;
        IMenuOption * WaveClearKey;
        IMenuOption * JungleClearKey;
        IMenuOption * FleeKey;
        IMenuOption * ComboMana;
        IMenuOption * ComboQ;
        IMenuOption * ComboW;
        IMenuOption * ComboE;
        IMenuOption * ComboEPred;
        IMenuOption * ComboR;
        IMenuOption * RWhen;
        IMenuOption * FarmingWithItems;
        IMenuOption * FarmNearEnemies;
        IMenuOption * WaveClearMana;
        IMenuOption * WaveClearOverride;
        IMenuOption * WaveClearMinHit;
        IMenuOption * WaveClearQ;
        IMenuOption * WaveClearW;
        IMenuOption * JungleClearMana;
        IMenuOption * JungleClearQ;
        IMenuOption * JungleClearW;
        IMenuOption * JungleClearE;
        IMenuOption * FleeE;
        IMenuOption * HarassMana;
        IMenuOption * HarassQ;
        IMenuOption * HarassW;
        IMenuOption * MagnetW;
        IMenuOption * MagnetWCombo;
        IMenuOption * MagnetWHarass;
        IMenuOption * MagnetWClear;
        IMenuOption * UseAntiGapcloser;
        IMenuOption * UseInterrupter;
        IMenuOption * DontEUnderTurret;
        IMenuOption * DontEUnderTurretToggle;
        IMenuOption * MinimumERange;
        IMenuOption * EnhancedEPrecision;
        IMenuOption * UltOnlySelected;
        IMenuOption * ForceROrbwalk;
        IMenuOption * ExpirimentalCombo;
        IMenuOption * DrawW;
        IMenuOption * DrawWColor;
        IMenuOption * DrawE;
        IMenuOption * DrawEColor;
        IMenuOption * DrawR;
        IMenuOption * DrawRColor;
        IMenuOption * AvoidEventHorizon;
        IMenuOption * AvoidR;
        IMenuOption * UseRAvoider;
        IMenuOption * AvoidSpell;
        std::map<std::string, IMenuOption *> SpellsToAvoid; };

