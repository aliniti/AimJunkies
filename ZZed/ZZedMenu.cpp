#include "ZZedMenu.h"
#include "ZZed.h"

ZZedMenu::ZZedMenu(IMenu * menu) {
    this->Menu = menu;

    auto menuhk = this->Menu->AddMenu("::Keys");
    this->ComboKey = menuhk->AddKey("Combo [active]", 32);
    this->HarassKey = menuhk->AddKey("Harass [active]", 'G');
    this->ClearKey = menuhk->AddKey("Clear [active]", 'V');
    this->FleeKey = menuhk->AddKey("Flee ", 'A');

    auto menucb = this->Menu->AddMenu("::Main");

    auto menuiq = menucb->AddMenu("::[Q] Razor Shuriken");
    this->UseComboQ = menuiq->CheckBox("Use Q in Combo", true);
    this->ExtendedQCombo = menuiq->CheckBox("- Use Extended Q", true);
    //this->ExComboQMaxDamage = menuiq->CheckBox("- Only Max Damage Q", false);
    this->UseHarassQ = menuiq->CheckBox("Use Q in Harass", true);
    this->ExtendedQHarass = menuiq->CheckBox("- Use Extended Q", false);
    //this->ExHarassQMaxDamage = menuiq->CheckBox("- Only Max Damage Q", false);
    this->UseJungleQ = menuiq->CheckBox("Use Q in Jungle", true);

    auto menuiw = menucb->AddMenu("::[W] Living Shadow");
    this->UseComboW = menuiw->CheckBox("Use W in Combo", true);
    this->SwapForKill = menuiw->CheckBox("- Swap for Kill", true);
    this->UseHarassW = menuiw->CheckBox("Use W in Harass", true);
    this->UseHarassWPF = menuiw->AddSelection("- Shadow Placement:", 0, std::vector<std::string> { "Unit Position", "Pathfinding", "Auto", });
    this->UseJungleW = menuiw->CheckBox("Use W in Jungle", true);
    this->DontWJungleNearEnemy = menuiw->CheckBox("- Dont Use Near Enemy", true);
    this->UseFleeW = menuiw->CheckBox("Use W in Flee", true);

    auto menuie = menucb->AddMenu("::[E] Shadow Slash");
    this->UseComboE = menuie->CheckBox("Use E in Combo", true);
    this->UseHarassE = menuie->CheckBox("Use E in Harass", true);
    this->UseJungleE = menuie->CheckBox("Use E in Jungle", true);

    this->DeathMarkMenu = menucb->AddMenu("::[R] Death Mark");
    this->UseComboR = this->DeathMarkMenu->CheckBox("Use R in Combo", true);
    this->UltMode = this->DeathMarkMenu->AddSelection("- Mode:", 0, std::vector<std::string> {"Only Kill", "Duel" });
    this->ShadowPlacement = this->DeathMarkMenu->AddSelection("- Shadow Placement:", 0, std::vector<std::string> {"Line", "Triangle", "Pathfinder" });
    this->SwapRIfDead = this->DeathMarkMenu->CheckBox("- Swap Back if Dead", true);
    this->AutoR = this->DeathMarkMenu->CheckBox("- Killsteal", true);

    this->AlwaysRSelected = this->DeathMarkMenu->CheckBox("- Always R on Focus Target", true);
    this->BlackListR = this->DeathMarkMenu->CheckBox("Blacklist R", true);

    for(auto i : GEntityList->GetAllHeros(false, true)) {
        if(!i->IsDead()) {
            this->BlackListRTargets[i->GetNetworkId()] = this->DeathMarkMenu->CheckBox(std::string("- Dont R on").append(" ").append(i->ChampionName()).c_str(), false); } }

    auto menuem = this->Menu->AddMenu("::Energy");
    this->MinimumHarassEnergy = menuem->AddInteger("Minimum Harass Energy", 0, 200, 100);
    this->MinimumClearEnergy = menuem->AddInteger("Minimum Clear Energy", 0, 200, 50);

    auto menumc = this->Menu->AddMenu("::Mechanics");
    this->UseItemsCombo = menumc->CheckBox("Use Items", true);
    this->UseIgnite = menumc->CheckBox("Use Ignite", true);
    this->AutoEUnitInRage = menumc->CheckBox("Auto E Enemies", true);
    this->AutoEUnitInRagePct = menumc->AddInteger("- If Energy >", 0, 200, 100);
    this->JungleOrderPriority = menumc->AddSelection("Jungle Target Priority:", 1, std::vector<std::string> {"Low Health", "Max Health", "Closest to Cursor" });

    auto menurd = this->Menu->AddMenu("::Render");
    this->DrawQ = menurd->CheckBox("Enabled Q Drawings", true);
    this->DrawQColor = menurd->AddColor("- Color Q", 102, 204, 204, 200);
    this->DrawW = menurd->CheckBox("Enabled W Drawings", true);
    this->DrawWColor = menurd->AddColor("- Color W", 102, 204, 204, 200);
    this->DrawE = menurd->CheckBox("Enabled E Drawings", false);
    this->DrawEColor = menurd->AddColor("- Color E", 102, 204, 204, 200);
    this->DrawR = menurd->CheckBox("Enabled R Drawings", true);
    this->DrawRColor = menurd->AddColor("- Color R", 102, 204, 204, 200);
    this->DrawComboDamage = menurd->CheckBox("Draw Combo Damage", true);
    this->DrawComboDamageColor = menurd->AddColor("- Color Combo Damage", 102, 204, 204, 200);

    auto menuav = this->Menu->AddMenu("::Avoider");
    this->UseRAvoider = menuav->CheckBox("R Avoider", true);
    this->SpellsToAvoid = std::map<std::string, IMenuOption *>();

    for(auto hero : GEntityList->GetAllHeros(false, true)) {
        for(auto i : ZZed::AvoidList) {
            if(strcmp(hero->ChampionName(), i.second->ChampName.c_str()) == 0) {
                auto uniqueHero = std::string("- ").append(i.second->ChampName).append(" R");
                this->SpellsToAvoid.insert(std::pair<std::string, IMenuOption *>(i.first, menuav->CheckBox(uniqueHero.c_str(), true))); } } }

    auto menudb = this->Menu->AddMenu("::Debug");
    this->Debug = menudb->CheckBox("Debug Damage", false);
    this->DrawPredictedShadow = menudb->CheckBox("Debug Shadow Position", false);
    this->DebugPathfinding = menudb->CheckBox("Debug Shadow Pathfinding", false); }

