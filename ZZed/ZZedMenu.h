#pragma once
#include "PluginSDK.h"

class ZZedMenu {
    public:
        explicit ZZedMenu(IMenu * menu);

        std::map<std::string, IMenuOption *> AlwaysRTargets;

        // root;
        IMenu * Menu;
        IMenuOption * Debug;

        IMenuOption * UseComboQ;
        IMenuOption * UseComboW;
        IMenuOption * UseComboE;
        IMenuOption * UseComboR;
        IMenuOption * SwapRIfDead;
        IMenuOption * ExtendedQCombo;
        IMenuOption * ShadowPlacement;
        IMenuOption * MinimumHarassEnergy;
        IMenuOption * UseHarassQ;
        IMenuOption * UseHarassW;
        IMenuOption * UseHarassE;
        IMenuOption * ExtendedQHarass;
        IMenuOption * JungleOrderPriority;
        IMenuOption * AutoEUnitInRage;
        IMenuOption * AutoEUnitInRagePct;
        IMenuOption * DrawComboDamage;
        IMenuOption * DrawComboDamageColor;
        IMenuOption * ComboKey;
        IMenuOption * HarassKey;
        IMenuOption * FleeKey;
        IMenuOption * UltMode;
        IMenuOption * ExHarassQMaxDamage;
        IMenuOption * ExComboQMaxDamage;
        IMenuOption * DrawPredictedShadow;
        IMenuOption * DebugPathfinding;
        IMenuOption * UseAlwaysR;
        IMenuOption * MinimumClearEnergy;
        IMenuOption * AutoR;
        IMenuOption * ClearKey;
        IMenuOption * UseFleeW;
        IMenuOption * UseItemsCombo;
        IMenuOption * AlwaysRSelected;
        IMenuOption * UseHarassWPF; };


