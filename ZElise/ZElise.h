#pragma once
#include "PluginSDK.h"
#include "ZEliseMenu.h"
#include "ZEliseModes.h"
#include "ZEliseExtensions.h"
#include <map>
#include <string>

class ZElise {
    public:
        static IUnit * Player;
        static ZEliseMenu * Menu;
        static ZEliseModes * Modes;
        static ZEliseExtensions * Ex;
        static DelayAction * Delay;

        static ISpell * SpellQ;
        static ISpell * SpellW;
        static ISpell * SpellE;
        static ISpell * SpellR;
        static ISpell * SpellZ; // spider q

        static IInventoryItem * Protobelt;

        static ISpell2 * E;
        static eCollisionFlags ECollisionFlags;
        static std::map<std::string, int> TimeStamps;

        static void OnBoot();
        static void OnShutdown();
        static void HandleAttacks();

        static bool CompareDamage(IUnit * a, IUnit * b);
        static bool CompareLowHealth(IUnit * a, IUnit * b);
        static bool CompareHighHealth(IUnit * a, IUnit * b);
        static bool CompareMaxHealth(IUnit * a, IUnit * b);
        static bool CompareDistanceToCursor(IUnit * a, IUnit * b);
        static bool GetPriorityJungleTarget(IUnit * a, IUnit * b);

        static bool SpiderForm();
        static bool CheckCocoonCollision(IUnit * unit);
        static bool BurstCheck(IUnit * unit, std::string mode);
        static bool CanBurst(IUnit * unit, std::string mode);

        static bool CocoonStunned(IUnit * unit);
        static bool CanTransform(IUnit * unit, bool human, std::string mode);
        static bool CanUse(ISpell * spell, bool human, std::string mode, int time = 1, IUnit * unit = nullptr);
        static void UseProtobelt(IUnit * unit, std::string mode);

        static void NeurotoxinQ(IUnit * unit, std::string mode);
        static void VolatileW(IUnit * unit, std::string mode);
        static void CocoonE(IUnit * unit, std::string mode);
        static void BiteQ(IUnit * unit, std::string mode);
        static void FrenzyW(IUnit * unit, std::string mode);
        static void RappelE(IUnit * unit, std::string mode);
        static void SwitchForm(IUnit * unit, std::string mode); };

inline void ZElise::OnBoot() {
    TimeStamps.insert(std::make_pair("HumanQ", 0));
    TimeStamps.insert(std::make_pair("HumanW", 0));
    TimeStamps.insert(std::make_pair("HumanE", 0));
    TimeStamps.insert(std::make_pair("HumanR", 0));
    TimeStamps.insert(std::make_pair("SpiderQ", 0));
    TimeStamps.insert(std::make_pair("SpiderW", 0));
    TimeStamps.insert(std::make_pair("SpiderE", 0));
    TimeStamps.insert(std::make_pair("SpiderR", 0));

    SpellQ = GPluginSDK->CreateSpell(kSlotQ, 625);
    SpellW = GPluginSDK->CreateSpell(kSlotW, 950);
    SpellE = GPluginSDK->CreateSpell(kSlotE, 750);
    SpellR = GPluginSDK->CreateSpell(kSlotR);
    SpellZ = GPluginSDK->CreateSpell(kSlotQ, 475);

    ECollisionFlags = static_cast<eCollisionFlags>(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall);
    E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, true, false, ECollisionFlags);
    E->SetSkillshot(0.25, 55, 1600, 1100);

    Protobelt = GPluginSDK->CreateItemForId(3152, 300); }

inline void ZElise::OnShutdown() {}

inline void ZElise::HandleAttacks() {
    if(SpiderForm()) {
        GOrbwalking->SetAttacksAllowed(true); }
    else {
        auto unit = GTargetSelector->FindTarget(QuickestKill, SpellDamage, E->Range());

        if(GOrbwalking->GetOrbwalkingMode() == kModeCombo) {
            GOrbwalking->SetAttacksAllowed(!(CanBurst(unit, "co"))); }
        else {
            GOrbwalking->SetAttacksAllowed(true); } } }

inline bool ZElise::CompareDamage(IUnit * a, IUnit * b) {
    return a->PhysicalDamage() + a->PhysicalDamageMod() > b->PhysicalDamage() + b->PhysicalDamageMod(); }

inline bool ZElise::CompareLowHealth(IUnit * a, IUnit * b) {
    return a->HealthPercent() < b->HealthPercent(); }

inline bool ZElise::CompareHighHealth(IUnit * a, IUnit * b) {
    return a->HealthPercent() > b->HealthPercent(); }

inline bool ZElise::CompareMaxHealth(IUnit * a, IUnit * b) {
    return a->GetMaxHealth() > b->GetMaxHealth(); }

inline bool ZElise::CompareDistanceToCursor(IUnit * a, IUnit * b) {
    return Ex->Dist2D(a->ServerPosition(), GGame->CursorPosition()) < Ex->Dist2D(b->ServerPosition(), GGame->CursorPosition()); }

inline bool ZElise::GetPriorityJungleTarget(IUnit * a, IUnit * b) {
    switch(Menu->JungleOrderPriority->GetInteger()) {
        case 0:
            return CompareLowHealth(a, b);

        case 1:
            return CompareMaxHealth(a, b);

        case 2:
            return CompareDistanceToCursor(a, b); }

    return false; }

inline bool ZElise::SpiderForm() {
    return strcmp(Player->GetBaseSkinName(), "Elise") != 0; }

inline bool ZElise::CheckCocoonCollision(IUnit * unit) {

    if(unit == nullptr) {
        return false; }

    if(unit->IsHero()) {
        auto pred = new AdvPredictionOutput();

        if(E->RunPrediction(unit, false, ECollisionFlags, pred)) {
            return true; } }
    else {
        auto pred = new AdvPredictionOutput();

        if(E->RunPrediction(unit, false, ECollisionFlags, pred)) {
            return true; } }

    return false; }

inline bool ZElise::BurstCheck(IUnit * unit, std::string mode) {

    if(unit == nullptr) {
        return  false; }

    auto i = TimeStamps.at("HumanE") - GGame->Time();
    auto time = i > 0 ? i : 0;

    if(time > Player->GetSpellTotalCooldown(kSlotE) - 1) {
        return true; }

    if(!CanUse(SpellE, true, mode, 1, unit)) {
        if(time < Player->GetSpellTotalCooldown(kSlotE) / 2) {
            return  true; } }

    if(CocoonStunned(unit)) {
        return true; }

    return false; }

inline bool ZElise::CanBurst(IUnit * unit, std::string mode) {
    if(!Menu->BlockAutoAttacks->Enabled() || SpiderForm() || unit == nullptr) {
        return  false; }

    if(Ex->Dist2D(unit) > SpellQ->GetSpellRange()) {
        return false; }

    if(CanUse(SpellE, true, mode, 1, unit) && CanUse(SpellQ, true, mode, 1, unit) && CanUse(SpellW, true, mode)) {
        return true; }

    if(CanUse(SpellE, true, mode, 1, unit) && CanUse(SpellQ, true, mode, 1, unit)) {
        return true; }

    return false; }

inline bool ZElise::CocoonStunned(IUnit * unit) {
    return unit != nullptr && unit->HasBuff("elisehumane"); }

inline void ZElise::UseProtobelt(IUnit * unit, std::string mode) {

    if(unit == nullptr) {
        return; }

    if(BurstCheck(unit, mode) && Menu->UseProtobelt->Enabled()) {
        Protobelt->CastOnPosition(unit->ServerPosition()); }

    if(Protobelt->IsOwned() && Protobelt->IsReady() && Menu->UseProtobelt->Enabled()) {
        if(unit != nullptr && unit->IsValidTarget()) {
            if(!unit->IsFacing(Player) && Player->IsFacing(unit)) {
                if(Ex->Dist2D(unit) > 200) {
                    auto dirPos = Ex->To2D(Player->ServerPosition()) + Ex->Perp(Ex->To2D(Player->Direction())) * 300;

                    if(Ex->CountInRange(dirPos, 355, GEntityList->GetAllHeros(false, true)) > 0) {
                        Protobelt->CastOnPosition(unit->ServerPosition()); } } }

            if(!Player->IsFacing(unit)) {
                auto dirPos = Ex->To2D(Player->ServerPosition()) + Ex->Perp(Ex->To2D(Player->Direction())) * 300;

                if(Ex->CountInRange(dirPos, 355, GEntityList->GetAllHeros(false, true)) < 1) {
                    Protobelt->CastOnPosition(unit->ServerPosition()); } } } } }

inline void ZElise::NeurotoxinQ(IUnit * unit, std::string mode) {
    if(CanUse(SpellQ, true, mode) && !SpiderForm()) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(Ex->Dist2D(unit) <= SpellQ->GetSpellRange()) {
            SpellQ->CastOnUnit(unit); } } }

inline void ZElise::VolatileW(IUnit * unit, std::string mode) {
    if(CanUse(SpellW, true, mode) && !SpiderForm()) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(Ex->Dist2D(unit) <= SpellW->GetSpellRange()) {
            SpellW->CastOnPosition(unit->ServerPosition()); } } }

inline void ZElise::CocoonE(IUnit * unit, std::string mode) {
    auto combo = strcmp(mode.c_str(), "co") == 0;
    auto harass = strcmp(mode.c_str(), "ha") == 0;

    if(CanUse(SpellE, true, mode) && !SpiderForm()) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(Ex->Dist2D(unit) <= E->Range()) {

            if(unit->IsHero()) {

                auto pred = new AdvPredictionOutput();

                if(E->RunPrediction(unit, false, ECollisionFlags, pred)) {
                    if(combo && pred->HitChance >= Menu->CocoonComboHitChance->GetInteger() + 3) {
                        E->CastOnPosition(pred->CastPosition); }

                    if(harass && pred->HitChance >= Menu->CocoonHarassHitChance->GetInteger() + 3) {
                        E->CastOnPosition(pred->CastPosition); } } }

            else {

                if(Menu->DontCocoonJungleNearEnemies->Enabled()) {
                    if(Ex->CountInRange(Player, 1000, GEntityList->GetAllHeros(false, true)) > 0) {
                        return; } }

                E->CastOnTarget(unit, kHitChanceMedium); }

        } } }

inline void ZElise::BiteQ(IUnit * unit, std::string mode) {

    if(SpiderForm() && CanUse(SpellZ, false, mode)) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(Ex->Dist2D(unit) <= SpellZ->GetSpellRange()) {
            SpellZ->CastOnUnit(unit); } } }

inline void ZElise::FrenzyW(IUnit * unit, std::string mode) {

    if(SpiderForm() && CanUse(SpellW, false, mode)) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(CanUse(SpellQ, false, mode) && Ex->Dist2D(unit) <= SpellZ->GetSpellRange() + 55) {
            SpellW->CastOnPlayer(); }
        else
            if(Ex->Dist2D(unit) <= Player->AttackRange() + 55) {
                SpellW->CastOnPlayer(); } } }

inline void ZElise::RappelE(IUnit * unit, std::string mode) {

    if(SpiderForm() && CanUse(SpellE, false, mode)) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(BurstCheck(unit, mode)) {
            if(Ex->Dist2D(unit) <= SpellQ->GetSpellRange() + 100 && CanUse(SpellQ, false, mode)) {
                return; } }

        if(Ex->Dist2D(unit) <= SpellE->GetSpellRange()) {
            if(CanUse(SpellQ, false, mode) && Ex->Dist2D(unit) >= SpellZ->GetSpellRange() + 55) {
                SpellE->CastOnUnit(unit); }
            else
                if(Ex->Dist2D(unit) >= Player->AttackRange() + 155) {
                    SpellE->CastOnUnit(unit); } } } }

inline void ZElise::SwitchForm(IUnit * unit, std::string mode) {

    if(!SpiderForm() && CanUse(SpellR, true, mode)) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(unit->IsHero()) {
            if(!CanTransform(unit, true, mode)) {
                return; }

            if(!CanUse(SpellQ, true, mode)) {
                if(CanUse(SpellZ, false, mode) && Ex->Dist2D(unit) <= SpellZ->GetSpellRange()) {
                    SpellR->CastOnPlayer(); }

                if(CanUse(SpellE, false, mode) && Ex->Dist2D(unit) <= SpellE->GetSpellRange()) {
                    if(!CanUse(SpellZ, false, mode) || Ex->Dist2D(unit) > SpellZ->GetSpellRange() + Player->AttackRange() + 35) {
                        SpellR->CastOnPlayer(); } } } } }

    if(SpiderForm() && CanUse(SpellR, false, mode)) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(unit->IsHero()) {
            if(!CanTransform(unit, false, mode)) {
                return; }

            if(Ex->Dist2D(unit) >= Player->AttackRange()) {
                SpellR->CastOnPlayer(); }  }
        else {
            if(!CanUse(SpellQ, false, mode) && !CanUse(SpellW, false, mode)) {
                if(!Player->HasBuff("elisespiderw")) {
                    SpellR->CastOnPlayer(); } } }

    }

}
inline bool ZElise::CanTransform(IUnit * unit, bool human, std::string mode) {

    if(unit == nullptr) {
        return false; }

    if(BurstCheck(unit, mode)) {
        return true; }

    if(strcmp(mode.c_str(), "co") && human) {
        switch(Menu->HumanToSpiderComboRequiredSkills->GetInteger()) {
            case 0:
                if(!CanUse(SpellQ, false, mode) && !CanUse(SpellW, false, mode) && !CanUse(SpellE, false, mode)) {
                    return false; }

                break;

            case 1:
                if(!CanUse(SpellQ, false, mode) || !CanUse(SpellW, false, mode)) {
                    return false; }

                break;

            case 2:
                if(!CanUse(SpellQ, false, mode) || !CanUse(SpellE, false, mode)) {
                    return  false; }

                break; } }

    if(strcmp(mode.c_str(), "ha") && human) {
        switch(Menu->HumanToSpiderHarassRequiredSkills->GetInteger()) {
            case 0:
                if(!CanUse(SpellQ, true, mode) && !CanUse(SpellW, true, mode) && !CanUse(SpellE, true, mode)) {
                    return false; }

                break;

            case 1:
                if(!CanUse(SpellQ, false, mode) || !CanUse(SpellW, false, mode)) {
                    return false; }

                break;

            case 2:
                if(!CanUse(SpellQ, false, mode) || !CanUse(SpellE, true, mode)) {
                    return  false; }

                break; } }

    if(strcmp(mode.c_str(), "co") && !human) {
        switch(Menu->SpiderToHumanComboRequiredSkills->GetInteger()) {
            case 0:
                if(!CanUse(SpellQ, true, mode) && !CanUse(SpellW, true, mode) && !CanUse(SpellE, true, mode)) {
                    return false; }

                break;

            case 1:
                if(!CanUse(SpellQ, true, mode) || !CanUse(SpellW, true, mode)) {
                    return false; }

                break;

            case 2:
                if(!CanUse(SpellQ, true, mode) || !CanUse(SpellE, true, mode)) {
                    return  false; }

                break; } }

    if(strcmp(mode.c_str(), "ha") && !human) {
        switch(Menu->SpiderToHumanHarassRequiredSkills->GetInteger()) {
            case 0:
                if(!CanUse(SpellQ, true, mode) && !CanUse(SpellW, true, mode) && !CanUse(SpellE, true, mode)) {
                    return false; }

                break;

            case 1:
                if(!CanUse(SpellQ, true, mode) || !CanUse(SpellW, true, mode)) {
                    return false; }

                break;

            case 2:
                if(!CanUse(SpellQ, true, mode) || !CanUse(SpellE, true, mode)) {
                    return  false; }

                break; } }

    return  true; }

inline bool ZElise::CanUse(ISpell * spell, bool human, std::string mode, int time, IUnit * unit) {

    if(Player->GetSpellLevel(spell->GetSpellSlot()) < 1) {
        return false; }

    if(spell->GetSpellSlot() == kSlotQ && human) {
        auto name = std::string("neurotoxin");
        auto key = name.append(mode);

        if(Menu->HumanQMap.at(key).first->Enabled() &&
                Menu->HumanQMap.at(key).second <= time) {
            return true; } }

    if(spell->GetSpellSlot() == kSlotW && human) {
        auto name = std::string("volatilespider");
        auto key = name.append(mode);

        if(Menu->HumanWMap.at(key).first->Enabled() &&
                Menu->HumanWMap.at(key).second <= time) {
            return true; } }

    if(spell->GetSpellSlot() == kSlotE && human) {
        auto name = std::string("cocoon");
        auto key = name.append(mode);

        if(CheckCocoonCollision(unit)) {
            return false; }

        if(Menu->HumanEMap.at(key).first->Enabled() &&
                Menu->HumanEMap.at(key).second <= time) {
            return true; } }

    if(spell->GetSpellSlot() == kSlotQ && !human) {
        auto name = std::string("venomousbite");
        auto key = name.append(mode);

        if(Menu->SpiderQMap.at(key).first->Enabled() &&
                Menu->SpiderQMap.at(key).second <= time) {
            return true; } }

    if(spell->GetSpellSlot() == kSlotW && !human) {
        auto name = std::string("frenzy");
        auto key = name.append(mode);

        if(Menu->SpiderWMap.at(key).first->Enabled() &&
                Menu->SpiderWMap.at(key).second <= time) {
            return true; } }

    if(spell->GetSpellSlot() == kSlotE && !human) {
        auto name = std::string("rapell");
        auto key = name.append(mode);

        if(Menu->SpiderEMap.at(key).first->Enabled() &&
                Menu->SpiderEMap.at(key).second <= time) {
            return true; } }

    if(spell->GetSpellSlot() == kSlotR) {
        std::string n = human ? "human" : "spider";
        auto key = "transform" + n.append(mode);

        if(human) {
            if(Menu->HumanRMap.at(key).first->Enabled() &&
                    Menu->HumanRMap.at(key).second <= time) {
                return true; } }
        else {
            if(Menu->SpiderRMap.at(key).first->Enabled() &&
                    Menu->SpiderRMap.at(key).second <= time) {
                return true; } } }

    return  false; }
