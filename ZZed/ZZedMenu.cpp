#include "ZZedMenu.h"

ZZedMenu::ZZedMenu(IMenu * menu) {
    this->Menu = menu;

    auto menuhk = this->Menu->AddMenu("::Hotkeys");
    this->ComboKey = menuhk->AddKey("Combo [active]", 32);
    this->HarassKey = menuhk->AddKey("Harass [active]", 'G');
    //this->ClearKey = menuhk->AddKey("Clear [active]", 'V');
    this->FleeKey = menuhk->AddKey("Flee ", 'A');

    auto menucb = this->Menu->AddMenu("::Main Settings");

    auto menuiq = menucb->AddMenu("::[Q] Razor Shuriken");
    this->UseComboQ = menuiq->CheckBox("Use Q in Combo", true);
    this->ExtendedQCombo = menuiq->CheckBox("- Use Extended Q", false);
    //this->ExComboQMaxDamage = menuiq->CheckBox("- Only Max Damage Q", false);
    this->UseHarassQ = menuiq->CheckBox("Use Q in Harass", true);
    this->ExtendedQHarass = menuiq->CheckBox("- Use Extended Q", true);
    //this->ExHarassQMaxDamage = menuiq->CheckBox("- Only Max Damage Q", false);


    auto menuiw = menucb->AddMenu("::[W] Living Shadow");
    this->UseComboW = menuiw->CheckBox("Use W in Combo", true);
    this->UseHarassW = menuiw->CheckBox("Use W in Harass", true);
    this->UseHarassWPF = menuiw->AddSelection("- Shadow Placement:", 0, std::vector<std::string> { "Unit Position (W-Q->E)", "Pathfinding" });
    this->UseFleeW = menuiw->CheckBox("Use W in Flee", true);

    auto menuie = menucb->AddMenu("::[E] Shadow Slash");
    this->UseComboE = menuie->CheckBox("Use E in Combo", true);
    this->UseHarassE = menuie->CheckBox("Use E in Harass", true);

    auto menuir = menucb->AddMenu("::[R] Death Mark");
    this->UseComboR = menuir->CheckBox("Use R in Combo", true);
    this->UltMode = menuir->AddSelection("- Mode:", 0, std::vector<std::string> {"Only Kill", "Duel" });
    this->ShadowPlacement = menuir->AddSelection("- Shadow Placement:", 0, std::vector<std::string> {"Line", "Triangle", "Pathfinder" });
    this->SwapRIfDead = menuir->CheckBox("- Swap Back if Dead", true);
    this->AutoR = menuir->CheckBox("- Killsteal", true);

    this->UseAlwaysR = menuir->CheckBox("Enable Always R", false);
    this->AlwaysRSelected = menuir->CheckBox("- Always R on Focus Target", true);

    for(auto i : GEntityList->GetAllHeros(false, true)) {
        if(!i->IsDead()) {
            this->AlwaysRTargets[i->ChampionName()] = menuir->CheckBox(std::string("- Always R on").append(" ").append(i->ChampionName()).c_str(), true); } }

    auto menuem = this->Menu->AddMenu("::Energy Management");
    this->MinimumHarassEnergy = menuem->AddInteger("Minimum Harass Energy", 0, 200, 100);
    //this->MinimumClearEnergy = menuem->AddInteger("Minimum Clear Energy", 0, 200, 50);

    auto menumc = this->Menu->AddMenu("::Mechanics");
    this->UseItemsCombo = menumc->CheckBox("Use Items", true);
    this->AutoEUnitInRage = menumc->CheckBox("Auto E Enemies", true);
    this->AutoEUnitInRagePct = menumc->AddInteger("- If Energy >", 0, 200, 100);
    this->JungleOrderPriority = menumc->AddSelection("Jungle Target Priority:", 1, std::vector<std::string> {"Low Health", "Max Health", "Closest to Cursor" });

    auto menurd = this->Menu->AddMenu("::Render");
    this->DrawComboDamage = menurd->CheckBox("Draw Combo Damage", true);
    this->DrawComboDamageColor = menurd->AddColor("- Color Combo Damage", 121, 77, 255, 200);

    auto menudb = this->Menu->AddMenu("::Debug");
    this->Debug = menudb->CheckBox("Debug Damage", false);
    this->DrawPredictedShadow = menudb->CheckBox("Debug Shadow Position", false);
    this->DebugPathfinding = menudb->CheckBox("Debug Shadow Pathfinding", false); }

