#pragma once
#include "PluginSDK.h"
#include "ZNidaleeMenu.h"
#include "ZNidaleeModes.h"
#include "ZNidaleeExtensions.h"

#include <map>
#include <string>

#define M_PI 3.14159265358979323846

class ZNidalee {
    public:
        ~ZNidalee();

        static void OnBoot();
        static IUnit * STarg;

        static ISpell * SpellQ;
        static ISpell * SpellW;
        static ISpell * SpellE;
        static ISpell * SpellR;

        static ISpell2 * SJavelin;
        static ISpell2 * SBushwhack;
        static ISpell2 * SPounce;
        static ISpell2 * SSwipe;

        static DelayAction * Delay;
        static eCollisionFlags QCollisionFlags;
        static std::map<std::string, int> TimeStamps;
        static std::map<float, std::pair<int, int>> TheHunted;

        static ZNidaleeModes * Modes;
        static ZNidaleeMenu * Menu;
        static ZNidaleeExtensions * Ex;

        static IUnit * Player;
        static bool CatForm();
        static bool IsHunted(IUnit * unit);

        static bool CompareHighHealth(IUnit * a, IUnit * b);
        static bool CompareMaxHealth(IUnit * a, IUnit * b);
        static bool CompareDamage(IUnit * a, IUnit * b);
        static bool CompareLowHealth(IUnit * a, IUnit * b);
        static bool CompareDistanceToCursor(IUnit * a, IUnit * b);
        static bool GetPriorityHealTarget(IUnit * a, IUnit * b);
        static bool GetPriorityJungleTarget(IUnit * a, IUnit * b);
        static bool CanUse(ISpell * spell, bool human, std::string mode, int time);
        static void CastSpear(IUnit * unit, std::string mode);
        static void PlaceTrap(IUnit * unit, std::string mode);
        static void Bite(IUnit * unit, std::string mode);
        static void Pounce(IUnit * unit, std::string mode);
        static void Swipe(IUnit * unit, std::string mode);
        static void SwitchForm(IUnit * unit, std::string mode);
        static void Flee(); };

inline void ZNidalee::OnBoot() {
    TimeStamps.insert(std::pair<std::string, int>("Takedown", 5));
    TimeStamps.insert(std::pair<std::string, int>("Swipe", 5));
    TimeStamps.insert(std::pair<std::string, int>("Pounce", 5));
    TimeStamps.insert(std::pair<std::string, int>("Javelin", 5));
    TimeStamps.insert(std::pair<std::string, int>("Bushwhack", 5));
    TimeStamps.insert(std::pair<std::string, int>("Primalsurge", 5));
    TimeStamps.insert(std::pair<std::string, int>("Aspect", 5));

    SpellQ = GPluginSDK->CreateSpell(kSlotQ, 200);
    SpellW = GPluginSDK->CreateSpell(kSlotW, 375);
    SpellE = GPluginSDK->CreateSpell(kSlotE, 600);
    SpellR = GPluginSDK->CreateSpell(kSlotR);

    SSwipe = GPluginSDK->CreateSpell2(kSlotE, kConeCast, false, true, kCollidesWithNothing);
    SSwipe->SetSkillshot(0.25, static_cast<float>(15 * M_PI / 180), 2800, 300);

    SJavelin = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, true, false, QCollisionFlags);
    SJavelin->SetSkillshot(0.25, 40, 1300, 1500);

    SPounce = GPluginSDK->CreateSpell2(kSlotW, kCircleCast, false, true, kCollidesWithNothing);
    SPounce->SetSkillshot(0.25, 175, 1750, 345);

    SBushwhack = GPluginSDK->CreateSpell2(kSlotW, kCircleCast, false, true, kCollidesWithNothing);
    SBushwhack->SetSkillshot(0.25, 100, 2800, 900);

    QCollisionFlags = static_cast<eCollisionFlags>(kCollidesWithHeroes | kCollidesWithMinions | kCollidesWithYasuoWall); }

inline bool ZNidalee::CatForm() {
    return strcmp(Player->GetBaseSkinName(), "Nidalee") != 0; }

inline bool ZNidalee::IsHunted(IUnit * unit) {
    return unit->HasBuff("nidaleepassivehunted"); }

inline bool ZNidalee::CompareDamage(IUnit * a, IUnit * b) {
    return a->PhysicalDamage() + a->PhysicalDamageMod() > b->PhysicalDamage() + b->PhysicalDamageMod(); }

inline bool ZNidalee::CompareLowHealth(IUnit * a, IUnit * b) {
    return a->HealthPercent() < b->HealthPercent(); }

inline bool ZNidalee::CompareHighHealth(IUnit * a, IUnit * b) {
    return a->HealthPercent() > b->HealthPercent(); }

inline bool ZNidalee::CompareMaxHealth(IUnit * a, IUnit * b) {
    return a->GetMaxHealth() > b->GetMaxHealth(); }

inline bool ZNidalee::CompareDistanceToCursor(IUnit * a, IUnit * b) {
    return Ex->Dist2D(a->ServerPosition(), GGame->CursorPosition()) < Ex->Dist2D(b->ServerPosition(), GGame->CursorPosition()); }

inline bool ZNidalee::CanUse(ISpell * spell, bool human, std::string mode, int time = 1) {

    if(Player->GetSpellLevel(spell->GetSpellSlot()) < 1) {
        return false; }

    if(spell->GetSpellSlot() == kSlotQ && human) {
        auto name = std::string("Javelin");
        auto key = name.append(mode);

        if(Menu->HumanQMap.at(key).first->Enabled() &&
                Menu->HumanQMap.at(key).second <= time) {
            return true; } }

    if(spell->GetSpellSlot() == kSlotW && human) {
        auto name = std::string("Bushwhack");
        auto key = name.append(mode);

        if(Menu->HumanWMap.at(key).first->Enabled() &&
                Menu->HumanWMap.at(key).second <= time) {
            return true; } }

    if(spell->GetSpellSlot() == kSlotE && human) {
        auto name = std::string("Primalsurge");
        auto key = name.append(mode);

        if(Menu->HumanEMap.at(key).first->Enabled() &&
                Menu->HumanEMap.at(key).second <= time) {
            return true; } }

    if(spell->GetSpellSlot() == kSlotQ && !human) {
        auto name = std::string("Takedown");
        auto key = name.append(mode);

        if(Menu->CougarQMap.at(key).first->Enabled() &&
                Menu->CougarQMap.at(key).second <= time) {
            return true; } }

    if(spell->GetSpellSlot() == kSlotW && !human) {
        auto name = std::string("Pounce");
        auto key = name.append(mode);

        if(Menu->CougarWMap.at(key).first->Enabled() &&
                Menu->CougarWMap.at(key).second <= time) {
            return true; } }

    if(spell->GetSpellSlot() == kSlotE && !human) {
        auto name = std::string("Swipe");
        auto key = name.append(mode);

        if(Menu->CougarEMap.at(key).first->Enabled() &&
                Menu->CougarEMap.at(key).second <= time) {
            return true; } }

    if(spell->GetSpellSlot() == kSlotR) {
        std::string n = human ? "human" : "cougar";
        auto key = "Aspect" + n.append(mode);

        if(human) {
            if(Menu->HumanRMap.at(key).first->Enabled() &&
                    Menu->HumanRMap.at(key).second <= time) {
                return true; } }
        else {
            if(Menu->CougarRMap.at(key).first->Enabled() &&
                    Menu->CougarRMap.at(key).second <= time) {
                return true; } } }

    return  false; }

inline bool ZNidalee::GetPriorityHealTarget(IUnit * a, IUnit * b) {
    switch(Menu->AllyHealPriroty->GetInteger()) {
        case 0:
            return CompareLowHealth(a, b);

        case 1:
            return CompareDamage(a, b);

        case 2:
            return CompareHighHealth(a, b); }

    return false; }

inline bool ZNidalee::GetPriorityJungleTarget(IUnit * a, IUnit * b) {
    switch(Menu->AllyHealPriroty->GetInteger()) {
        case 0:
            return CompareLowHealth(a, b);

        case 1:
            return CompareMaxHealth(a, b);

        case 2:
            return CompareDistanceToCursor(a, b); }

    return false; }

inline void ZNidalee::CastSpear(IUnit * unit, std::string mode) {
    auto jungle = strcmp("jg", mode.c_str()) == 0;

    if(!CatForm() && CanUse(SpellQ, true, mode)) {
        if(strcmp(mode.c_str(), "ha") == 0 && Player->ManaPercent() <= 65) {
            return; }

        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(Ex->Dist2D(unit) <= SJavelin->Range()) {
            if(unit->IsHero()) {
                if(Menu->CheckSpearHitChance->Enabled()) {
                    switch(Menu->PredictionSelection->GetInteger()) {
                        case 0:
                            auto pred = new AdvPredictionOutput();

                            if(SJavelin->RunPrediction(unit, false, QCollisionFlags, pred)) {
                                if(pred->HitChance >= Menu->CheckSpearHitChance->GetInteger() + 3) {
                                    SJavelin->CastOnPosition(pred->CastPosition); } } } }
                else {
                    SJavelin->CastOnPosition(unit->ServerPosition()); } }
            else {
                SJavelin->CastOnTarget(unit, kHitChanceHigh); } } } }

inline void ZNidalee::PlaceTrap(IUnit * unit, std::string mode) {
    if(!CatForm() && CanUse(SpellW, true, mode)) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(Ex->Dist2D(unit) <= SBushwhack->Range()) {
            if(Menu->BushwhackLocation->GetInteger() == 0) {
                if(unit->IsHero()) {
                    Vec3 pos;
                    GPrediction->GetFutureUnitPosition(unit, 1, true, pos);

                    //  todo: prediction
                    if(Ex->IsValid(pos)) {
                        SBushwhack->CastOnPosition(pos.Extend(Player->ServerPosition(), -75)); } }
                else {
                    if(CanUse(SpellQ, true, mode)) {
                        if(Ex->Dist2D(unit) > 265) {
                            SBushwhack->CastOnPosition(unit->ServerPosition().Extend(Player->ServerPosition(), 305)); }
                        else {} SBushwhack->CastOnPosition(unit->ServerPosition().Extend(Player->ServerPosition(), 100)); } } } } } }


inline void ZNidalee::Bite(IUnit * unit, std::string mode) {
    if(CatForm() && CanUse(SpellQ, false, mode)) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(Ex->Dist2D(unit) <= SpellQ->GetSpellRange() + Player->AttackRange() - 75) {
            SpellQ->CastOnPlayer(); } } }

inline void ZNidalee::Pounce(IUnit * unit, std::string mode) {
    auto jungle = strcmp("jg", mode.c_str()) == 0;
    auto waveclear = strcmp("wc", mode.c_str()) == 0;
    auto combo = strcmp("co", mode.c_str()) == 0;

    if(CatForm() && CanUse(SpellW, false, mode)) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(IsHunted(unit) && Ex->Dist2D(unit) <= SPounce->Range() * 2) {
            if(Menu->IgnorePounceChecksOnHuntedTarget->Enabled() ||
                    Ex->Dist2D(unit) > Player->AttackRange() + 35 || jungle ||
                    waveclear && !Menu->CheckPounceMinRangeWaveClear->Enabled() ||
                    combo && !Menu->CheckPounceMinRangeCombo->Enabled()) {
                SPounce->CastOnPosition(unit->ServerPosition()); }  }
        else {
            if(Ex->Dist2D(unit) <= SPounce->Range()) {
                if(Ex->Dist2D(unit) > Player->AttackRange() + 35 || jungle ||
                        waveclear && !Menu->CheckPounceMinRangeWaveClear->Enabled() ||
                        combo && !Menu->CheckPounceMinRangeCombo->Enabled()) {
                    if(unit->IsHero()) {
                        if(Menu->CheckPounceHitChance->Enabled()) {
                            // todo: prediction
                            SPounce->CastOnPosition(unit->ServerPosition()); }
                        else {
                            SPounce->CastOnPosition(unit->ServerPosition()); } }
                    else {
                        if(waveclear && Menu->CheckPounceIntoEnemiesWaveClear->Enabled() &&
                                Ex->CountInRange(Player, 750, GEntityList->GetAllHeros(false, true)) > 0) {
                            return; }

                        if(waveclear && Menu->CheckPounceIntoTurretWaveClear->Enabled() &&
                                Ex->UnderEnemyTurret(unit)) {
                            return; }

                        SPounce->CastOnPosition(unit->ServerPosition()); } } } }  } }

inline void ZNidalee::Swipe(IUnit * unit, std::string mode) {
    auto jungle = strcmp("jg", mode.c_str()) == 0;
    auto waveclear = strcmp("wc", mode.c_str()) == 0;

    if(CatForm() && CanUse(SpellE, false, mode)) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(Ex->Dist2D(unit) <= SSwipe->Range()) {
            if(unit->IsHero()) {
                if(Menu->CheckSwipeHitchange->Enabled()) {
                    auto pred = new AdvPredictionOutput();

                    if(SSwipe->RunPrediction(unit, true, kCollidesWithNothing, pred)) {
                        if(pred->HitChance >= Menu->CheckSwipeHitChanceMin->GetInteger() + 3) {
                            SSwipe->CastOnPosition(pred->CastPosition); } } }
                else {
                    SSwipe->CastOnPosition(unit->ServerPosition()); } }
            else {
                Vec3 pos;
                int numberHit;

                if(waveclear) {
                    SSwipe->FindBestCastPosition(true, false, pos, numberHit);

                    if(numberHit >= Menu->CheckSwipeMinimumHitWaveClear->GetInteger()) {
                        SSwipe->CastOnPosition(pos); } }
                else {
                    SSwipe->CastOnPosition(unit->ServerPosition()); } } } } }

inline void ZNidalee::SwitchForm(IUnit * unit, std::string mode) {
    auto jungle = strcmp("jg", mode.c_str()) == 0;
    auto waveclear = strcmp("wc", mode.c_str()) == 0;
    auto combo = strcmp("co", mode.c_str()) == 0;

    if(CatForm() && CanUse(SpellR, false, mode)) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(CanUse(SpellQ, false, mode)) {
            if(Player->HasBuff("Takedown") && Ex->Dist2D(unit) <= SPounce->Range()) {
                return; } }

        if(unit->IsHero()) {
            if(CanUse(SpellQ, true, mode)) {
                switch(Menu->PredictionSelection->GetInteger()) {
                    case 0:
                        auto pred = new AdvPredictionOutput();

                        if(SJavelin->RunPrediction(unit, false, QCollisionFlags, pred)) {
                            if(pred->HitChance >= Menu->CheckSpearHitChanceMin->GetInteger() + 3) {
                                SpellR->CastOnPlayer(); } } } }
            else {
                if(!CanUse(SpellW, false, mode) && !CanUse(SpellQ, false, mode)
                        || CanUse(SpellQ, true, mode) && Ex->Dist2D(unit) > SPounce->Range() + 100) {
                    if(Ex->Dist2D(unit) > SpellW->GetSpellRange() + 25) {
                        SpellR->CastOnPlayer(); } } } }

        else {
            if(!CanUse(SpellW, false, mode) && !CanUse(SpellE, false, mode) && !CanUse(SpellQ, false, mode)
                    || CanUse(SpellW, false, mode, 3) && CanUse(SpellQ, true, mode) && (Ex->Dist2D(unit) > SPounce->Range() || jungle)) {
                if(CanUse(SpellQ, true, mode)) {
                    auto pred = new AdvPredictionOutput();

                    if(SJavelin->RunPrediction(unit, false, QCollisionFlags, pred)) {
                        if(pred->HitChance != kHitChanceCollision) {
                            SpellR->CastOnPlayer(); } } }
                else {
                    SpellR->CastOnPlayer(); } } } }

    if(!CatForm() && CanUse(SpellR, true, mode)) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(IsHunted(unit)) {
            if(Menu->IgnorePounceChecksOnHuntedTarget->Enabled() && combo || jungle) {
                if(combo || jungle) {
                    SpellR->CastOnPlayer(); } }

            if(CanUse(SpellW, false, mode) || Ex->Dist2D(unit) <= SpellQ->GetSpellRange() * 2) {
                if(Ex->Dist2D(unit) <= SpellW->GetSpellRange() * 2) {
                    if(CanUse(SpellQ, false, mode) || CanUse(SpellE, false, mode) || !Menu->RequireSwipeTakedownCombo->Enabled()) {
                        SpellR->CastOnPlayer(); } } } }
        else {
            if(waveclear && Menu->CheckCatSwitchNearEnemies->Enabled() &&
                    Ex->CountInRange(Player, 750, GEntityList->GetAllHeros(false, true)) > 0) {
                return; }

            if(waveclear && Menu->CheckCatSwitchNearTurret->Enabled() &&
                    Ex->UnderEnemyTurret(unit)) {
                return; }

            if(Ex->Dist2D(unit) <= SpellW->GetSpellRange() + 35 || !IsHunted(unit) && Menu->CurrentPlayStyle->GetInteger() == 1 &&
                    Ex->CountInRange(Player, SpellE->GetSpellRange() + 35, GEntityList->GetAllHeros(false, true)) > 1) {
                auto pred = new AdvPredictionOutput();

                if(SJavelin->RunPrediction(unit, false, QCollisionFlags, pred)) {
                    if(CanUse(SpellQ, true, mode) && pred->HitChance == kHitChanceCollision) {
                        SpellR->CastOnPlayer(); } }

                if(!CanUse(SpellQ, true, mode) && !jungle ||
                        !CanUse(SpellQ, true, mode) && !CanUse(SpellW, true, mode)) {
                    SpellR->CastOnPlayer(); }  } } }  }

inline void ZNidalee::Flee() {

}
