#pragma once
#include <map>
#include "PluginSDK.h"


class ZNidaleeMenu {
    public:
        ~ZNidaleeMenu();
        ZNidaleeMenu(IMenu * menu);

        IMenu * Menu;

        std::map<std::string, IMenuOption *> AllyHealMap;

        std::map<std::string, std::pair<IMenuOption *, float>> HumanQMap;
        std::map<std::string, std::pair<IMenuOption *, float>> HumanWMap;
        std::map<std::string, std::pair<IMenuOption *, float>> HumanEMap;
        std::map<std::string, std::pair<IMenuOption *, float>> HumanRMap;

        std::map<std::string, std::pair<IMenuOption *, float>> CougarQMap;
        std::map<std::string, std::pair<IMenuOption *, float>> CougarWMap;
        std::map<std::string, std::pair<IMenuOption *, float>> CougarEMap;
        std::map<std::string, std::pair<IMenuOption *, float>> CougarRMap;

        IMenuOption * EnableHealing;
        IMenuOption * ReduceWUsage;
        IMenuOption * HealingManaPct;
        IMenuOption * HealingSwitchForm;
        IMenuOption * AllyHealPriroty;
        IMenuOption * RequireSwipeTakedownCombo;
        IMenuOption * RequireSwipeTakedownJungle;
        IMenuOption * CurrentPlayStyle;
        IMenuOption * ComboButton;
        IMenuOption * HarassButton;
        IMenuOption * ClearButton;
        IMenuOption * FleeButton;
        IMenuOption * BushwhackLocation;
        IMenuOption * CheckSpearHitChance;
        IMenuOption * CheckSpearHitChanceMin;
        IMenuOption * CheckPounceHitChance;
        IMenuOption * CheckPounceHitChanceMin;
        IMenuOption * IgnorePounceChecksOnHuntedTarget;
        IMenuOption * CheckPounceMinRangeCombo;
        IMenuOption * CheckPounceMinRangeWaveClear;
        IMenuOption * CheckPounceIntoEnemiesWaveClear;
        IMenuOption * CheckCatSwitchNearEnemies;
        IMenuOption * CheckCatSwitchNearTurret;
        IMenuOption * CheckPounceIntoTurretWaveClear;
        IMenuOption * CheckSwipeHitchange;
        IMenuOption * CheckSwipeHitChanceMin;
        IMenuOption * CheckSwipeMinimumHitWaveClear;
        IMenuOption * DrawJavelinRange;
        IMenuOption * DrawJavelinTimer;
        IMenuOption * DrawHuntedPounceRange;
        IMenuOption * DrawCurrentTarget;
        IMenuOption * PracticeToolsDebug;
        IMenuOption * EnableSmite;
        IMenuOption * TakedownSmite;
        IMenuOption * SmiteEpic;
        IMenuOption * SmiteLarge;
        IMenuOption * SmiteSmall;
        IMenuOption * SmiteOnHero;
        IMenuOption * AutoLevelR;
        IMenuOption * AutoQImmobile;
        IMenuOption * AutoWImmobile;
        IMenuOption * AutoRGapcloser;
        IMenuOption * AutoSwipeGapcloser;
        IMenuOption * AutoSpearGapcloser;
        IMenuOption * PredictionSelection;
        IMenuOption * JungleOrderPriority;
        IMenuOption * DrawQ;
        IMenuOption * DrawQColor;
        IMenuOption * DrawW2;
        IMenuOption * DrawW2Color;
        IMenuOption * DrawE;
        IMenuOption * DrawEColor;
        IMenuOption * DrawQTimer; };

