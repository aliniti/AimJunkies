#include "PluginSDK.h"
#include "ZCamilleMenu.h"
#include "ZCamille.h"

ZCamilleMenu::ZCamilleMenu(IMenu * menu) {
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
    this->ComboEPred = menucb->AddSelection("- Prediction:", 0, std::vector<std::string>({ "Disabled", "Enabled" }));
    this->MinimumERange = menucb->AddInteger("- Minimum Range", 0, 1100, 165);
    this->EnhancedEPrecision = menucb->CheckBox("- Enhanced Precision", false);
    this->PreferedHookPoint = menucb->AddSelection("- Prefered E Spot:", 0, std::vector<std::string> { "Closest To Hero", "Closest To Me" });
    this->ComboR = menucb->CheckBox("Use R", true);
    this->RWhen = menucb->AddSelection("- When:", 2, std::vector<std::string>({ "Easy", "Normal", "Hard", "Challenging" }));
    this->ForceROrbwalk = menucb->CheckBox("- Orbwalk Focus Target", true);
    this->UltOnlySelected = menucb->CheckBox("- Selected Target Only", false);
    //this->ExpirimentalCombo = menucb->CheckBox("Use Expiremental Combo", false);
    auto menuhr = this->Menu->AddMenu("::Harass");
    this->HarassMana = menuhr->AddInteger("Minimum Mana %", 0, 100, 75);
    this->HarassQ = menuhr->CheckBox("Use Q", true);
    this->HarassW = menuhr->CheckBox("Use W", true);
    auto menufl = this->Menu->AddMenu("::Flee");
    this->FleeE = menufl->CheckBox("Use E", true);
    auto menufm = this->Menu->AddMenu("::Farming");
    this->FarmingWithItems = menufm->CheckBox("Farm With Items", true);
    this->FarmNearEnemies = menufm->CheckBox("Farm Near Enemies", false);
    this->WaveClearOverride = menufm->AddInteger(" - Minimum Mana Override %", 0, 100, 75);
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
    //auto menuev = this->Menu->AddMenu("::Events");
    //this->UseAntiGapcloser = menuev->CheckBox("Anti-Gapcloser", false);
    //this->UseInterrupter = menuev->CheckBox("Use Interrupter", true);
    auto menumg = this->Menu->AddMenu("::Magnet");
    this->MagnetW = menumg->CheckBox("Magner W [Beta]", true);
    this->MagnetWCombo = menumg->CheckBox("- Combo", true);
    this->MagnetWHarass = menumg->CheckBox("- Harass", true);
    this->MagnetWClear = menumg->CheckBox("- Clear", true);
    auto menurd = this->Menu->AddMenu("::Render");
    this->DrawW = menurd->CheckBox("Enabled W Drawings", false);
    this->DrawWColor = menurd->AddColor("- Color W", 102, 204, 204, 200);
    this->DrawE = menurd->CheckBox("Enabled E Drawings", true);
    this->DrawEColor = menurd->AddColor("- Color E", 102, 204, 204, 200);
    this->DrawR = menurd->CheckBox("Enabled R Drawings", true);
    this->DrawRColor = menurd->AddColor("- Color R", 255, 255, 255, 200);
    this->DrawComboDamage = menurd->CheckBox("Draw Combo Damage", true);
    this->DrawComboDamageColor = menurd->AddColor("- Color Combo Damage", 102, 204, 204, 200);

    auto menuav = this->Menu->AddMenu("::Avoider");
    this->DontEUnderTurret = menuav->CheckBox("Avoid E Into Turret", true);
    this->DontEUnderTurretToggle = menuav->AddKey("- Toggle Key:", 'J');
    this->AvoidEventHorizon = menuav->CheckBox("Avoid E Into Veigar", true);
    this->UseRAvoider = menuav->CheckBox("R Avoider", true);
    this->SpellsToAvoid = std::map<std::string, IMenuOption *>();

    for(auto hero : GEntityList->GetAllHeros(false, true)) {
        for(auto i : ZCamille::AvoidList) {
            if(strcmp(hero->ChampionName(), i.second->ChampName.c_str()) == 0) {
                auto uniqueHero = std::string("- ").append(i.second->ChampName).append(" R");
                this->SpellsToAvoid.insert(std::pair<std::string, IMenuOption *>(i.first, menuav->CheckBox(uniqueHero.c_str(), true))); } } } }
