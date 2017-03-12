#include <string>
#include "ZCamille.h"
#include "ZCamilleExtensions.h"
#include "PluginData.h"

bool ZCamilleExtensions::IsValid(Vec3 p) {
    return p.x != 0 && p.z != 0; }

bool ZCamilleExtensions::IsValid(Vec2 p) {
    return p.x != 0 && p.y != 0; }

Vec2 ZCamilleExtensions::To2D(Vec3 p) {
    return Vec2(p.x, p.z); }

Vec3 ZCamilleExtensions::To3D(Vec2 p) {
    return Vec3(p.x, 0, p.y); }

float ZCamilleExtensions::Dist(IUnit * to) {
    return (ZCamille::Player->ServerPosition() - to->ServerPosition()).Length() -
           (ZCamille::Player->BoundingRadius() + to->BoundingRadius()); }

float ZCamilleExtensions::Dist2D(IUnit * to) {
    return (ZCamille::Player->ServerPosition() - to->ServerPosition()).Length2D() -
           (ZCamille::Player->BoundingRadius() + to->BoundingRadius()); }

float ZCamilleExtensions::Dist(IUnit * from, IUnit * to) {
    return (from->ServerPosition() - to->ServerPosition()).Length(); }

float ZCamilleExtensions::Dist2D(IUnit * from, IUnit * to) {
    return (from->ServerPosition() - to->ServerPosition()).Length2D(); }

float ZCamilleExtensions::Dist2D(Vec3 from, Vec3 to) {
    return (from - to).Length2D(); }

float ZCamilleExtensions::Dist2D(Vec2 from, Vec2 to) {
    return (from - to).Length(); }

float ZCamilleExtensions::Dist2D(Vec2 from, Vec3 to) {
    return (from - To2D(to)).Length(); }

bool ZCamilleExtensions::UnderEnemyTurret(Vec3 pos) {
    return GUtility->IsPositionUnderTurret(pos, false, true); }

bool ZCamilleExtensions::UnderEnemyTurret(Vec2 pos) {
    return GUtility->IsPositionUnderTurret(To3D(pos), false, true); }

bool ZCamilleExtensions::UnderEnemyTurret(IUnit * unit) {
    return GUtility->IsPositionUnderTurret(unit->ServerPosition(), false, true); }

bool ZCamilleExtensions::UnderAllyTurret(IUnit * unit) {
    return GUtility->IsPositionUnderTurret(unit->ServerPosition(), true, false); }

bool ZCamilleExtensions::IsKeyDown(IMenuOption * menuOption) {
    return (GetAsyncKeyState(menuOption->GetInteger()) & 0x8000) != 0; }

std::vector<IUnit *> ZCamilleExtensions::GetInRange(IUnit * unit, float range, std::vector<IUnit *> group) {
    std::vector<IUnit *> list;

    if (group.empty()) {
        return list; }

    for (auto o : group) {
        if (o != nullptr && o->IsValidTarget() && Dist2D(o, unit) <= range) {
            list.push_back(o); } }

    return list; }

int ZCamilleExtensions::CountInRange(IUnit * unit, float range, std::vector<IUnit *> units) {
    return GetInRange(unit, range, units).size(); }

ProjectionInfo * ZCamilleExtensions::ProjectOn(Vec2 point, Vec2 start, Vec2 end) {
    auto cx = point.x;
    auto cy = point.y;
    auto ax = start.x;
    auto ay = start.y;
    auto bx = end.x;
    auto by = end.y;
    auto rL = ((cx - ax) * (bx - ax) + (cy - ay) * (by - ay)) / (static_cast<float>(pow(bx - ax, 2)) + static_cast<float>(pow(by - ay, 2)));
    auto pointLine = Vec2(ax + rL * (bx - ax), ay + rL * (by - ay));
    float rS;

    if (rL < 0) {
        rS = 0; }

    else
        if (rL > 1) {
            rS = 1; }

        else {
            rS = rL; }

    auto isOnSegment = rS == rL || isnan(rL);
    auto pointSegment = isOnSegment ? pointLine : Vec2(ax + rS * (bx - ax), ay + rS * (by - ay));
    return new ProjectionInfo(isOnSegment, pointSegment, pointLine); }

bool ZCamilleExtensions::CanCombo() {
    return IsKeyDown(ZCamille::Menu->ComboKey) && ZCamille::Menu->ComboMana->GetInteger() <
           ZCamille::Player->GetMana(); }

bool ZCamilleExtensions::CanHarass() {
    return IsKeyDown(ZCamille::Menu->HarassKey) && ZCamille::Menu->HarassMana->GetInteger() <
           ZCamille::Player->GetMana(); }

bool ZCamilleExtensions::CanJungleClear() {
    bool nearEnemy = CountInRange(GEntityList->Player(), 1000, GEntityList->GetAllHeros(false, true)) > 0;

    if (!IsKeyDown(ZCamille::Menu->JungleClearKey)) {
        return false; }

    return IsKeyDown(ZCamille::Menu->JungleClearKey) && ZCamille::Player->GetMana() > nearEnemy
           ? ZCamille::Menu->WaveClearOverride->GetInteger() : ZCamille::Menu->JungleClearMana->GetInteger(); }

bool ZCamilleExtensions::CanWaveClear() {
    bool nearEnemy = CountInRange(GEntityList->Player(), 1000, GEntityList->GetAllHeros(false, true)) > 0;

    if (!IsKeyDown(ZCamille::Menu->WaveClearKey)) {
        return false; }

    return IsKeyDown(ZCamille::Menu->WaveClearKey) && ZCamille::Player->GetMana() > nearEnemy
           ? ZCamille::Menu->WaveClearOverride->GetInteger() : ZCamille::Menu->WaveClearMana->GetInteger(); }

bool ZCamilleExtensions::CanFlee() {
    return IsKeyDown(ZCamille::Menu->FleeKey); }
