#include <utility>
#include "ZNidaleeMenu.h"

ZNidaleeMenu::ZNidaleeMenu(IMenu * menu) {
    this->Menu = menu;

    auto mainMenu = this->Menu->AddMenu("::Hotkeys");
    this->ComboButton = mainMenu->AddKey("Combo [active]", 32);
    this->HarassButton = mainMenu->AddKey("Harass [active]", 'G');
    this->ClearButton = mainMenu->AddKey("Wave/Jungle [active]", 'V');
    this->FleeButton = mainMenu->AddKey("Flee/Walljumper [active]", 'A');

    auto humanMenu = this->Menu->AddMenu("::Human");

    auto javelinMenu = humanMenu->AddMenu("::[Q] Javelin");
    this->CheckSpearHitChance = javelinMenu->CheckBox("Check HitChance", true);
    HumanQMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Javelinco", std::make_pair(javelinMenu->CheckBox("Use In Combo", true), 0)));
    HumanQMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Javelinha", std::make_pair(javelinMenu->CheckBox("Use In Harass", true), 0)));
    HumanQMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Javelinjg", std::make_pair(javelinMenu->CheckBox("Use In Jungle", true), 0)));
    HumanQMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Javelinwc", std::make_pair(javelinMenu->CheckBox("Use In WaveClear", false), 0)));

    auto bushwhackMenu = humanMenu->AddMenu("::[W] Bushwhack");
    HumanWMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Bushwhackco", std::make_pair(bushwhackMenu->CheckBox("Use In Combo", false), 0)));
    HumanWMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Bushwhackha", std::make_pair(bushwhackMenu->CheckBox("Use In Harass", false), 0)));
    this->ReduceWUsage = bushwhackMenu->CheckBox("- Reduce W Usage", true);
    HumanWMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Bushwhackjg", std::make_pair(bushwhackMenu->CheckBox("Use In Jungle", true), 0)));
    HumanWMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Bushwhackwc", std::make_pair(bushwhackMenu->CheckBox("Use In WaveClear", false), 0)));
    this->BushwhackLocation = bushwhackMenu->AddSelection("Location: ", 0, std::vector<std::string>({ "Behind Target" }));

    auto primalsurgeMenu = humanMenu->AddMenu("::[E] Primalsurge");
    HumanEMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Primalsurgeon", std::make_pair(primalsurgeMenu->CheckBox("Enable Healing", true), 0)));
    CougarRMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Aspectcougarheal", std::make_pair(primalsurgeMenu->CheckBox("Auto Switch Forms", true), 0)));
    this->AllyHealPriroty = primalsurgeMenu->AddSelection("Ally Priority: ", 0, std::vector<std::string>({ "Low HP", "Most AD/AP", "Max Health" }));
    this->HealingManaPct = primalsurgeMenu->AddInteger("Minimum Mana %", 1, 100, 55);

    for (auto hero : GEntityList->GetAllHeros(true, false)) {
        auto uniqueStr = std::string("Use on ").append(hero->ChampionName());
        this->AllyHealMap.insert(std::pair<std::string, IMenuOption *>(hero->ChampionName() + std::string("on"), primalsurgeMenu->CheckBox(uniqueStr.c_str(), true)));

        auto uniqueStr2 = std::string("- ").append(hero->ChampionName()).append(" Min Health %");
        this->AllyHealMap.insert(std::pair<std::string, IMenuOption *>(hero->ChampionName() + std::string("pct"), primalsurgeMenu->AddInteger(uniqueStr2.c_str(), 1, 99, 88))); }

    auto aspectHumanMenu = humanMenu->AddMenu("::[R] Aspect of the Cougar");
    HumanRMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Aspecthumanco", std::make_pair(aspectHumanMenu->CheckBox("Use In Combo", true), 0)));
    this->RequireSwipeTakedownCombo = aspectHumanMenu->CheckBox("- Require Swipe/Takedown", true);
    HumanRMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Aspecthumanha", std::make_pair(aspectHumanMenu->CheckBox("Use In Harass", true), 0)));
    HumanRMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Aspecthumanjg", std::make_pair(aspectHumanMenu->CheckBox("Use In Jungle", true), 0)));
    this->RequireSwipeTakedownJungle = aspectHumanMenu->CheckBox("- Require Swipe/Takedown", true);
    HumanRMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Aspecthumanwc", std::make_pair(aspectHumanMenu->CheckBox("Use In WaveClear", true), 0)));
    this->CheckCatSwitchNearEnemies = aspectHumanMenu->CheckBox("- Dont R Near Enemies", true);
    this->CheckCatSwitchNearTurret = aspectHumanMenu->CheckBox("- Dont R Near Turret", true);

    auto cougarMenu = this->Menu->AddMenu("::Cougar");

    auto takedownMenu = cougarMenu->AddMenu("::[Q] Takedown");
    CougarQMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Takedownco", std::make_pair(takedownMenu->CheckBox("Use In Combo", true), 0)));
    CougarQMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Takedownha", std::make_pair(takedownMenu->CheckBox("Use In Harass", true), 0)));
    CougarQMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Takedownjg", std::make_pair(takedownMenu->CheckBox("Use In Jungle", true), 0)));
    CougarQMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Takedownwc", std::make_pair(takedownMenu->CheckBox("Use In WaveClear", true), 0)));

    auto pounceMenu = cougarMenu->AddMenu("::[W] Pounce");
    this->CheckPounceHitChance = pounceMenu->CheckBox("Check Hitchance", false);
    this->CheckPounceHitChanceMin = pounceMenu->AddSelection("- Min HitChance: ", 2, std::vector<std::string>({ "Low", "Medium", "High", "Very High" }));
    CougarWMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Pounceco", std::make_pair(pounceMenu->CheckBox("Use In Combo", true), 0)));
    this->IgnorePounceChecksOnHuntedTarget = pounceMenu->CheckBox("- Ignore Checks if Hunted", false);
    this->CheckPounceMinRangeCombo = pounceMenu->CheckBox("- Pounce Only if > AARange", true);
    CougarWMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Pounceha", std::make_pair(pounceMenu->CheckBox("Use In Harass", false), 0)));
    CougarWMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Pouncejg", std::make_pair(pounceMenu->CheckBox("Use In Jungle", true), 0)));
    CougarWMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Pouncewc", std::make_pair(pounceMenu->CheckBox("Use In WaveClear", true), 0)));
    this->CheckPounceMinRangeWaveClear = pounceMenu->CheckBox("- Pounce Only if > AARange", true);
    this->CheckPounceIntoEnemiesWaveClear = pounceMenu->CheckBox("- Dont W into Enemies", true);
    this->CheckPounceIntoTurretWaveClear = pounceMenu->CheckBox("- Dont W into Turret", true);


    auto swipeMenu = cougarMenu->AddMenu("::[E] Swipe");
    CougarEMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Swipeco", std::make_pair(swipeMenu->CheckBox("Use In Combo", true), 0)));
    this->CheckSwipeHitchange = swipeMenu->CheckBox("Check Hitchance", false);
    this->CheckSwipeHitChanceMin = swipeMenu->AddSelection("- Min HitChance: ", 2, std::vector<std::string>({ "Low", "Medium", "High", "Very High" }));
    CougarEMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Swipeha", std::make_pair(swipeMenu->CheckBox("Use In Harass", true), 0)));
    CougarEMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Swipejg", std::make_pair(swipeMenu->CheckBox("Use In Jungle", true), 0)));
    CougarEMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Swipewc", std::make_pair(swipeMenu->CheckBox("Use In WaveClear", true), 0)));
    this->CheckSwipeMinimumHitWaveClear = swipeMenu->AddInteger("- Minimum Minions Hit", 1, 5, 3);

    auto aspectCougarMenu = cougarMenu->AddMenu("::[R] Aspect of the Human");
    CougarRMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Aspectcougarco", std::make_pair(aspectCougarMenu->CheckBox("Use In Combo", true), 0)));
    CougarRMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Aspectcougarha", std::make_pair(aspectCougarMenu->CheckBox("Use In Harass", true), 0)));
    CougarRMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Aspectcougarjg", std::make_pair(aspectCougarMenu->CheckBox("Use In Jungle", true), 0)));
    CougarRMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Aspectcougarwc", std::make_pair(aspectCougarMenu->CheckBox("Use In WaveClear", false), 0)));

    auto mechanicsMenu = this->Menu->AddMenu("::Mechanics");
    this->CurrentPlayStyle = mechanicsMenu->AddSelection("Playstyle: ", 1, std::vector<std::string>({ "Single Target", "Multi-Target" }));
    this->PredictionSelection = mechanicsMenu->AddSelection("Prediction: ", 0, std::vector<std::string>({ "Default" }));
    this->CheckSpearHitChanceMin = mechanicsMenu->AddSelection("- Min HitChance", 2, std::vector<std::string>({ "Low", "Medium", "High", "Very High" }));
    this->JungleOrderPriority = mechanicsMenu->AddSelection("Jungle Target Priority", 1, std::vector<std::string>({ "Low HP", "MaxHealth", "Distance To Cursor" }));

    auto renderMenu = this->Menu->AddMenu("::Render");
    this->DrawQ = renderMenu->CheckBox("Enabled Q Drawings", true);
    this->DrawQColor = renderMenu->AddColor("- Color Q", 121, 77, 255, 200);
    this->DrawE = renderMenu->CheckBox("Enabled E Drawings", true);
    this->DrawEColor = renderMenu->AddColor("- Color E", 121, 77, 255, 200);
    this->DrawW2 = renderMenu->CheckBox("Enabled W Drawings", true);
    this->DrawW2Color = renderMenu->AddColor("- Color W", 121, 77, 255, 200);
    this->DrawQTimer = renderMenu->CheckBox("Draw Q Timer", true);

    //auto smiteMenu = this->Menu->AddMenu("::Smite Settings");
    //this->EnableSmite = smiteMenu->CheckBox("Enable Smite", false);
    //this->TakedownSmite = smiteMenu->CheckBox("Takedown + Smite", true);
    //this->SmiteEpic = smiteMenu->CheckBox("- Smite Epic", true);
    //this->SmiteLarge = smiteMenu->CheckBox("- Smite Large", true);
    //this->SmiteSmall = smiteMenu->CheckBox("- Smite Small", true);
    //this->SmiteOnHero = smiteMenu->CheckBox("- Smite ON hero", true);

    auto eventMenu = this->Menu->AddMenu("::Events");
    //this->AutoLevelR = eventMenu->CheckBox("Auto [R] Level Up", true);

    HumanQMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Javelinimm", std::make_pair(eventMenu->CheckBox("Auto [Q] Javelin Immobile", true), 0)));
    HumanWMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Bushwhackimm", std::make_pair(eventMenu->CheckBox("Auto [W] Bushwhack Immobile", true), 0)));

    //CougarRMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Aspectcougargap", std::make_pair(eventMenu->CheckBox("Auto [R] Enemy Gapclosers", true), 0)));
    //CougarEMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Swipegap", std::make_pair(eventMenu->CheckBox("Auto [E] Swipe Gapclosers", true), 0)));
    //HumanQMap.insert(std::pair<std::string, std::pair<IMenuOption *, int>>("Javelingap", std::make_pair(eventMenu->CheckBox("Auto [Q] Javelin Gapclosers", true), 0)));
    this->PracticeToolsDebug = this->Menu->CheckBox("Debug/Practice Tools", false);


}

