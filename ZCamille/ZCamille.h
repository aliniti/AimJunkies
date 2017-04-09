#pragma once
#include "ZCamilleModes.h"
#include "ZCamilleMenu.h"
#include "ZCamilleAvoider.h"
#include "ZCamilleExtensions.h"

#include <math.h>
#include <map>
#include <string>

#define M_PI 3.14159265358979323846

class ZCamille {
    public:
        ~ZCamille();

        static void SetupItems();
        static void SetupSpells();

        static IUnit * Player;
        static ZCamilleMenu * Menu;
        static ZCamilleModes * Modes;
        static ZCamilleExtensions * Ex;
        static IInventoryItem * Youmuus;
        static IInventoryItem * Tiamat;
        static IInventoryItem * Hydra;
        static IInventoryItem * Titanic;
        static ISpell2 * Q, *W, *E, *R;

        static std::map<std::string, ZCamilleAvoider *> AvoidList;
        static std::map<float, ZCamilleAvoider *> DangerPoints;

        // aura mechanics
        static bool HasQ();
        static bool HasQ2();
        static bool ChargingW();;
        static bool OnWall();
        static bool IsDashing();
        static bool KnockedBack(IUnit * unit);
        static bool KeyState;

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
        static bool LethalTarget(IUnit * unit);
        static double CDmg(IUnit * unit);
        static double QDmg(IUnit * unit, bool includeq2 = true);
        static double WDmg(IUnit * unit, bool bonus = false);
        static double EDmg(IUnit * unit);
        static double RDmg(double dmg, IUnit * unit);
        static auto DrawDamageOnChampionHPBar(IUnit * Hero, double Damage, const char * Text, Vec4 BarColor) -> void;
        static auto DrawDamageOnChampionHPBar(IUnit * Hero, double Damage, Vec4 BarColor) -> void; };

inline void ZCamille::SetupItems() {
    Tiamat = GPluginSDK->CreateItemForId(3077, 400);
    Hydra = GPluginSDK->CreateItemForId(3074, 400);
    Titanic = GPluginSDK->CreateItemForId(3748, 400);
    Youmuus = GPluginSDK->CreateItemForId(3142, 900); }

inline void ZCamille::SetupSpells() {
    // hi im using estimated values :^)
    Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, false, kCollidesWithNothing);
    Q->SetOverrideRange(175);
    W = GPluginSDK->CreateSpell2(kSlotW, kConeCast, false, true, kCollidesWithNothing);
    W->SetSkillshot(0.5, 200, 9999, 625);
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

inline auto ZCamille::DrawDamageOnChampionHPBar(IUnit * Hero, double Damage, const char * Text, Vec4 BarColor) -> void {
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

inline auto ZCamille::DrawDamageOnChampionHPBar(IUnit * Hero, double Damage, Vec4 BarColor) -> void {
    Vec2 HPBarPos;

    if(Hero->GetHPBarPosition(HPBarPos)) {
        Vec2 HPBarSize = Vec2(103 * (Damage / Hero->GetMaxHealth()), 8);
        HPBarPos = Vec2(HPBarPos.x + 10, HPBarPos.y += 20);

        Vec2 LinePos1 = Vec2(HPBarPos.x + HPBarSize.x, HPBarPos.y);
        Vec2 LinePos2 = Vec2(HPBarPos.x + HPBarSize.x - 5, HPBarPos.y - 7);

        GRender->DrawFilledBox(HPBarPos, HPBarSize, BarColor); } }


inline void ZCamille::UseQ(IUnit * unit) {
    if(Q->IsReady()) {
        if(!HasQ() || HasQ2()) {
            if(Q->CastOnPlayer()) {
                GOrbwalking->ResetAA(); } }

        else {
            if(unit != nullptr && unit->IsHero()) {
                if(QDmg(unit, false) + GDamage->GetAutoAttackDamage(Player, unit, true) * 1 >= unit->GetHealth()) {
                    if(Q->CastOnPlayer()) {
                        GOrbwalking->ResetAA(); } } } } } }

inline void ZCamille::UseW(IUnit * unit) {
    if(!ChargingW() && !IsDashing() && !OnWall() && CanW(unit)) {
        if(KnockedBack(unit)) {
            return; }

        if(W->IsReady() && Ex->Dist2D(unit) <= W->Range()) {
            W->CastOnPosition(unit->ServerPosition()); } } }

inline bool ZCamille::CanW(IUnit * unit) {
    std::vector<void *> buffPtr;
    Player->GetAllBuffsData(buffPtr);
    const float animationtime = 2000;

    if(OnWall() || IsDashing() || unit == nullptr) {
        return false; }

    if(GGame->TickCount() - LastE < 1000) {
        // to prevent instant w after e
        return false; }

    if(Q->IsReady()) {
        for(auto i : buffPtr) {
            if(i != nullptr) {
                if(strcmp("camilleqprimingstart", GBuffData->GetBuffName(i))) {
                    if((GBuffData->GetEndTime(i) - GGame->Time()) * 1000 <= animationtime) {
                        return false; } }

                if(strcmp("camilleqprimingcomplete", GBuffData->GetBuffName(i))) {
                    if((GBuffData->GetEndTime(i) - GGame->Time()) * 1000 <= animationtime) {
                        return false; } } } }

        if(!HasQ() || HasQ2()) {
            if(Ex->Dist(unit) <=  Player->AttackRange() + 35) {
                return false; } }

        else {
            if(QDmg(unit, false) + GDamage->GetAutoAttackDamage(Player, unit, true) * 1 >= unit->GetHealth()) {
                return  false; } } }

    if(Ex->Dist2D(unit) <= Player->AttackRange() + 35) {
        if(GDamage->GetAutoAttackDamage(Player, unit, true) * 3 > unit->GetHealth()) {
            return false; } }

    return true; }

inline void ZCamille::LockW(IUnit * unit) {
    if(OnWall() || IsDashing() || unit == nullptr) {
        return; }

    if(ChargingW() && Ex->Dist2D(unit) <= W->Range() + unit->BoundingRadius() + Player->BoundingRadius()) {
        Vec3 pos;
        GPrediction->GetFutureUnitPosition(unit, 100, true, pos);

        if(Ex->IsValid(pos)) {
            GGame->IssueOrder(Player, kMoveTo, pos.Extend(Player->ServerPosition(), W->Range() - 65)); } } }

inline void ZCamille::LockW(Vec3 p) {
    if(OnWall() || IsDashing() || !Ex->IsValid(p)) {
        return; }

    if(ChargingW() && Ex->Dist2D(Player->ServerPosition(), p) <= W->Range() + Player->BoundingRadius() * 2) {
        GGame->IssueOrder(Player, kMoveTo, p.Extend(Player->ServerPosition(), W->Range() - 25)); } }

inline void ZCamille::UseE(Vec3 pos, bool combo = true) {
    if(IsDashing() || OnWall() || !E->IsReady()) {
        return; }

    if(combo) {
        if(Ex->Dist2D(Player->ServerPosition(), pos) - Player->BoundingRadius() * 2 < Menu->MinimumERange->GetInteger()) {
            return; }

        if(Ex->UnderEnemyTurret(pos) && Menu->DontEUnderTurret->Enabled()) {
            return; } }

    auto posChecked = 0;
    auto maxPosChecked = 40;
    auto posRadius = 145;
    auto radiusIndex = 0;

    if(Menu->EnhancedEPrecision->Enabled()) {
        maxPosChecked = 80;
        posRadius = 65; }

    auto candidatePosList = std::vector<Vec2>();

    while(posChecked < maxPosChecked) {
        radiusIndex++;
        auto curRadius = radiusIndex * (0x2 * posRadius);
        auto curCurcleChecks = static_cast<int>(ceil((0x2 * M_PI * curRadius) / (2 * static_cast<double>(posRadius))));

        for(auto i = 1; i < curCurcleChecks; i++) {
            posChecked++;
            auto cRadians = (2 * M_PI / (curCurcleChecks - 1)) * i;
            auto xPos = static_cast<float>(floor(pos.x + curRadius * cos(cRadians)));
            auto zPos = static_cast<float>(floor(pos.z + curRadius * sin(cRadians)));
            auto posFor2D = Vec2(xPos, zPos);
            auto anyDangerousPos = false;

            for(auto entry : DangerPoints) {
                auto obj = entry.second;

                if(obj->aType == Outside && Ex->Dist2D(posFor2D, obj->Emitter->GetPosition()) > obj->Radius) {
                    anyDangerousPos = true;
                    break; }

                if(obj->aType == Inside) {
                    auto startPos = posFor2D;
                    auto endPos = Ex->To2D(pos);
                    auto initPos = Ex->To2D(entry.second->Emitter->GetPosition());
                    auto proj = Ex->ProjectOn(initPos, startPos, endPos);

                    if(proj->IsOnSegment && Ex->Dist2D(proj->SegmentPoint, initPos) <= obj->Radius + Player->BoundingRadius()) {
                        anyDangerousPos = true;
                        break; } } }

            if(anyDangerousPos) {
                continue; }

            auto wtarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());

            if(wtarget != nullptr && wtarget->IsValidTarget() && ChargingW()) {
                if(Ex->Dist2D(Ex->To2D(wtarget->ServerPosition()), posFor2D) > W->Range() - 100) {
                    continue; } }

            if(GNavMesh->IsPointWall(Ex->To3D(posFor2D))) {
                candidatePosList.push_back(posFor2D); }

            std::sort(candidatePosList.begin(), candidatePosList.end(), [&](Vec2 a, Vec2 b) {
                switch(Menu->PreferedHookPoint->GetInteger()) {
                    case 0:
                        return Ex->Dist2D(a, pos) < Ex->Dist2D(b, pos);

                    case 1:
                        return Ex->Dist2D(a, Player->ServerPosition()) < Ex->Dist2D(b, Player->ServerPosition()); } }); } }

    if(ChargingW() == false) {
        for(auto vec : candidatePosList) {
            if(Ex->IsValid(vec) != false) {
                if(Ex->Dist2D(Ex->To3D(vec), Player->ServerPosition()) <= E->Range() && Ex->Dist2D(Ex->To3D(vec), pos) <= E->Range()) {
                    if(Ex->IsValid(vec)) {
                        if(W->IsReady() && Menu->ExpirimentalCombo->Enabled() && combo) {
                            // todo
                        }

                        if(E->CastOnPosition(Ex->To3D(vec))) {
                            LastE = GGame->TickCount(); } } } } } } }

inline void ZCamille::UseR(IUnit * unit, bool force = false) {
    if(R->IsReady() && force) {
        R->CastOnUnit(unit); }

    if(R->IsReady() && Ex->Dist2D(unit) <= R->Range()) {
        if(Menu->UltOnlySelected->Enabled()) {
            auto selected = GTargetSelector->GetFocusedTarget();

            if(selected == nullptr || selected->GetNetworkId() != unit->GetNetworkId()) {
                return; } }

        if(QDmg(unit) + GDamage->GetAutoAttackDamage(Player, unit, true) * 2 >= unit->GetHealth()) {
            if(Ex->Dist2D(unit) <= unit->BoundingRadius() + Player->AttackRange()) {
                return; } }

        if(R->IsReady() && CDmg(unit) >= unit->GetHealth()) {
            R->CastOnUnit(unit); } } }

inline double ZCamille::CDmg(IUnit * unit) {
    if(unit != nullptr) {
        auto qq = std::vector<int> {1, 1, 2, 2, 3 };
        auto qqcount = std::vector<int> {2, 3, 4, 4 } [min(Player->GetLevel(), 18) / 6];

        qqcount += abs(Player->GetCooldownReductionPercent()) * 100 / 10;

        switch(Menu->RWhen->GetInteger()) {
            case 0:
                qqcount = qqcount - round(qqcount - 0.6);

            case 1:
                qqcount = qqcount - round(qqcount * 0.4);

            case 2:
                qqcount = qqcount - round(qqcount * 0.3);

            default: qqcount = qqcount; }

        auto az = qqcount * qq[static_cast<int>(abs(Player->GetCooldownReductionPercent()) * 100 / 10)];
        auto ac = Player->GetMana() / Q->ManaCost();
        auto aa = GDamage->GetAutoAttackDamage(Player, unit, true);

        return min(az, ac) * QDmg(unit, false) + WDmg(unit, false) + (RDmg(aa * qqcount, unit)) + EDmg(unit); }

    return 0; }

inline double ZCamille::QDmg(IUnit * unit, bool bonus) {
    double dmg = 0;

    if(Q->IsReady() && unit != nullptr) {
        dmg += GDamage->CalcPhysicalDamage(Player, unit, GDamage->GetAutoAttackDamage(Player, unit, true) + std::vector<double> {0.2, 0.25, 0.30, 0.35, 0.40 } [Player->GetSpellLevel(kSlotQ) - 1] * (Player->PhysicalDamage() + Player->PhysicalDamageMod()));
        auto dmgreg = GDamage->CalcPhysicalDamage(Player, unit, GDamage->GetAutoAttackDamage(Player, unit, true) + std::vector<double> {0.4, 0.5, 0.6, 0.7, 0.8 } [Player->GetSpellLevel(kSlotQ) - 1] * (Player->PhysicalDamage() + Player->PhysicalDamageMod()));
        auto pct = 52 + (3 * min(16, Player->GetLevel()));
        auto dmgtrue = dmgreg * pct / 100;

        if(bonus) {
            dmg += dmgtrue; } }

    return dmg; }

inline double ZCamille::WDmg(IUnit * unit, bool bonus) {
    double dmg = 0;

    if(W->IsReady() && unit != nullptr) {
        dmg += GDamage->CalcPhysicalDamage(Player, unit, std::vector<double> { 65, 95, 125, 155, 185 } [Player->GetSpellLevel(kSlotW) - 1] + (0.6 * Player->PhysicalDamageMod()));
        auto pct = std::vector<double> { 6, 6.5, 7, 7.5, 8 } [Player->GetSpellLevel(kSlotW) - 1];

        if(Player->PhysicalDamageMod() >= 100) {
            pct += min(300, Player->PhysicalDamageMod() * 3 / 100); }

        if(bonus && Ex->Dist2D(unit) > 400) {
            dmg += GDamage->CalcPhysicalDamage(Player, unit, pct * (unit->GetMaxHealth() / 100)); } }

    return dmg; }

inline double ZCamille::EDmg(IUnit * unit) {
    double dmg = 0;

    if(E->IsReady() && unit != nullptr) {
        dmg += GDamage->CalcPhysicalDamage(Player, unit, GDamage->GetAutoAttackDamage(Player, unit, true) + std::vector<double> { 70, 115, 160, 205, 250 } [Player->GetSpellLevel(kSlotE) - 1] + (0.75 * Player->PhysicalDamageMod())); }

    return dmg; }

inline double ZCamille::RDmg(double dmg, IUnit * unit) {
    if(unit == nullptr) {
        return 0; }

    if(R->IsReady() || unit->HasBuff("camillertether")) {
        auto xtra = std::vector<double> { 5, 10, 15, 15 } [Player->GetSpellLevel(kSlotR) - 1] +
                    std::vector<double> { 4, 6, 8, 8 } [Player->GetSpellLevel(kSlotR) - 1] * (unit->GetHealth() / 100);
        return dmg + xtra; }

    return dmg; }


