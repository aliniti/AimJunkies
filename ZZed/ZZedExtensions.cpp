#include <string>
#include "ZZed.h"
#include "ZZedExtensions.h"
#include "PluginData.h"

bool ZZedExtensions::IsReady(ISpell * spell, float time) {
    auto io = ZZed::Player->GetSpellRemainingCooldown(spell->GetSpellSlot());
    return io <= time || spell->IsReady(); }

bool ZZedExtensions::IsReady(ISpell2 * spell, float time) {
    auto io = ZZed::Player->GetSpellRemainingCooldown(spell->GetSlot());
    return io <= time || spell->IsReady(); }

bool ZZedExtensions::IsValid(Vec3 p) {
    return p.x != 0 && p.z != 0; }

bool ZZedExtensions::IsValid(Vec2 p) {
    return p.x != 0 && p.y != 0; }

Vec2 ZZedExtensions::To2D(Vec3 p) {
    return Vec2(p.x, p.z); }

Vec3 ZZedExtensions::To3D(Vec2 p) {
    return Vec3(p.x, 0, p.y); }

Vec2 ZZedExtensions::Perp(Vec2 v) {
    return Vec2(-v.y, v.x); }

Vec3 ZZedExtensions::Perp(Vec3 v) {
    return Vec3(-v.y, 0, v.x); }

float ZZedExtensions::Dist(IUnit * to) {
    return (ZZed::Player->ServerPosition() - to->ServerPosition()).Length(); }

float ZZedExtensions::Dist2D(IUnit * to) {
    return (ZZed::Player->ServerPosition() - to->ServerPosition()).Length2D(); }

float ZZedExtensions::Dist(IUnit * from, IUnit * to) {
    return (from->ServerPosition() - to->ServerPosition()).Length(); }

float ZZedExtensions::Dist2D(IUnit * from, IUnit * to) {
    return (from->ServerPosition() - to->ServerPosition()).Length2D(); }

float ZZedExtensions::Dist2D(Vec3 from, Vec3 to) {
    return (from - to).Length2D(); }

float ZZedExtensions::Dist2D(Vec2 from, Vec2 to) {
    return (from - to).Length(); }

float ZZedExtensions::Dist2D(Vec2 from, Vec3 to) {
    return (from - To2D(to)).Length(); }

bool ZZedExtensions::UnderEnemyTurret(Vec3 pos) {
    return GUtility->IsPositionUnderTurret(pos, false, true); }

bool ZZedExtensions::UnderEnemyTurret(Vec2 pos) {
    return GUtility->IsPositionUnderTurret(To3D(pos), false, true); }

bool ZZedExtensions::UnderEnemyTurret(IUnit * unit) {
    return GUtility->IsPositionUnderTurret(unit->ServerPosition(), false, true); }

bool ZZedExtensions::UnderAllyTurret(IUnit * unit) {
    return GUtility->IsPositionUnderTurret(unit->ServerPosition(), true, false); }

bool ZZedExtensions::IsInEnemyFountain(Vec3 pos) {
    return GUtility->IsPositionInFountain(pos, false, true); }

bool ZZedExtensions::IsInAllyFountain(Vec3 pos) {
    return GUtility->IsPositionInFountain(pos, true, false); }

bool ZZedExtensions::IsKeyDown(IMenuOption * menuOption) {
    return (GetAsyncKeyState(menuOption->GetInteger()) & 0x8000) != 0; }

std::vector<Vec2> ZZedExtensions::CircleCircleIntersection(Vec2 center1, Vec2 center2, float radius1, float radius2) {
    auto D = Dist2D(center1, center2);

    //The Circles dont intersect:
    if(D > radius1 + radius2 || (D <= std::abs(radius1 - radius2))) {
        return std::vector<Vec2>(); }

    auto A = (radius1 * radius1 - radius2 * radius2 + D * D) / (2 * D);
    auto H = static_cast<float>(std::sqrt(radius1 * radius1 - A * A));
    auto Direction = (center2 - center1).VectorNormalize();
    auto PA = center1 + A * Direction;
    auto S1 = PA + H * Perp(Direction);
    auto S2 = PA - H * Perp(Direction);
    return std::vector<Vec2>({ S1, S2 }); }

// credits: @rembrandt
void ZZedExtensions::DrawDamageOnChampionHPBar(IUnit * unit, double damage, const char * text, Vec4 color) {
    Vec2 barpos;

    if(unit->GetHPBarPosition(barpos)) {
        Vec2 barsize = Vec2(103 * (min(damage, unit->GetHealth()) / unit->GetMaxHealth()), 8);
        barpos = Vec2(barpos.x + 10, barpos.y += 20);

        Vec2 xd1 = Vec2(barpos.x + barsize.x, barpos.y);
        Vec2 xd2 = Vec2(barpos.x + barsize.x - 5, barpos.y - 7);

        GRender->DrawFilledBox(barpos, barsize, color);
        GRender->DrawLine(xd1, xd2, Vec4(255, 255, 255, 255));
        GRender->DrawLine(xd1, xd1 + Vec2(0, 8), Vec4(255, 255, 255, 255));
        GRender->DrawTextW(xd2 - Vec2(13, 13), Vec4(255, 255, 255, 255), text); } }

// credits: @rembrandt
void ZZedExtensions::DrawDamageOnChampionHPBar(IUnit * unit, double damage, Vec4 color) {
    Vec2 barpos;

    if(unit->GetHPBarPosition(barpos)) {
        Vec2 barsize = Vec2(103 * (min(damage, unit->GetHealth()) / unit->GetMaxHealth()), 8);
        barpos = Vec2(barpos.x + 10, barpos.y += 20);

        Vec2 xd1 = Vec2(barpos.x + barsize.x, barpos.y);
        Vec2 xd2 = Vec2(barpos.x + barsize.x - 5, barpos.y - 7);

        GRender->DrawFilledBox(barpos, barsize, color); } }

// credits @rembrandt
double ZZedExtensions::AmplifyDamage(IUnit * source, IUnit * target)  {
    double modifier = 1;
    std::vector<HeroMastery> masteryPtr;

    if(source->GetMasteries(masteryPtr)) {
        for(auto mastery : masteryPtr) {
            if(mastery.MasteryId == 40) {
                modifier += (0.4 * mastery.Points) / 100; }
            else if(mastery.MasteryId == 12) {
                modifier += 0.03; }
            else if(mastery.MasteryId == 186) {
                bool active = true;

                for(auto ally : GEntityList->GetAllHeros(true, false)) {
                    if(ally != source && (source->GetPosition() - ally->GetPosition()).Length2D() <= 800) {
                        active = false;
                        break; } }

                if(active) {
                    modifier += 0.02; } }
            else if(mastery.MasteryId == 232) {
                if(target->HealthPercent() < 40) {
                    modifier += (0.6 * mastery.Points) / 100; } } } }

    std::vector<HeroMastery> enemymasteryPtr;

    if(target->GetMasteries(enemymasteryPtr)) {
        for(auto mastery : enemymasteryPtr) {

            if(mastery.MasteryId == 12) {
                modifier += 0.015;
                break; } } }

    return modifier; }

std::vector<IUnit *> ZZedExtensions::GetInRange(Vec2 pos, float range, std::vector<IUnit *> group) {
    std::vector<IUnit *> list;

    if(group.empty()) {
        return list; }

    for(auto o : group) {
        if(o != nullptr && o->IsValidTarget() && Dist2D(pos, o->ServerPosition()) <= range) {
            list.push_back(o); } }

    return list; }

int ZZedExtensions::CountInRange(IUnit * unit, float range, std::vector<IUnit *> units) {
    return GetInRange(To2D(unit->ServerPosition()), range, units).size(); }

int ZZedExtensions::CountInRange(Vec2 pos, float range, std::vector<IUnit *> units) {
    return GetInRange(pos, range, units).size(); }

ProjectionInfo * ZZedExtensions::ProjectOn(Vec2 point, Vec2 start, Vec2 end) {
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

    else if(rL > 1) {
        rS = 1; }

    else {
        rS = rL; }

    auto isOnSegment = rS == rL || isnan(rL);
    auto pointSegment = isOnSegment ? pointLine : Vec2(ax + rS * (bx - ax), ay + rS * (by - ay));
    return new ProjectionInfo(isOnSegment, pointSegment, pointLine); }