#pragma once
#include "PluginSDK.h"
#include <algorithm>

class ProjectionInfo {
    public:

        bool IsOnSegment;
        Vec2 LinePoint;
        Vec2 SegmentPoint;

        ProjectionInfo(bool isOnSegment, Vec2 segmentPoint, Vec2 linePoint) {
            this->IsOnSegment = isOnSegment;
            this->SegmentPoint = segmentPoint;
            this->LinePoint = linePoint; } };

class ZCamilleExtensions {
    public:
        bool IsValid(Vec3 p);
        bool IsValid(Vec2 p);

        Vec2 To2D(Vec3 p);
        Vec3 To3D(Vec2 p);

        float Dist(IUnit * to);
        float Dist2D(IUnit * to);
        float Dist(IUnit * from, IUnit * to);
        float Dist2D(IUnit * from, IUnit * to);
        float Dist2D(Vec3 from, Vec3 to);
        float Dist2D(Vec2 from, Vec2 to);
        float Dist2D(Vec2 from, Vec3 to);

        bool UnderEnemyTurret(Vec3 pos);
        bool UnderEnemyTurret(Vec2 pos);
        bool UnderEnemyTurret(IUnit * unit);
        bool UnderAllyTurret(IUnit * unit);
        bool IsKeyDown(IMenuOption * menuOption);
        std::vector<IUnit *> GetInRange(IUnit * unit, float range, std::vector<IUnit *> units);
        int CountInRange(IUnit * unit, float range, std::vector<IUnit *> units);
        ProjectionInfo * ProjectOn(Vec2 point, Vec2 star, Vec2 end);

        bool CanCombo();
        bool CanHarass();
        bool CanJungleClear();
        bool CanWaveClear();
        bool CanFlee(); };

struct Action {
    Action(int time, std::function<void()> callback) {
        Time = time + GGame->TickCount();
        CallbackObject = callback; }

    void Call() {
        CallbackObject(); }

    std::function<void()> CallbackObject;
    int Time;

};

struct ActionIUnit {
    ActionIUnit(int time, IUnit * unit, std::function<void(IUnit *)> callback) {
        Time = time + GGame->TickCount();
        CallbackObject = callback;
        Unit = unit; }

    void Call() {
        if (Unit != nullptr) {
            CallbackObject(Unit); } }

    std::function<void(IUnit *)> CallbackObject;
    int Time;
    IUnit * Unit; };

struct ActionPosition {
    ActionPosition(int time, Vec3 position, std::function<void(Vec3)> callback) {
        Time = time + GGame->TickCount();
        CallbackObject = callback;
        Position = position; }

    void Call() {
        CallbackObject(Position); }

    std::function<void(Vec3)> CallbackObject;
    int Time;
    Vec3 Position; };

class DelayAction {
    public:
        void OnGameUpdate() {
            Actions.erase(std::remove_if(Actions.begin(), Actions.end(), [](Action & Args) {
                if (GGame->TickCount() >= Args.Time) {
                    Args.Call();
                    return true; }

                return false; }), Actions.end());
            ActionsIunit.erase(std::remove_if(ActionsIunit.begin(), ActionsIunit.end(), [](ActionIUnit & Args) {
                if (GGame->TickCount() >= Args.Time) {
                    Args.Call();
                    return true; }

                return false; }), ActionsIunit.end());
            ActionsPosition.erase(std::remove_if(ActionsPosition.begin(), ActionsPosition.end(), [](ActionPosition & Args) {
                if (GGame->TickCount() >= Args.Time) {
                    Args.Call();
                    return true; }

                return false; }), ActionsPosition.end()); }

        void Add(int Time, std::function<void()> Callback) {
            Actions.emplace_back(Time, Callback); }
        void AddIUnit(int Time, IUnit * Unit, std::function<void(IUnit *)> Callback) {
            ActionsIunit.emplace_back(Time, Unit, Callback); }
        void AddPosition(int Time, Vec3 Position, std::function<void(Vec3)> Callback) {
            ActionsPosition.emplace_back(Time, Position, Callback); }
    private:
        std::vector<Action> Actions;
        std::vector<ActionIUnit> ActionsIunit;
        std::vector<ActionPosition> ActionsPosition; };
