#pragma once
#include "ZZedMenu.h"
#include "ZZedModes.h"
#include "ZZedExtensions.h"
#include <map>
#include <string>

#define M_PI 3.14159265358979323846

class ZZed {
    public:

        static IUnit * Player;
        static ZZedMenu * Menu;
        static ZZedModes * Modes;
        static ZZedExtensions * Ex;

        static ISpell * W;
        static ISpell * E;
        static ISpell * R;
        static ISpell * Ignite;
        static ISpell2 * Q;
        static IInventoryItem * Tiamat;
        static IInventoryItem * Hydra;
        static IInventoryItem * Titanic;
        static IInventoryItem * Youmuus;

        static void UseQ(IUnit * unit, bool harass = false);
        static void UseW(IUnit * unit, bool harass);
        static void UseE(IUnit * unit, bool harass);
        static void UseR(IUnit * unit, bool beans);

        static bool SoloQ(Vec3 sourcepos, IUnit * unit);
        static void GetMaxWPositions(IUnit * unit, Vec3 & wpos);
        static void GetBestWPosition(IUnit * unit, Vec3 & wpos, bool draw = false);

        static void OnBoot();
        static void OnShutdown();
        static bool CompareLowHealth(IUnit * a, IUnit * b);
        static bool CompareMaxHealth(IUnit * a, IUnit * b);
        static bool CompareDistanceToCursor(IUnit * a, IUnit * b);
        static bool GetPriorityJungleTarget(IUnit * a, IUnit * b);
        static void CanUlt(IUnit * unit, bool & morebeans);
        static bool Beans(std::string name, float time);


        static bool CanSwap(ISpell * spell);
        static bool HasDeathMark(IUnit * unit);
        static bool WShadowExists();
        static bool RShadowExists();

        static IUnit * RShadow();
        static IUnit * WShadow();

        static std::map<float, IUnit *> Marked;
        static std::map<float, IUnit *> Shadows;
        static std::map<std::string, float> Ticks;

        // damage
        static double QDmg(IUnit * source, IUnit * unit, double & energy);
        static double EDmg(IUnit * unit, double & energy);
        static double RDmg(IUnit * unit, float predictedDmg);
        static double AADmg(IUnit * unit, float healthmod);
        static double CDmg(IUnit * unit, double & energy); };

inline void ZZed::OnBoot() {

    // init timestamps
    Ticks["ZedQ"] = 0;
    Ticks["ZedW"] = 0;
    Ticks["ZedW2"] = 0;
    Ticks["ZedE"] = 0;
    Ticks["ZedR"] = 0;
    Ticks["ZedR2"] = 0;

    if(strcmp(Player->GetSpellName(kSummonerSlot1), "SummonerDot") == 0) {
        Ignite = GPluginSDK->CreateSpell(kSummonerSlot1, 550); }

    if(strcmp(Player->GetSpellName(kSummonerSlot2), "SummonerDot") == 0) {
        Ignite = GPluginSDK->CreateSpell(kSummonerSlot2, 550); }

    // items
    Tiamat = GPluginSDK->CreateItemForId(3077, 400);
    Hydra = GPluginSDK->CreateItemForId(3074, 400);
    Titanic = GPluginSDK->CreateItemForId(3748, 400);
    Youmuus = GPluginSDK->CreateItemForId(3142, 900);

    W = GPluginSDK->CreateSpell(kSlotW, 700);
    E = GPluginSDK->CreateSpell(kSlotE, 200);
    R = GPluginSDK->CreateSpell(kSlotR, 625);

    Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, true, false, kCollidesWithYasuoWall);
    Q->SetSkillshot(0.25, 50, 1700, 900); }

inline void ZZed::OnShutdown() { }

inline bool ZZed::CompareLowHealth(IUnit * a, IUnit * b) {
    return a->HealthPercent() < b->HealthPercent(); }

inline bool ZZed::CompareMaxHealth(IUnit * a, IUnit * b) {
    return a->GetMaxHealth() > b->GetMaxHealth(); }

inline bool ZZed::CompareDistanceToCursor(IUnit * a, IUnit * b) {
    return Ex->Dist2D(a->ServerPosition(), GGame->CursorPosition()) < Ex->Dist2D(b->ServerPosition(), GGame->CursorPosition()); }

inline bool ZZed::GetPriorityJungleTarget(IUnit * a, IUnit * b) {
    switch(Menu->JungleOrderPriority->GetInteger()) {
        case 0:
            return CompareLowHealth(a, b);

        case 1:
            return CompareMaxHealth(a, b);

        case 2:
            return CompareDistanceToCursor(a, b); }

    return false; }

inline bool ZZed::Beans(std::string name, float time = 0) {
    return  GGame->TickCount() - Ticks[name] < 500 + time - GGame->Latency(); }

inline void ZZed::CanUlt(IUnit * unit, bool & coolbeans) {
    coolbeans = false;

    if(unit != nullptr && R->IsReady() && Menu->UseComboR->Enabled()) {
        auto focus = GTargetSelector->GetFocusedTarget();
        double energy = 0;

        if(HasDeathMark(unit)) {
            coolbeans = false; }

        if(Menu->AlwaysRTargets[unit->ChampionName()]->Enabled() && Menu->UseAlwaysR->Enabled()
            || CDmg(unit, energy) /*100001*/ >= unit->GetHealth()
            || focus != nullptr && focus->GetNetworkId() == unit->GetNetworkId()) {
            coolbeans = true; } } }

inline bool ZZed::CanSwap(ISpell * spell) {
    return strstr(Player->GetSpellName(spell->GetSpellSlot()), std::to_string(2).c_str()); }

inline bool ZZed::HasDeathMark(IUnit * unit) {

    for(auto o : Marked) {
        auto x = o.second;
        return x != nullptr && x->GetNetworkId() == unit->GetNetworkId(); }

    return false; }

inline bool ZZed::WShadowExists() {
    if(Beans("ZedW")) {
        return true; }

    for(auto z : Shadows) {
        auto shadow = z.second;

        if(shadow->HasBuff("zedwshadowbuff")) {
            return true; } }

    return  false; }

inline bool ZZed::RShadowExists() {
    if(Beans("ZedR")) {
        return true; }

    for(auto z : Shadows) {
        auto shadow = z.second;

        if(shadow->HasBuff("zedrshadowbuff")) {
            return true; } }

    return  false; }

inline IUnit * ZZed::RShadow() {
    for(auto o : Shadows) {
        auto shadow = o.second;

        if(shadow->HasBuff("zedrshadowbuff")) {
            return shadow; } }

    return nullptr; }

inline IUnit * ZZed::WShadow() {
    for(auto o : Shadows) {
        auto shadow = o.second;

        if(shadow->HasBuff("zedwshadowbuff")) {
            return shadow; } }

    return nullptr; }

inline void ZZed::UseQ(IUnit * unit, bool harass) {

    if(unit == nullptr || !unit->IsValidTarget()) {
        return; }

    bool somebeans;
    CanUlt(unit, somebeans);

    if(Q->IsReady()) {
        if(Ex->Dist2D(unit) <= Q->Range() && harass && Menu->UseHarassQ->Enabled()
            || Ex->Dist2D(unit) <= Q->Range() && !harass && Menu->UseComboQ->Enabled()) {
            Q->CastOnTarget(unit); }

        if(harass && Ex->Dist2D(unit) > Q->Range() && Menu->UseHarassQ->Enabled()) {
            for(auto z : Shadows) {
                auto shadow = z.second;

                if(Ex->Dist2D(shadow, unit) <= Q->Range() + unit->BoundingRadius()) {
                    if(Ex->Dist2D(shadow, unit) <= E->GetSpellRange() + unit->BoundingRadius() || Menu->ExtendedQHarass->Enabled()) {
                        Q->SetFrom(shadow->ServerPosition());
                        Q->SetRangeCheckFrom(shadow->ServerPosition());

                        if(Q->CastOnTarget(unit)) {
                            Q->SetFrom(Player->ServerPosition());
                            Q->SetRangeCheckFrom(Player->ServerPosition()); } } } } }

        if(!harass && Ex->Dist2D(unit) > Q->Range() && Menu->UseComboQ->Enabled()) {
            for(auto z : Shadows) {
                auto shadow = z.second;

                if(Ex->Dist2D(shadow, unit) <= Q->Range() + unit->BoundingRadius()) {
                    if(Ex->Dist2D(shadow, unit) <= E->GetSpellRange() + unit->BoundingRadius() || Menu->ExtendedQCombo->Enabled()) {
                        Q->SetFrom(shadow->ServerPosition());
                        Q->SetRangeCheckFrom(shadow->ServerPosition());

                        if(Q->CastOnTarget(unit)) {
                            Q->SetFrom(Player->ServerPosition());
                            Q->SetRangeCheckFrom(Player->ServerPosition()); } } } } } } }

inline void ZZed::UseW(IUnit * unit, bool harass) {

    if(unit == nullptr
        || !unit->IsValidTarget()) {
        return; }

    // default castposition
    auto castposition = unit->ServerPosition();

    bool somebeans;
    CanUlt(unit, somebeans);

    // update the cast position via our method
    GetBestWPosition(unit, castposition);

    if(!WShadowExists()) {
        if(Ex->Dist2D(unit) <= W->GetSpellRange() * 2) {

            // combo check if extended position has enemy near e, or if extended q is enabled. Go full yolo if we have a marked target
            if((!harass && Ex->Dist2D(unit) <= W->GetSpellRange() + E->GetSpellRange() || !harass && Menu->ExtendedQCombo->Enabled())) {
                if(Q->IsReady() && Player->GetMana() >= Q->ManaCost() + W->GetManaCost()) {
                    W->CastOnPosition(castposition); }

                if(E->IsReady() && Player->GetMana() >= E->GetManaCost() + W->GetManaCost()) {
                    W->CastOnPosition(castposition); } }

            // harass check if extended position has enemy near e, or if extended q is enabled. Go full yolo if we have a marked target
            if((harass && Ex->Dist2D(unit) <= W->GetSpellRange() + E->GetSpellRange() || harass && Menu->ExtendedQHarass->Enabled())) {
                if(Q->IsReady() && Player->GetMana() >= Q->ManaCost() + W->GetManaCost()) {
                    W->CastOnPosition(castposition); }

                if(E->IsReady() && Player->GetMana() >= E->GetManaCost() + W->GetManaCost()) {
                    W->CastOnPosition(castposition); } } } } }

inline void ZZed::UseE(IUnit * unit, bool harass) {
    if(unit == nullptr || !unit->IsValidTarget()) {
        return; }

    if(E->IsReady() && Player->GetMana() >= E->GetManaCost()) {
        if(Ex->Dist2D(unit) <= E->GetSpellRange()) {
            if(!harass && Menu->UseComboE->Enabled() || harass && Menu->UseHarassE->Enabled()) {
                E->CastOnPlayer(); } } }

    for(auto o : Shadows) {
        auto shadow = o.second;

        if(Ex->Dist2D(unit, shadow) <= E->GetSpellRange()) {
            if(!harass && Menu->UseComboE->Enabled() || harass && Menu->UseHarassE->Enabled()) {
                E->CastOnPlayer(); } } } }

inline void ZZed::UseR(IUnit * unit, bool beans) {
    if(unit != nullptr && unit->IsValidTarget()) {

        double energy = 0;

        if(RShadowExists() || !Menu->UseComboR->Enabled()) {
            return; }

        if(R->IsReady() && beans) {
            if(Ex->Dist2D(unit) <= R->GetSpellRange()) {
                if(Player->GetMana() >= energy) {
                    if(Youmuus->IsReady()) {
                        Youmuus->CastOnPlayer(); }

                    if(Ex->Dist2D(unit) <= Ignite->GetSpellRange() && Ignite->IsReady()) {
                        Ignite->CastOnUnit(unit); }

                    R->CastOnUnit(unit); } }

            for(auto o : Shadows) {
                auto shadow = o.second;

                if(shadow->HasBuff("zedwshadowbuff") && CanSwap(W) && !HasDeathMark(unit)) {
                    if(Ex->Dist2D(shadow, unit) <= R->GetSpellRange() + 25) {
                        W->CastOnPlayer(); } } } }

        if(!R->IsReady() && beans) {
            for(auto o : Shadows) {
                auto shadow = o.second;

                if(shadow->HasBuff("zedwshadowbuff") && CanSwap(W) && !HasDeathMark(unit)) {
                    if(Ex->Dist2D(shadow, unit) <= Player->AttackRange() + 25) {
                        W->CastOnPlayer(); }

                    if(E->IsReady() && Ex->Dist2D(shadow, unit) <= E->GetSpellRange() + 25) {
                        W->CastOnPlayer(); } } } } } }

inline bool ZZed::SoloQ(Vec3 sourcepos, IUnit * unit) {

    // check if zed shuriken collides with enemy minions or heroes
    auto pred = new AdvPredictionOutput;

    // set new source position
    Q->SetFrom(sourcepos);
    Q->SetRangeCheckFrom(sourcepos);

    if(Q->RunPrediction(unit, false, static_cast<eCollisionFlags>(kCollidesWithHeroes | kCollidesWithMinions), pred)) {
        if(pred->HitChance != kHitChanceCollision) {
            return true; } }

    // to be safe reset source position
    Q->SetFrom(Player->ServerPosition());
    Q->SetRangeCheckFrom(Player->ServerPosition());

    return false; }

inline double ZZed::QDmg(IUnit * source, IUnit * unit, double & energy) {

    // initial q damage
    double dmg = 0;

    // predicted shuriken count
    int shurikens = 1;

    if(unit != nullptr && unit->IsValidTarget() && Ex->IsReady(Q, 2) && Player->GetMana() >= Q->ManaCost()) {
        Vec3 wposition;
        energy += Q->ManaCost();

        // if networkId is me predict extra damage because shadows aren't created yet
        if(source->GetNetworkId() == Player->GetNetworkId()) {
            GetBestWPosition(unit, wposition);

            if(W->IsReady() && !WShadowExists()) {
                energy += W->GetManaCost(); }

            if(W->IsReady() && SoloQ(wposition, unit)) {
                shurikens += 1; }

            if(R->IsReady() && SoloQ(Player->ServerPosition(), unit)) {
                shurikens += 1; } }

        dmg += GDamage->CalcPhysicalDamage(Player, unit, std::vector<int> {70, 105, 140, 175, 210 } [Player->GetSpellLevel(kSlotQ) - 1] + (0.9 * Player->BonusDamage()));

        // reduce damage if the main unit is not the first thing hit
        if(!SoloQ(source->ServerPosition(), unit)) {
            dmg = dmg * 0.6; }

        if(shurikens > 1) {
            dmg += dmg * (shurikens - 1) * 0.75; }

        // amplify damage credits @rembrandt for mastery data
        return dmg * Ex->AmplifyDamage(Player, unit); }

    return  dmg; }

inline double ZZed::EDmg(IUnit * unit, double & energy) {

    // initial e damage
    double dmg = 0;


    if(unit != nullptr && unit->IsValidTarget() && Ex->IsReady(E, 2) && Player->GetMana() >= E->GetManaCost()) {
        energy += E->GetManaCost();
        dmg += GDamage->CalcPhysicalDamage(Player, unit, std::vector<int> {65, 90, 115, 140, 165 } [Player->GetSpellLevel(kSlotE) - 1] + (0.8 * Player->BonusDamage()));

        return dmg * Ex->AmplifyDamage(Player, unit); }

    return  dmg; }

inline double ZZed::RDmg(IUnit * unit, float predictedDmg = 0) {
    double dmg = 0;

    if(unit != nullptr && unit->IsValidTarget() && R->IsReady() && !CanSwap(R)) {
        dmg += GDamage->CalcPhysicalDamage(Player, unit, predictedDmg * std::vector<double> {0.25, 0.35, 0.45, 0.45 } [Player->GetSpellLevel(kSlotR) - 1] + 1 * Player->TotalPhysicalDamage());

        return dmg; }

    return  dmg; }

inline double ZZed::AADmg(IUnit * unit, float predictedDmg = 0) {
    double dmg = 0;

    if(unit != nullptr && unit->IsValidTarget()) {

        int aa = 1;

        if(Menu->UltMode->GetInteger() == 1) {
            aa += std::vector<int> { 2, 2, 3, 3 } [min(18, Player->GetLevel()) / 6]; }

        if(!unit->HasBuff("zedpassivecd") && unit->HealthPercent() <= 50) {
            dmg += GDamage->CalcMagicDamage(Player, unit, std::vector<double> {0.06, 0.08, 0.1, 0.1 } [min(18, Player->GetLevel()) / 6] * (unit->GetMaxHealth() - predictedDmg)); }

        dmg += GDamage->GetAutoAttackDamage(Player, unit, false) * aa;

        return dmg; }

    return  dmg; }

inline double ZZed::CDmg(IUnit * unit, double & energy) {

    // initial combo damage
    double dmg = 0;

    if(unit == nullptr
        || !unit->IsValidTarget()) {
        return dmg; }

    // initial q damage
    double qq = QDmg(Player, unit, energy);;

    for(auto i : Shadows) {
        auto shadow = i.second;
        qq += QDmg(shadow, unit, energy); }

    auto ee = EDmg(unit, energy);
    auto aa = AADmg(unit, qq + ee);
    auto rr = RDmg(unit, qq + ee + aa);

    if(Ignite->GetSpellSlot() != kSlotUnknown && Ignite->IsReady()) {
        dmg += GDamage->GetSummonerSpellDamage(Player, unit, kSummonerSpellIgnite); }

    dmg += qq + ee + aa + rr;
    return dmg; }

inline void ZZed::GetBestWPosition(IUnit * unit, Vec3 & wpos, bool draw) {

    if(!Beans("ZedR", 1500) && !draw) {
        wpos = unit->ServerPosition();
        return; }

    // get w position using r shadow position
    if(Shadows.size() > 0) {

        for(auto o : Shadows) {
            auto shadow = o.second;

            // line
            if(Menu->ShadowPlacement->GetInteger() == 0) {
                if(shadow->HasBuff("zedrshadowbuff")) {
                    wpos = shadow->ServerPosition().Extend(unit->ServerPosition(), W->GetSpellRange() + 200); } }

            // triangle
            if(Menu->ShadowPlacement->GetInteger() == 1) {
                if(shadow->HasBuff("zedrshadowbuff")) {
                    auto v = Ex->CircleCircleIntersection(Ex->To2D(shadow->ServerPosition()), Ex->To2D(Player->ServerPosition()), 550, 450);
                    wpos = v.size() > 0 ? Ex->To3D(v.front()) : shadow->ServerPosition().Extend(unit->ServerPosition(), W->GetSpellRange() + 200); } } }

        if(Menu->ShadowPlacement->GetInteger() == 2) {
            GetMaxWPositions(unit, wpos); } }

    // simulate a shadow position using player position
    else {
        // line
        if(Menu->ShadowPlacement->GetInteger() == 0) {
            wpos = Player->ServerPosition().Extend(unit->ServerPosition(), W->GetSpellRange() + 200); }

        // triangle
        if(Menu->ShadowPlacement->GetInteger() == 1) {
            auto v = Ex->CircleCircleIntersection(Ex->To2D(unit->ServerPosition()), Ex->To2D(Player->ServerPosition()), 400, 550);
            wpos = v.size() > 0 ? Ex->To3D(v.front()) : Player->ServerPosition().Extend(unit->ServerPosition(), W->GetSpellRange() + 200); }

        // check multiple positions find best one with less units in way
        if(Menu->ShadowPlacement->GetInteger() == 2) {
            GetMaxWPositions(unit, wpos); } }

}


inline void ZZed::GetMaxWPositions(IUnit * unit, Vec3 & wpos) {

    auto posChecked = 0;
    auto maxPosChecked = 12;
    auto posRadius = 75;
    auto radiusIndex = 0;

    auto possiblePositions = std::vector<Vec3>();

    while(posChecked < maxPosChecked) {
        radiusIndex++;

        auto curRadius = radiusIndex * (0x2 * posRadius);
        auto curCurcleChecks = static_cast<int>(ceil((0x2 * M_PI * curRadius) / (0x2 * static_cast<double>(posRadius))));

        for(auto i = 0x1; i < curCurcleChecks; i++) {
            posChecked++;
            auto cRadians = (0x2 * M_PI / (curCurcleChecks - 0x1)) * i;
            auto xaxis = static_cast<float>(floor(unit->ServerPosition().x + curRadius * cos(cRadians)));
            auto yaxis = static_cast<float>(floor(unit->ServerPosition().z + curRadius * sin(cRadians)));
            auto position = Vec3(xaxis, 0, yaxis);

            // todo: maybe check fountain

            // dont push if it is a wall;
            if(GNavMesh->IsPointWall(position)) {
                continue; }

            // if wposition range greater than my range dont push
            if(Ex->Dist2D(Player->ServerPosition(), position) > W->GetSpellRange()) {
                continue; }

            // if wposition range greater than enemy range dont push
            if(Ex->Dist2D(unit->ServerPosition(), position) > W->GetSpellRange()) {
                continue; }

            // fps drop? :s
            if(SoloQ(position, unit)) {

                if(Menu->DebugPathfinding->Enabled()) {
                    GRender->DrawCircle(position, 100, Vec4(255, 0, 255, 255), 2, false, true); }

                possiblePositions.push_back(position); } } }

    // sort by furthest to enemy
    std::sort(possiblePositions.begin(), possiblePositions.end(), [&](Vec3 v1, Vec3 v2) {
        return Ex->Dist2D(unit->ServerPosition(), v1) > Ex->Dist2D(unit->ServerPosition(), v2); });
    // then by furthest to me
    std::sort(possiblePositions.begin(), possiblePositions.end(), [&](Vec3 v1, Vec3 v2) {
        return Ex->Dist2D(Player->ServerPosition(), v1) > Ex->Dist2D(Player->ServerPosition(), v2); });

    if(possiblePositions.size() > 0) {
        for(auto vec : possiblePositions) {
            wpos = vec; } }
    else {
        // if all fail just do line kappa
        wpos = Player->ServerPosition().Extend(unit->ServerPosition(), W->GetSpellRange() + 200); } }




