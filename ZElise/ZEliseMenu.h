#pragma once
#include "PluginSDK.h"
#include <map>


class ZEliseMenu {
    public:
        ZEliseMenu(IMenu * menu);

        // root;
        IMenu * Menu;
        IMenuOption * PracticeToolsDebug;

        // maps

        std::map<std::string, std::pair<IMenuOption *, float>> HumanQMap;
        std::map<std::string, std::pair<IMenuOption *, float>> HumanWMap;
        std::map<std::string, std::pair<IMenuOption *, float>> HumanEMap;
        std::map<std::string, std::pair<IMenuOption *, float>> HumanRMap;

        std::map<std::string, std::pair<IMenuOption *, float>> SpiderQMap;
        std::map<std::string, std::pair<IMenuOption *, float>> SpiderWMap;
        std::map<std::string, std::pair<IMenuOption *, float>> SpiderEMap;
        std::map<std::string, std::pair<IMenuOption *, float>> SpiderRMap;

        std::map<std::string, IMenuOption *> AntiGapcloserWhiteList;
        std::map<std::string, IMenuOption *> InterrupterWhiteList;

        // keys

        IMenuOption * ComboButton;
        IMenuOption * HarassButton;
        IMenuOption * FarmButton;
        IMenuOption * FleeButton;
        IMenuOption * CocoonComboHitChance;
        IMenuOption * CocoonComboMinRange;
        IMenuOption * CocoonHarassMinMana;
        IMenuOption * NeurotoxinHarassMinMana;
        IMenuOption * VolotileSpiderHarassMinMana;
        IMenuOption * NeurotoxinJungleMinMana;
        IMenuOption * VolotileSpiderJungleMinMana;
        IMenuOption * CocoonJungleMinMana;
        IMenuOption * HumanToSpiderComboRequiredSkills;
        IMenuOption * HumanToSpiderHarassRequiredSkills;
        IMenuOption * CocoonHarassHitChance;
        IMenuOption * SpiderToHumanComboRequiredSkills;
        IMenuOption * SpiderToHumanHarassRequiredSkills;
        IMenuOption * BetaCollisionChecks;
        IMenuOption * BlockAutoAttacks;
        IMenuOption * UseProtobelt;
        IMenuOption * DrawQ;
        IMenuOption * DrawQColor;
        IMenuOption * DrawE;
        IMenuOption * DrawEColor;
        IMenuOption * JungleOrderPriority;
        IMenuOption * DontCocoonJungleNearEnemies; };


