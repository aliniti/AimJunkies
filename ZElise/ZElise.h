#pragma once
#include "PluginSDK.h"
#include "ZEliseMenu.h"
#include "ZEliseModes.h"
#include "ZEliseExtensions.h"
#include <map>
#include <string>
#include "ZEliseAvoider.h"
#include <map>

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
        static std::map<std::string, ZEliseAvoider *> AvoidList;

        static void OnBoot();
        static void OnShutdown();
        static void HandleAttacks();
        static void HandleSpellTimers();

        static double AADmg(IUnit * unit);
        static double QDmg(IUnit * unit, bool human);
        static double WDmg(IUnit * unit);
        static double CDmg(IUnit * unit);
        static bool CompareDamage(IUnit * a, IUnit * b);
        static bool CompareLowHealth(IUnit * a, IUnit * b);
        static bool CompareHighHealth(IUnit * a, IUnit * b);
        static bool CompareMaxHealth(IUnit * a, IUnit * b);
        static bool CompareDistanceToCursor(IUnit * a, IUnit * b);
        static bool GetPriorityJungleTarget(IUnit * a, IUnit * b);

        static bool SpiderForm();
        static auto DrawDamageOnChampionHPBar(IUnit * Hero, double Damage, const char * Text, Vec4 BarColor) -> void;
        static auto DrawDamageOnChampionHPBar(IUnit * Hero, double Damage, Vec4 BarColor) -> void;
        static bool CheckCocoonCollision(IUnit * unit, std::string mode);
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
    SpellZ = GPluginSDK->CreateSpell(kSlotQ, 425);

    ECollisionFlags = static_cast<eCollisionFlags>(kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall);
    E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, true, false, ECollisionFlags);
    E->SetSkillshot(0.25, 55, 1600, 1100);

    Protobelt = GPluginSDK->CreateItemForId(3152, 300); }

inline void ZElise::OnShutdown() {}

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

// credits: @rembrandt
inline auto ZElise::DrawDamageOnChampionHPBar(IUnit * Hero, double Damage, const char * Text, Vec4 BarColor) -> void {
    Vec2 HPBarPos;

    if(Hero->GetHPBarPosition(HPBarPos)) {
        Vec2 HPBarSize = Vec2(103 * (Damage / Hero->GetMaxHealth()), 8);
        HPBarPos = Vec2(HPBarPos.x + 10, HPBarPos.y += 20);

        Vec2 LinePos1 = Vec2(HPBarPos.x + HPBarSize.x, HPBarPos.y);
        Vec2 LinePos2 = Vec2(HPBarPos.x + HPBarSize.x - 5, HPBarPos.y - 7);

        GRender->DrawFilledBox(HPBarPos, HPBarSize, BarColor);
        GRender->DrawLine(LinePos1, LinePos2, Vec4(255, 255, 255, 255));
        GRender->DrawLine(LinePos1, LinePos1 + Vec2(0, 8), Vec4(255, 255, 255, 255));
        GRender->DrawTextW(LinePos2 - Vec2(13, 13), Vec4(255, 255, 255, 255), Text); } }

// credits: @rembrandt
inline auto ZElise::DrawDamageOnChampionHPBar(IUnit * Hero, double Damage, Vec4 BarColor) -> void {
    Vec2 HPBarPos;

    if(Hero->GetHPBarPosition(HPBarPos)) {
        Vec2 HPBarSize = Vec2(103 * (Damage / Hero->GetMaxHealth()), 8);
        HPBarPos = Vec2(HPBarPos.x + 10, HPBarPos.y += 20);

        Vec2 LinePos1 = Vec2(HPBarPos.x + HPBarSize.x, HPBarPos.y);
        Vec2 LinePos2 = Vec2(HPBarPos.x + HPBarSize.x - 5, HPBarPos.y - 7);

        GRender->DrawFilledBox(HPBarPos, HPBarSize, BarColor); } }

inline bool ZElise::CheckCocoonCollision(IUnit * unit, std::string mode) {

    if(unit == nullptr) {
        return false; }

    if(Menu->HumanEMap.at(std::string("cocoon").append(mode)).second > 0) {
        return false; }

    if(unit->IsHero()) {
        auto pred = new AdvPredictionOutput();

        if(E->RunPrediction(unit, false, ECollisionFlags, pred)) {
            return pred->HitChance == kHitChanceCollision; } }

    return false; }

inline bool ZElise::BurstCheck(IUnit * unit, std::string mode) {

    if(unit == nullptr) {
        return  false; }

    auto i = TimeStamps.at("HumanE") - GGame->Time();
    auto time = i > 0 ? i : 0;

    if(time > Player->GetSpellTotalCooldown(kSlotE) - 1) {
        return true; }

    if(!CanUse(SpellE, true, mode, 1, unit)) {
        return  true; }

    if(CocoonStunned(unit)) {
        return true; }

    return false; }

inline bool ZElise::CanBurst(IUnit * unit, std::string mode) {
    if(unit == nullptr || (!Menu->BlockAutoAttacks->Enabled() || SpiderForm())) {
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

    if(unit == nullptr || strcmp(mode.c_str(), "co") != 0) {
        return; }

    if(BurstCheck(unit, mode) && Menu->UseProtobelt->Enabled()) {
        if(Ex->Dist2D(unit) <= SpellZ->GetSpellRange() && !CheckCocoonCollision(unit, mode)) {
            Protobelt->CastOnPosition(unit->ServerPosition()); } }

    if(Protobelt->IsOwned() && Protobelt->IsReady() && Menu->UseProtobelt->Enabled()) {
        if(unit != nullptr && unit->IsValidTarget()) {
            auto dirPos = Player->ServerPosition() + Player->Direction() * 285;

            if(Ex->Dist2D(unit) > 100 || BurstCheck(unit, mode)) {
                if(Ex->CountInRange(Ex->To2D(dirPos), 300, GEntityList->GetAllHeros(false, true)) > 0) {
                    if(BurstCheck(unit, mode) && Ex->Dist2D(unit) <= 285 && !CheckCocoonCollision(unit, mode)) {
                        Protobelt->CastOnPosition(unit->ServerPosition()); }
                    else {
                        Protobelt->CastOnPosition(unit->ServerPosition()); } } } } } }

inline void ZElise::NeurotoxinQ(IUnit * unit, std::string mode) {
    if(strcmp(mode.c_str(), "ha") != 0 || Player->ManaPercent() >= Menu->NeurotoxinHarassMinMana->GetInteger()) {
        if(strcmp(mode.c_str(), "jg") != 0 || Player->ManaPercent() >= Menu->NeurotoxinJungleMinMana->GetInteger()) {

            if(CanUse(SpellQ, true, mode) && !SpiderForm()) {
                if(unit == nullptr || !unit->IsValidTarget()) {
                    return; }

                if(Ex->Dist2D(unit) <= SpellQ->GetSpellRange()) {
                    SpellQ->CastOnUnit(unit); } } } } }

inline void ZElise::VolatileW(IUnit * unit, std::string mode) {
    if(strcmp(mode.c_str(), "ha") != 0 || Player->ManaPercent() >= Menu->VolotileSpiderHarassMinMana->GetInteger()) {
        if(strcmp(mode.c_str(), "jg") != 0 || Player->ManaPercent() >= Menu->VolotileSpiderJungleMinMana->GetInteger()) {

            if(CanUse(SpellW, true, mode) && !SpiderForm()) {
                if(unit == nullptr || !unit->IsValidTarget()) {
                    return; }

                if(Ex->Dist2D(unit) <= SpellW->GetSpellRange() && !CheckCocoonCollision(unit, mode)) {
                    SpellW->CastOnPosition(unit->ServerPosition()); } } } } }

inline void ZElise::CocoonE(IUnit * unit, std::string mode) {
    auto combo = strcmp(mode.c_str(), "co") == 0;
    auto harass = strcmp(mode.c_str(), "ha") == 0;

    if(!harass || Player->ManaPercent() >= Menu->CocoonHarassMinMana->GetInteger()) {
        if(!strcmp(mode.c_str(), "jg") != 0 || Player->ManaPercent() >= Menu->CocoonJungleMinMana->GetInteger()) {

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

                        E->CastOnTarget(unit, kHitChanceMedium); } } } } } }

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

        if(BurstCheck(unit, mode) && !CheckCocoonCollision(unit, mode)) {
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
                    SpellR->CastOnPlayer(); } }

            if(CanUse(SpellE, false, mode) && Ex->Dist2D(unit) <= SpellE->GetSpellRange()) {
                if(!CanUse(SpellZ, false, mode)
                    || Ex->Dist2D(unit) > SpellZ->GetSpellRange() + Player->AttackRange() + 35
                    || CheckCocoonCollision(unit, mode)) {
                    SpellR->CastOnPlayer(); } } }
        else {
            if(!CanUse(SpellQ, true, mode) && !CanUse(SpellW, true, mode) && !CanUse(SpellE, true, mode)) {
                SpellR->CastOnPlayer(); } } }

    if(SpiderForm() && CanUse(SpellR, false, mode)) {
        if(unit == nullptr || !unit->IsValidTarget()) {
            return; }

        if(unit->IsHero()) {
            if(!CanTransform(unit, false, mode)) {
                return; }

            if(Ex->Dist2D(unit) >= Player->AttackRange()
                || CanTransform(unit, true, mode) && !CheckCocoonCollision(unit, mode)) {
                SpellR->CastOnPlayer(); }  }
        else {
            if(CanUse(SpellQ, true, mode) && CanUse(SpellW, true, mode) && CanUse(SpellE, true, mode)) {
                if(!Player->HasBuff("elisespiderw")) {
                    if(Ex->Dist2D(unit) > SpellZ->GetSpellRange() + 55) {
                        SpellR->CastOnPlayer(); } } }

            if(!CanUse(SpellQ, false, mode) && !CanUse(SpellW, false, mode)) {
                if(!Player->HasBuff("elisespiderw")) {
                    SpellR->CastOnPlayer(); } } } }

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

        if(CheckCocoonCollision(unit, mode)) {
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

inline void ZElise::HandleAttacks() {
    if(SpiderForm()) {
        GOrbwalking->SetAttacksAllowed(true); }
    else {
        auto unit = GTargetSelector->FindTarget(QuickestKill, SpellDamage, E->Range());

        if(GOrbwalking->GetOrbwalkingMode() == kModeCombo) {
            GOrbwalking->SetAttacksAllowed(!(CanBurst(unit, "co"))); }
        else {
            GOrbwalking->SetAttacksAllowed(true); } } }


inline void ZElise::HandleSpellTimers() {
    auto & menu = Menu;
    auto & stamps = TimeStamps;

    // update spell states/timers
    for(auto & HumanQ : menu->HumanQMap) {
        auto map = menu->HumanQMap.find(HumanQ.first);

        if(map != menu->HumanQMap.end()) {
            map->second.second = (stamps.at("HumanQ") - GGame->Time()) > 0 ? (stamps.at("HumanQ") - GGame->Time()) : 0; } }

    for(auto & HumanW : menu->HumanWMap) {
        auto map = menu->HumanWMap.find(HumanW.first);

        if(map != menu->HumanWMap.end()) {
            map->second.second = (stamps.at("HumanW") - GGame->Time()) > 0 ? (stamps.at("HumanW") - GGame->Time()) : 0; } }

    for(auto & HumanE : menu->HumanEMap) {
        auto map = menu->HumanEMap.find(HumanE.first);

        if(map != menu->HumanEMap.end()) {
            map->second.second = (stamps.at("HumanE") - GGame->Time()) > 0 ? (stamps.at("HumanE") - GGame->Time()) : 0; } }

    for(auto & HumanR : menu->HumanRMap) {
        auto map = menu->HumanRMap.find(HumanR.first);

        if(map != menu->HumanRMap.end()) {
            map->second.second = (stamps.at("HumanR") - GGame->Time()) > 0 ? (stamps.at("HumanR") - GGame->Time()) : 0; } }

    for(auto & spiderQ : menu->SpiderQMap) {
        auto map = menu->SpiderQMap.find(spiderQ.first);

        if(map != menu->SpiderQMap.end()) {
            map->second.second = (stamps.at("SpiderQ") - GGame->Time()) > 0 ? (stamps.at("SpiderQ") - GGame->Time()) : 0; } }

    for(auto & spiderW : menu->SpiderWMap) {
        auto map = menu->SpiderWMap.find(spiderW.first);

        if(map != menu->SpiderWMap.end()) {
            map->second.second = (stamps.at("SpiderW") - GGame->Time()) > 0 ? (stamps.at("SpiderW") - GGame->Time()) : 0; } }

    for(auto & spiderE : menu->SpiderEMap) {
        auto map = menu->SpiderEMap.find(spiderE.first);

        if(map != menu->SpiderEMap.end()) {
            map->second.second = (stamps.at("SpiderE") - GGame->Time()) > 0 ? (stamps.at("SpiderE") - GGame->Time()) : 0; } }

    for(auto & spiderR : menu->SpiderRMap) {
        auto map = menu->SpiderRMap.find(spiderR.first);

        if(map != menu->SpiderRMap.end()) {
            map->second.second = (stamps.at("SpiderR") - GGame->Time()) > 0 ? (stamps.at("SpiderR") - GGame->Time()) : 0; } } }

inline double ZElise::AADmg(IUnit * unit) {
    double dmg = 0;

    if(unit == nullptr || !unit->IsValidTarget()) {
        return  dmg; }

    if(SpiderForm() || !SpiderForm() && CanUse(SpellR, true, "co")) {
        dmg += GDamage-> CalcMagicDamage(
                   Player, unit, GDamage->GetAutoAttackDamage(Player, unit, true)
                   + std::vector<int> { 10, 20, 30, 40 } [Player->GetSpellLevel(kSlotR) - 1] + 0.3 * Player->TotalMagicDamage()); }

    return  dmg; }

inline double ZElise::QDmg(IUnit * unit, bool human) {
    double dmg = 0;

    if(unit == nullptr
        || !unit->IsValidTarget()
        || !CanUse(SpellQ, human, "co")) {
        return dmg; }

    if(human) {
        dmg += GDamage->CalcMagicDamage(
                   Player, unit, std::vector<int> {40, 75, 110, 145, 180 } [Player->GetSpellLevel(kSlotQ) - 1]
                   + (0.08 + 0.03 / 100 * Player->TotalMagicDamage()) * unit->GetHealth()); }
    else {

        auto predictedDmg = QDmg(unit, true);

        dmg += GDamage->CalcMagicDamage(
                   Player, unit, std::vector<int> {60, 100, 140, 180, 220 } [Player->GetSpellLevel(kSlotQ) - 1]
                   + (0.08 + 0.03 / 100 * Player->TotalMagicDamage()) * (unit->GetMaxHealth() - predictedDmg)); }

    return dmg; }

inline double ZElise::WDmg(IUnit * unit) {
    double dmg = 0;

    if(unit == nullptr
        || !unit->IsValidTarget()) {
        return  dmg; }

    if(CanUse(SpellW, false, "co")) {
        dmg += GDamage->CalcMagicDamage(
                   Player, unit, std::vector<int> {75, 125, 175, 225, 275 } [Player->GetSpellLevel(kSlotW) - 1]
                   + (0.8 * Player->TotalMagicDamage())); }

    return dmg; }

inline double ZElise::CDmg(IUnit * unit) {
    double dmg = 0;
    int aa = 2;

    if(unit == nullptr
        || !unit->IsValidTarget()) {
        return  dmg; }

    if(CanUse(SpellW, false, "co") || Player->HasBuff("elisespiderw")) {
        aa = aa + std::vector<int> { 2, 3, 4, 5 } [min(Player->GetLevel(), 18) / 6]; }

    return (AADmg(unit) * aa) + QDmg(unit, true) + QDmg(unit, false) + WDmg(unit); }


