#include "ZEliseMenu.h"

ZEliseMenu::ZEliseMenu(IMenu * menu) {
    this->Menu = menu;

    auto mainMenu = this->Menu->AddMenu("::Hotkeys");
    this->ComboButton = mainMenu->AddKey("Combo [active]", 32);
    this->FarmButton = mainMenu->AddKey("Wave/Jungleclear [active]", 'V');
    this->HarassButton = mainMenu->AddKey("Harass [active]", 'G');
    this->FleeButton = mainMenu->AddKey("Flee [active]", 'A');

    auto HumanMenu = this->Menu->AddMenu("::Human");

    auto menuHQ = HumanMenu->AddMenu("::[Q] Neurotoxin");
    HumanQMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("neurotoxinco", std::make_pair(menuHQ->CheckBox("Use In Combo", true), 0)));
    HumanQMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("neurotoxinha", std::make_pair(menuHQ->CheckBox("Use In Harass", true), 0)));
    this->NeurotoxinHarassMinMana = menuHQ->AddInteger("- Harass Min Mana %", 0, 100, 66);
    HumanQMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("neurotoxinjg", std::make_pair(menuHQ->CheckBox("Use In Jungle", true), 0)));
    this->NeurotoxinJungleMinMana = menuHQ->AddInteger("- Jungle Min Mana %", 0, 100, 35);
    HumanQMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("neurotoxinwc", std::make_pair(menuHQ->CheckBox("Use In WaveClear", false), 0)));

    auto menuHW = HumanMenu->AddMenu("::[W] Volotile Spider");
    HumanWMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("volatilespiderco", std::make_pair(menuHW->CheckBox("Use In Combo", true), 0)));
    HumanWMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("volatilespiderha", std::make_pair(menuHW->CheckBox("Use In Harass", true), 0)));
    this->VolotileSpiderHarassMinMana = menuHW->AddInteger("- Harass Min Mana %", 0, 100, 66);
    HumanWMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("volatilespiderjg", std::make_pair(menuHW->CheckBox("Use In Jungle", true), 0)));
    this->VolotileSpiderJungleMinMana = menuHW->AddInteger("- Jungle Min Mana %", 0, 100, 35);
    HumanWMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("volatilespiderwc", std::make_pair(menuHW->CheckBox("Use In WaveClear", false), 0)));

    auto menuHE = HumanMenu->AddMenu("::[E] Cocoon");
    HumanEMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("cocoonco", std::make_pair(menuHE->CheckBox("Use In Combo", true), 0)));
    this->CocoonComboHitChance = menuHE->AddSelection("- Combo HitChance:", 2, std::vector<std::string>({ "Low", "Medium", "High", "Very High" }));
    HumanEMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("cocoonha", std::make_pair(menuHE->CheckBox("Use In Harass", true), 0)));
    this->CocoonHarassHitChance = menuHE->AddSelection("- Harass HitChance:", 3, std::vector<std::string>({ "Low", "Medium", "High", "Very High" }));
    this->CocoonHarassMinMana = menuHE->AddInteger("- Harass Min Mana %", 0, 100, 66);
    HumanEMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("cocoonjg", std::make_pair(menuHE->CheckBox("Use In Jungle", true), 0)));
    this->CocoonJungleMinMana = menuHE->AddInteger("- Jungle Min Mana %", 0, 100, 35);
    this->DontCocoonJungleNearEnemies = menuHE->CheckBox("- Dont Use Near Enemies", true);
    HumanEMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("cocoonwc", std::make_pair(menuHE->CheckBox("Use In WaveClear", false), 0)));

    auto menuHR = HumanMenu->AddMenu("::[R] Human -> Spider");
    HumanRMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("transformhumanco", std::make_pair(menuHR->CheckBox("Use In Combo", true), 0)));
    this->HumanToSpiderComboRequiredSkills = menuHR->AddSelection("- Required Ready Spells:", 0, std::vector<std::string>({ "Any Ready", "Spider Q + W", "Spider Q or E" }));
    HumanRMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("transformhumanha", std::make_pair(menuHR->CheckBox("Use In Harass", true), 0)));;
    this->HumanToSpiderHarassRequiredSkills = menuHR->AddSelection("- Required Ready Spells:", 0, std::vector<std::string>({ "Any Ready", "Spider Q + W", "Spider Q or E" }));
    HumanRMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("transformhumanjg", std::make_pair(menuHR->CheckBox("Use In Jungle", true), 0)));
    HumanRMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("transformhumanwc", std::make_pair(menuHR->CheckBox("Use In WaveClear", true), 0)));

    auto spiderMenu = this->Menu->AddMenu("::Spider");
    auto menuSQ = spiderMenu->AddMenu("::[Q] Venomous Bite");
    SpiderQMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("venomousbiteco", std::make_pair(menuSQ->CheckBox("Use In Combo", true), 0)));
    SpiderQMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("venomousbiteha", std::make_pair(menuSQ->CheckBox("Use In Harass", true), 0)));
    SpiderQMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("venomousbitejg", std::make_pair(menuSQ->CheckBox("Use In Jungle", true), 0)));
    SpiderQMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("venomousbitewc", std::make_pair(menuSQ->CheckBox("Use In WaveClear", true), 0)));

    auto menuSW = spiderMenu->AddMenu("::[W] Skittering Frenzy");
    SpiderWMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("frenzyco", std::make_pair(menuSW->CheckBox("Use In Combo", true), 0)));
    SpiderWMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("frenzyha", std::make_pair(menuSW->CheckBox("Use In Harass", true), 0)));
    SpiderWMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("frenzyjg", std::make_pair(menuSW->CheckBox("Use In Jungle", true), 0)));
    SpiderWMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("frenzywc", std::make_pair(menuSW->CheckBox("Use In WaveClear", true), 0)));

    auto menuSE = spiderMenu->AddMenu("::[E] Rappel");
    SpiderEMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("rapellco", std::make_pair(menuSE->CheckBox("Use In Combo", true), 0)));
    SpiderEMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("rapellha", std::make_pair(menuSE->CheckBox("Use In Harass", false), 0)));
    SpiderEMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("rapelljg", std::make_pair(menuSE->CheckBox("Use In Jungle", false), 0)));
    SpiderEMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("rapellwc", std::make_pair(menuSE->CheckBox("Use In WaveClear", false), 0)));

    auto menuSR = spiderMenu->AddMenu("::[R] Spider -> Human");
    SpiderRMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("transformspiderco", std::make_pair(menuSR->CheckBox("Use In Combo", true), 0)));
    this->SpiderToHumanComboRequiredSkills = menuSR->AddSelection("- Required Ready Spells:", 0, std::vector<std::string>({ "Any Ready", "Human Q + W", "Human Q or E" }));
    SpiderRMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("transformspiderha", std::make_pair(menuSR->CheckBox("Use In Harass", true), 0)));
    this->SpiderToHumanHarassRequiredSkills = menuSR->AddSelection("- Required Ready Spells:", 0, std::vector<std::string>({ "Any Ready", "Human Q + W", "Human Q or E" }));
    SpiderRMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("transformspiderjg", std::make_pair(menuSR->CheckBox("Use In Jungle", true), 0)));
    SpiderRMap.insert(std::pair<std::string, std::pair<IMenuOption *, float>>("transformspiderwc", std::make_pair(menuSR->CheckBox("Use In WaveClear", true), 0)));

    auto mechanicsMenu = this->Menu->AddMenu("::Mechanics");
    this->UseProtobelt = mechanicsMenu->CheckBox("Use Protobelt in Burst", true);
    this->BlockAutoAttacks = mechanicsMenu->CheckBox("Block Auto Attacks in Burst", true);
    this->JungleOrderPriority = mechanicsMenu->AddSelection("Jungle Target Priority", 1, std::vector<std::string>({ "Low HP", "MaxHealth", "Distance To Cursor" }));
    //this->BetaCollisionChecks = mechanicsMenu->CheckBox("Beta Collision Checks", false);

    auto renderMenu = this->Menu->AddMenu("::Render");
    this->DrawQ = renderMenu->CheckBox("Enabled Q Drawings", true);
    this->DrawQColor = renderMenu->AddColor("- Color Q", 121, 77, 255, 200);
    this->DrawE = renderMenu->CheckBox("Enabled E Drawings", true);
    this->DrawEColor = renderMenu->AddColor("- Color E", 121, 77, 255, 200);

    this->PracticeToolsDebug = this->Menu->CheckBox("Debug/Practice Tools", false); }
