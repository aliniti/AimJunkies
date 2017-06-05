#include "ZZedMenu.h"
#include "ZZed.h"

ZZedMenu::ZZedMenu(IMenu * menu) {

    auto menu_keys = menu->AddMenu("::Keys");
    this->ComboKey = menu_keys->AddKey("Combo [active]", 32);
    this->HarassKey = menu_keys->AddKey("Harass [active]", 'G');
    this->ClearKey = menu_keys->AddKey("Clear/Last Hit [active]", 'V');
    this->FleeKey = menu_keys->AddKey("Flee [active]", 'A');

    ComboMenu = menu->AddMenu("::Combo");
    this->UseComboQ = ComboMenu->CheckBox("Use Q in Combo", true);
    this->ExtendedQCombo = ComboMenu->CheckBox("- Use Extended Q", true);;
    this->UseComboW = ComboMenu->CheckBox("Use W in Combo", true);
    this->GapcloseAfterR = ComboMenu->CheckBox("- Gapclose W on Marked", false);
    this->SwapForKill = ComboMenu->CheckBox("- Swap for Kill", true);
    this->UseComboE = ComboMenu->CheckBox("Use E in Combo", true);
    this->UseComboR = ComboMenu->CheckBox("Use R in Combo", true);
    this->AutoR = ComboMenu->CheckBox("- Killsteal R", true);
    this->AlwaysRSelected = ComboMenu->CheckBox("- Always R on Focus Target", true);
    this->UltMode = ComboMenu->AddSelection("- Mode:", 0, std::vector<std::string> {"Only Kill", "Duel" });
    this->ShadowPlacement = ComboMenu->AddSelection("- Shadow Combo:", 0, std::vector<std::string> {"Line", "Triangle", "Pathfinder", "None" });
    this->AssassinRange = ComboMenu->AddInteger("- Assasination Range", int(ZZed::R->GetSpellRange()), int(ZZed::Q->Range() * 3), int(ZZed::Q->Range() + 500));
    this->SwapRIfDead = ComboMenu->CheckBox("- Swap Back if Dead", true);
    this->LaughIfDead = ComboMenu->CheckBox("- Laugh if Dead", true);

    this->UseBlackList = ComboMenu->CheckBox("Blacklist R", true);

    for (auto i : GEntityList->GetAllHeros(false, true)) {
        if (!i->IsDead()) {
            this->BlackListRTargets[i->GetNetworkId()] = ComboMenu->CheckBox(std::string("- Dont R on").append(" ").append(i->ChampionName()).c_str(), false); } }


    auto menu_harass = menu->AddMenu("::Harass");
    this->UseHarassQ = menu_harass->CheckBox("Use Q in Harass", true);
    this->ExtendedQHarass = menu_harass->CheckBox("- Use Extended Q", false);
    this->UseHarassW = menu_harass->CheckBox("Use W in Harass", true);
    this->UseHarassWPF = menu_harass->AddSelection("Harass Shadow Combo:", 0, std::vector<std::string> { "W->E->Q", "Pathfinder", "Auto", });
    this->UseHarassE = menu_harass->CheckBox("Use E in Harass", true);


    auto menu_jungle = menu->AddMenu("::Jungle");
    this->UseJungleQ = menu_jungle->CheckBox("Use Q in Jungle", true);
    this->UseJungleW = menu_jungle->CheckBox("Use W in Jungle", true);
    this->DontWJungleNearEnemy = menu_jungle->CheckBox("- Dont Use Near Enemy", true);
    this->UseJungleE = menu_jungle->CheckBox("Use E in Jungle", true);

    auto menu_lasthit = menu->AddMenu("::Last Hit (Beta)");
    this->LastHitQ = menu_lasthit->CheckBox("Use Q in Last Hit", true);
    this->LastHitQCount = menu_lasthit->AddInteger("- If Can Kill >", 1, 6, 2);
    this->LastHitQUnderTower = menu_lasthit->CheckBox("- Any Under Tower", true);
    this->LastHitQUnkillable = menu_lasthit->CheckBox("- On Unkillable Minion", true);
    this->LastHitE = menu_lasthit->CheckBox("Use E in Last Hit", true);
    this->LastHitECount = menu_lasthit->AddInteger("- If Can Kill >", 1, 6, 2);
    this->LastHitEUnderTower = menu_lasthit->CheckBox("- Any Under Tower", true);
    this->LastHitEUnkillable = menu_lasthit->CheckBox("- On Unkillable Minion", true);

    auto menu_flee = menu->AddMenu("::Flee");
    this->UseFleeW = menu_flee->CheckBox("Use W in Flee", true);

    auto menu_energy = menu->AddMenu("::Energy");
    this->MinimumHarassEnergy = menu_energy->AddInteger("Min Harass Energy", 0, 200, 100);
    this->MinimumClearEnergy = menu_energy->AddInteger("Min Clear Energy", 0, 200, 100);
    this->MinimumLastHitEnergy = menu_energy->AddInteger("Min Last Hit Energy", 0, 200, 145);
    this->MinimumLastHitEnergyTower = menu_energy->AddInteger("Min Last Hit Energy Under Tower", 0, 200, 50);

    auto menu_mechanics = menu->AddMenu("::Mechanics");
    this->UseItemsCombo = menu_mechanics->CheckBox("Use Items", true);
    this->UseIgnite = menu_mechanics->CheckBox("Use Ignite", true);
    this->LowFPSMode = menu_mechanics->CheckBox("Use Low FPS Mode", false);
    this->AutoEUnitInRage = menu_mechanics->CheckBox("Auto E Enemies", true);
    this->AutoEUnitInRagePct = menu_mechanics->AddInteger("- If Energy >", 0, 200, 100);
    this->JungleOrderPriority = menu_mechanics->AddSelection("Jungle Target Priority:", 1, std::vector<std::string> {"Low Health", "Max Health", "Closest to Cursor" });

    auto menu_render = menu->AddMenu("::Render");
    this->DrawQ = menu_render->CheckBox("Enabled Q Drawings", true);
    this->DrawQColor = menu_render->AddColor("- Color Q", 102, 204, 204, 200);
    this->DrawW = menu_render->CheckBox("Enabled W Drawings", true);
    this->DrawWColor = menu_render->AddColor("- Color W", 102, 204, 204, 200);
    this->DrawE = menu_render->CheckBox("Enabled E Drawings", false);
    this->DrawEColor = menu_render->AddColor("- Color E", 102, 204, 204, 200);
    this->DrawR = menu_render->CheckBox("Enabled R Drawings", true);
    this->DrawRColor = menu_render->AddColor("- Color R", 102, 204, 204, 200);
    this->DrawAssassinRange = menu_render->CheckBox("Enable Assassin Range Drawings", true);
    this->DrawAssassinRangeColor = menu_render->AddColor("- Color Assassin Range", 255, 102, 0, 200);
    this->DrawComboDamage = menu_render->CheckBox("Draw Combo Damage", true);
    this->DrawComboDamageColor = menu_render->AddColor("- Color Combo Damage", 102, 204, 204, 200);

    auto menu_avoider = menu->AddMenu("::Avoider");
    this->UseRAvoider = menu_avoider->CheckBox("R Avoider", true);
    this->SpellsToAvoid = std::map<std::string, IMenuOption *>();

    for (auto hero : GEntityList->GetAllHeros(false, true)) {
        for (auto i : ZZed::AvoidList) {
            if (strcmp(hero->ChampionName(), i.second->ChampName.c_str()) == 0) {
                auto uniqueHero = std::string("- ").append(i.second->ChampName).append(" R");
                this->SpellsToAvoid.insert(std::pair<std::string, IMenuOption *>(i.first, menu_avoider->CheckBox(uniqueHero.c_str(), true))); } } }

    auto menu_debug = menu->AddMenu("::Debug");
    this->Debug = menu_debug->CheckBox("Debug Damage", false);
    this->DrawPredictedShadow = menu_debug->CheckBox("Debug Shadow Position", false);
    this->DebugPathfinding = menu_debug->CheckBox("Debug Shadow Pathfinding", false);

    this->Menu = menu; }

