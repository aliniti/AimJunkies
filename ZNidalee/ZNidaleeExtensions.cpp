#include <string>
#include "ZNidalee.h"
#include "ZNidaleeExtensions.h"
#include "PluginData.h"

bool ZNidaleeExtensions::IsValid(Vec3 p) {
    return p.x != 0 && p.z != 0; }

bool ZNidaleeExtensions::IsValid(Vec2 p) {
    return p.x != 0 && p.y != 0; }

Vec2 ZNidaleeExtensions::To2D(Vec3 p) {
    return Vec2(p.x, p.z); }

Vec3 ZNidaleeExtensions::To3D(Vec2 p) {
    return Vec3(p.x, 0, p.y); }

float ZNidaleeExtensions::Dist(IUnit * to) {
    return (ZNidalee::Player->ServerPosition() - to->ServerPosition()).Length() -
           (ZNidalee::Player->BoundingRadius() + to->BoundingRadius()); }

float ZNidaleeExtensions::Dist2D(IUnit * to) {
    return (ZNidalee::Player->ServerPosition() - to->ServerPosition()).Length2D() -
           (ZNidalee::Player->BoundingRadius() + to->BoundingRadius()); }

float ZNidaleeExtensions::Dist(IUnit * from, IUnit * to) {
    return (from->ServerPosition() - to->ServerPosition()).Length(); }

float ZNidaleeExtensions::Dist2D(IUnit * from, IUnit * to) {
    return (from->ServerPosition() - to->ServerPosition()).Length2D(); }

float ZNidaleeExtensions::Dist2D(Vec3 from, Vec3 to) {
    return (from - to).Length2D(); }

float ZNidaleeExtensions::Dist2D(Vec2 from, Vec2 to) {
    return (from - to).Length(); }

float ZNidaleeExtensions::Dist2D(Vec2 from, Vec3 to) {
    return (from - To2D(to)).Length(); }

bool ZNidaleeExtensions::UnderEnemyTurret(Vec3 pos) {
    return GUtility->IsPositionUnderTurret(pos, false, true); }

bool ZNidaleeExtensions::UnderEnemyTurret(Vec2 pos) {
    return GUtility->IsPositionUnderTurret(To3D(pos), false, true); }

bool ZNidaleeExtensions::UnderEnemyTurret(IUnit * unit) {
    return GUtility->IsPositionUnderTurret(unit->ServerPosition(), false, true); }

bool ZNidaleeExtensions::UnderAllyTurret(IUnit * unit) {
    return GUtility->IsPositionUnderTurret(unit->ServerPosition(), true, false); }

bool ZNidaleeExtensions::IsKeyDown(IMenuOption * menuOption) {
    return (GetAsyncKeyState(menuOption->GetInteger()) & 0x8000) != 0; }

std::vector<IUnit *> ZNidaleeExtensions::GetInRange(IUnit * unit, float range, std::vector<IUnit *> group) {
    std::vector<IUnit *> list;

    if(group.empty()) {
        return list; }

    for(auto o : group) {
        if(o != nullptr && o->IsValidTarget() && Dist2D(o, unit) <= range) {
            list.push_back(o); } }

    return list; }

int ZNidaleeExtensions::CountInRange(IUnit * unit, float range, std::vector<IUnit *> units) {
    return GetInRange(unit, range, units).size(); }

ProjectionInfo * ZNidaleeExtensions::ProjectOn(Vec2 point, Vec2 start, Vec2 end) {
    auto cx = point.x;
    auto cy = point.y;
    auto ax = start.x;
    auto ay = start.y;
    auto bx = end.x;
    auto by = end.y;
    auto rL = ((cx - ax) * (bx - ax) + (cy - ay) * (by - ay)) / (static_cast<float>(pow(bx - ax, 2)) + static_cast<float>(pow(by - ay, 2)));
    auto pointLine = Vec2(ax + rL * (bx - ax), ay + rL * (by - ay));
    float rS;

    if(rL < 0) {
        rS = 0; }

    else
        if(rL > 1) {
            rS = 1; }

        else {
            rS = rL; }

    auto isOnSegment = rS == rL || isnan(rL);
    auto pointSegment = isOnSegment ? pointLine : Vec2(ax + rS * (bx - ax), ay + rS * (by - ay));
    return new ProjectionInfo(isOnSegment, pointSegment, pointLine); }