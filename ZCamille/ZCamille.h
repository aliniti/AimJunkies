#pragma once
#include "ZCamilleModes.h"
#include "ZCamilleMenu.h"
#include "ZCamilleAvoider.h"
#include "ZCamilleExtensions.h"

#include "PluginData.h"
#include "PluginSDK.h"

#include <math.h>
#include <map>
#include <string>

#define M_PI 3.14159265358979323846

class ZCamille {
    public:
        ~ZCamille();

        static void CreateItems();
        static void CreateSpells();

        static IUnit * Player;
        static ZCamilleMenu * Menu;
        static ZCamilleModes * Modes;
        static ZCamilleExtensions * Ex;
        static IInventoryItem * Youmuus;
        static IInventoryItem * Tiamat;
        static IInventoryItem * Hydra;
        static IInventoryItem * Titanic;
        static ISpell2 * Q, *W, *E, *R;

        static DelayAction Delay;
        static std::map<std::string, ZCamilleAvoider *> DangerSpells;
        static std::map<float, ZCamilleAvoider *> DangerPoints;

        // aura mechanics
        static bool HasQ();
        static bool HasQ2();
        static bool ChargingW();;
        static bool OnWall();
        static bool IsDashing();
        static bool KnockedBack(IUnit * unit);
        static bool LethalTarget(IUnit * unit);

        // tick counts
        static int LastQ;
        static int LastW;
        static int LastE;
        static int LastR;

        // ability methods
        static void UseQ(IUnit * unit);
        static void UseW(IUnit * unit);
        static bool CanW(IUnit * unit);
        static void LockW(IUnit * unit);
        static void LockW(Vec3 p);
        static void UseE(Vec3 pos, bool combo);
        static void UseR(IUnit * unit, bool force);

        // damage
        static double CDmg(IUnit * unit);
        static double QDmg(IUnit * unit, bool includeq2 = true);
        static double WDmg(IUnit * unit, bool bonus = false);
        static double EDmg(IUnit * unit);
        static double RDmg(double dmg, IUnit * unit);

        // combo damage dificulties
        static double SuperRotation(IUnit * unit);
        static double HardRotation(IUnit * unit);
        static double StandardRotation(IUnit * unit);
        static double EasyRotation(IUnit * unit); };

inline void ZCamille::CreateItems() {
    Player = GEntityList->Player();
    Tiamat = GPluginSDK->CreateItemForId(3077, 400);
    Hydra = GPluginSDK->CreateItemForId(3074, 400);
    Titanic = GPluginSDK->CreateItemForId(3748, 400);
    Youmuus = GPluginSDK->CreateItemForId(3142, 900); }

inline void ZCamille::CreateSpells() {
    Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, false, kCollidesWithNothing);
    Q->SetOverrideRange(175);
    W = GPluginSDK->CreateSpell2(kSlotW, kConeCast, false, true, kCollidesWithNothing);
    W->SetOverrideRange(625);
    E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, false, false, kCollidesWithNothing); // not using collision;
    E->SetSkillshot(0.125, Player->BoundingRadius(), 1750, 975);
    R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, false, false, kCollidesWithNothing);
    R->SetOverrideRange(465); }

inline bool ZCamille::HasQ() {
    return Player->HasBuff("camilleqprimingstart"); }

inline bool ZCamille::HasQ2() {
    return Player->HasBuff("camilleqprimingcomplete"); }

inline bool ZCamille::ChargingW() {
    return Player->HasBuff("camillewconeslashcharge"); }

inline bool ZCamille::OnWall() {
    return Player->HasBuff("camilleedashtoggle") || strcmp("CamilleE", Player->GetSpellName(kSlotE)) != 0; }

inline bool ZCamille::IsDashing() {
    return Player->HasBuff("camilleedash1") || Player->HasBuff("camilleedash2") || Player->IsDashing(); }

inline bool ZCamille::KnockedBack(IUnit * unit) {
    return unit != nullptr && unit->HasBuff("camilleeknockback2"); }

inline bool ZCamille::LethalTarget(IUnit * unit) {
    return CDmg(unit) / 1.65 >= unit->GetHealth(); }

inline void ZCamille::UseQ(IUnit * unit) {
    if (Q->IsReady()) {
        if (!HasQ() || HasQ2()) {
            if (Q->CastOnPlayer()) {
                GOrbwalking->ResetAA(); } }

        else {
            if (unit != nullptr && unit->IsHero()) {
                if (QDmg(unit, false) + GDamage->GetAutoAttackDamage(Player, unit, true) * 1 >= unit->GetHealth()) {
                    if (Q->CastOnPlayer()) {
                        GOrbwalking->ResetAA(); } } } } } }

inline void ZCamille::UseW(IUnit * unit) {
    if (!ChargingW() && !IsDashing() && !OnWall() && CanW(unit)) {
        if (KnockedBack(unit)) {
            return; }

        if (W->IsReady() && Ex->Dist2D(unit) <= W->Range()) {
            W->CastOnPosition(unit->ServerPosition()); } } }

inline bool ZCamille::CanW(IUnit * unit) {
    std::vector<void *> buffPtr;
    Player->GetAllBuffsData(buffPtr);
    const float animationtime = 2000;

    if (OnWall() || IsDashing() || unit == nullptr) {
        return false; }

    if (GGame->TickCount() - LastE < 500) {
        // to prevent instant w after e
        return false; }

    if (Q->IsReady()) {
        for (auto i : buffPtr) {
            if (i != nullptr) {
                if (strcmp("camilleqprimingstart", GBuffData->GetBuffName(i))) {
                    if ((GBuffData->GetEndTime(i) - GGame->Time()) * 1000 <= animationtime) {
                        return false; } }

                if (strcmp("camilleqprimingcomplete", GBuffData->GetBuffName(i))) {
                    if ((GBuffData->GetEndTime(i) - GGame->Time()) * 1000 <= animationtime) {
                        return false; } } } }

        if (!HasQ() || HasQ2()) {
            if (Ex->Dist(unit) <=  Player->AttackRange() + 35) {
                return false; } }

        else {
            if (QDmg(unit, false) + GDamage->GetAutoAttackDamage(Player, unit, true) * 1 >= unit->GetHealth()) {
                return  false; } } }

    if (Ex->Dist2D(unit) <= Player->AttackRange() + 35) {
        if (GDamage->GetAutoAttackDamage(Player, unit, true) * 3 > unit->GetHealth()) {
            return false; } }

    return true; }

inline void ZCamille::LockW(IUnit * unit) {
    if (OnWall() || IsDashing() || unit == nullptr) {
        return; }

    if (ChargingW() && Ex->Dist2D(unit) <= W->Range() + unit->BoundingRadius() + Player->BoundingRadius()) {
        Vec3 pos;
        GPrediction->GetFutureUnitPosition(unit, 100, true, pos);

        if (Ex->IsValid(pos)) {
            GGame->IssueOrder(Player, kMoveTo, pos.Extend(Player->ServerPosition(), W->Range() - 65)); } } }

inline void ZCamille::LockW(Vec3 p) {
    if (OnWall() || IsDashing() || !Ex->IsValid(p)) {
        return; }

    if (ChargingW() && Ex->Dist2D(Player->ServerPosition(), p) <= W->Range() + Player->BoundingRadius() * 2) {
        GGame->IssueOrder(Player, kMoveTo, p.Extend(Player->ServerPosition(), W->Range() - 25)); } }

inline void ZCamille::UseE(Vec3 pos, bool combo = true) {
    if (IsDashing() || OnWall() || !E->IsReady()) {
        return; }

    if (combo) {
        if (Ex->Dist2D(Player->ServerPosition(), pos) - Player->BoundingRadius() * 2 < Menu->MinimumERange->GetInteger()) {
            return; }

        if (Ex->UnderEnemyTurret(pos) && Menu->DontEUnderTurret->Enabled()) {
            return; } }

    auto posChecked = 0;
    auto maxPosChecked = 40;
    auto posRadius = 145;
    auto radiusIndex = 0;

    if (Menu->EnhancedEPrecision->Enabled()) {
        maxPosChecked = 80;
        posRadius = 65; }

    auto candidatePosList = std::vector<Vec2>();

    while (posChecked < maxPosChecked) {
        radiusIndex++;
        auto curRadius = radiusIndex * (0x2 * posRadius);
        auto curCurcleChecks = static_cast<int>(ceil((2 * M_PI * curRadius) / (2 * static_cast<double>(posRadius))));

        for (auto i = 1; i < curCurcleChecks; i++) {
            posChecked++;
            auto cRadians = (2 * M_PI / (curCurcleChecks - 1)) * i;
            auto xPos = static_cast<float>(floor(pos.x + curRadius * cos(cRadians)));
            auto zPos = static_cast<float>(floor(pos.z + curRadius * sin(cRadians)));
            auto posFor2D = Vec2(xPos, zPos);
            auto anyDangerousPos = false;

            for (auto entry : DangerPoints) {
                auto obj = entry.second;

                if (obj->aType == Outside && Ex->Dist2D(posFor2D, obj->Emitter->GetPosition()) > obj->Radius) {
                    anyDangerousPos = true;
                    break; }

                if (obj->aType == Inside) {
                    auto startPos = posFor2D;
                    auto endPos = Ex->To2D(pos);
                    auto initPos = Ex->To2D(entry.second->Emitter->GetPosition());
                    auto proj = Ex->ProjectOn(initPos, startPos, endPos);

                    if (proj->IsOnSegment && Ex->Dist2D(proj->SegmentPoint, initPos) <= obj->Radius + Player->BoundingRadius()) {
                        anyDangerousPos = true;
                        break; } } }

            if (anyDangerousPos) {
                continue; }

            auto wtarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());

            if (wtarget != nullptr && wtarget->IsValidTarget() && ChargingW()) {
                if (Ex->Dist2D(Ex->To2D(wtarget->ServerPosition()), posFor2D) > W->Range() - 100) {
                    continue; } }

            if (GNavMesh->IsPointWall(Ex->To3D(posFor2D))) {
                candidatePosList.push_back(posFor2D); } } }

    if (ChargingW() == false) {
        for (auto vec : candidatePosList) {
            if (Ex->IsValid(vec) != false) {
                if (Ex->Dist2D(Ex->To3D(vec), Player->ServerPosition()) <= E->Range() && Ex->Dist2D(Ex->To3D(vec), pos) <= E->Range()) {
                    if (Ex->IsValid(vec)) {
                        //if (W->IsReady() && Menu->ExpirimentalCombo->Enabled() && combo) {
                        //    int dashSpeedEst = 1450;
                        //    int hookSpeedEst = 1250;
                        //    float e1Time = 1000 * (Ex->Dist2D(vec, Player->ServerPosition()) - Player->BoundingRadius() / hookSpeedEst);
                        //    float meToWall = e1Time + (1000 * (Ex->Dist2D(vec, Player->ServerPosition()) - Player->BoundingRadius() / dashSpeedEst));
                        //    float wallToHero = (1000 * ((Ex->Dist2D(vec, pos) - Player->BoundingRadius() / dashSpeedEst)));
                        //    auto travelTime = 250 + meToWall + wallToHero;

                        //    if (travelTime >= 1250 && travelTime <= 1750) {
                        //        W->CastOnPosition(pos); }

                        //    if (travelTime > 1750) {
                        //        auto delay = 100 + (travelTime - 1750);
                        //        Delay.Add(static_cast<int>(std::round(delay)), [&]() { W->CastOnPosition(pos); }); } }
                        if (E->CastOnPosition(Ex->To3D(vec))) {
                            LastE = GGame->TickCount(); } } } } } } }

inline void ZCamille::UseR(IUnit * unit, bool force = false) {
    if (R->IsReady() && force) {
        R->CastOnUnit(unit); }

    if (R->IsReady() && Ex->Dist2D(unit) <= R->Range()) {
        if (Menu->UltOnlySelected->Enabled()) {
            auto selected = GTargetSelector->GetFocusedTarget();

            if (selected == nullptr || selected->GetNetworkId() != unit->GetNetworkId()) {
                return; } }

        if (QDmg(unit) + GDamage->GetAutoAttackDamage(Player, unit, true) * 2 >= unit->GetHealth()) {
            if (Ex->Dist2D(unit) <= unit->BoundingRadius() + Player->AttackRange()) {
                return; } }

        if (R->IsReady() && CDmg(unit) >= unit->GetHealth()) {
            R->CastOnUnit(unit); } } }

inline double ZCamille::CDmg(IUnit * unit) {
    if (unit == nullptr) {
        return 0; }

    switch (Menu->RLiability->GetInteger()) {
        case 1:
            return EasyRotation(unit);

        case 2:
            return StandardRotation(unit);

        case 3:
            return HardRotation(unit);

        case 4:
            return SuperRotation(unit); } }


inline double ZCamille::QDmg(IUnit * unit, bool bonus) {
    double dmg = 0;

    if (Q->IsReady() && unit != nullptr) {
        double aa[] = { 0.2, 0.25, 0.30, 0.35, 0.40 };
        double bb[] = { 0.4, 0.5, 0.6, 0.7, 0.8 };
        dmg += GDamage->CalcPhysicalDamage(Player, unit, aa[Player->GetSpellLevel(kSlotQ) - 1] * (Player->PhysicalDamage() + Player->PhysicalDamageMod()));
        auto dmgreg = GDamage->CalcPhysicalDamage(Player, unit, bb[Player->GetSpellLevel(kSlotQ) - 1] * (Player->PhysicalDamage() + Player->PhysicalDamageMod()));
        auto pct = 52 + (3 * min(16, Player->GetLevel()));
        auto dmgtrue = dmgreg * (pct / 100);

        if (bonus) {
            dmg += dmgtrue; } }

    return dmg; }

inline double ZCamille::WDmg(IUnit * unit, bool bonus) {
    double dmg = 0;

    if (W->IsReady() && unit != nullptr) {
        int aa[] = { 65, 95, 125, 155, 185 };
        dmg += GDamage->CalcPhysicalDamage(Player, unit, aa[Player->GetSpellLevel(kSlotW) - 1] + (0.6 * Player->PhysicalDamageMod()));
        double bb[] = { 6, 6.5, 7, 7.5, 8 };
        auto pct = bb[Player->GetSpellLevel(kSlotW) - 1];

        if (Player->PhysicalDamageMod() >= 100) {
            pct += min(300, Player->PhysicalDamageMod() * 3 / 100); }

        if (bonus && Ex->Dist2D(unit) > 400) {
            dmg += GDamage->CalcPhysicalDamage(Player, unit, pct * (unit->GetMaxHealth() / 100)); } }

    return dmg; }

inline double ZCamille::EDmg(IUnit * unit) {
    double dmg = 0;

    if (E->IsReady() && unit != nullptr) {
        int aa[] = { 70, 115, 160, 205, 250 };
        dmg += GDamage->CalcPhysicalDamage(Player, unit, aa[Player->GetSpellLevel(kSlotE) - 1] + (0.75 * Player->PhysicalDamageMod())); }

    return dmg; }

inline double ZCamille::RDmg(double dmg, IUnit * unit) {
    if (unit == nullptr) {
        return 0; }

    if (R->IsReady() || unit->HasBuff("camillertether")) {
        int aa[] = { 4, 6, 8, 8 };
        int bb[] = { 5, 10, 15, 15 };;
        auto xtra = bb[Player->GetSpellLevel(kSlotR) - 1] + aa[Player->GetSpellLevel(kSlotR) - 1] * (unit->GetHealth() / 100);
        return dmg + xtra; }

    return dmg; }

inline double ZCamille::SuperRotation(IUnit * unit) {
    int a[] = { 5, 6, 7 };
    return QDmg(unit, true) * a[min(18, Player->GetLevel()) / 6] + WDmg(unit, true) + EDmg(unit) + RDmg(GDamage->GetAutoAttackDamage(Player, unit, true) *
            (a[min(18, Player->GetLevel()) / 6] * 0.75), unit); }

inline double ZCamille::HardRotation(IUnit * unit) {
    int a[] = { 3, 4, 5 };
    return QDmg(unit, true) * a[min(18, Player->GetLevel()) / 6] + WDmg(unit, true) + EDmg(unit) + RDmg(GDamage->GetAutoAttackDamage(Player, unit, true) *
            (a[min(18, Player->GetLevel()) / 6] * 0.75), unit); }

inline double ZCamille::StandardRotation(IUnit * unit) {
    int a[] = { 2, 3, 4 };
    return QDmg(unit, true) * a[min(18, Player->GetLevel()) / 6] + WDmg(unit, true) + EDmg(unit) + RDmg(GDamage->GetAutoAttackDamage(Player, unit, true) *
            (a[min(18, Player->GetLevel()) / 6] * 0.75), unit); }

inline double ZCamille::EasyRotation(IUnit * unit) {
    int a[] = { 1, 2, 3 };
    return QDmg(unit, true) * a[min(18, Player->GetLevel()) / 6] + WDmg(unit, true) + EDmg(unit) + RDmg(GDamage->GetAutoAttackDamage(Player, unit, true) *
            (a[min(18, Player->GetLevel()) / 6] * 0.75), unit); }


