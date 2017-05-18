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

class ZNidaleeExtensions {
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
        ProjectionInfo * ProjectOn(Vec2 point, Vec2 star, Vec2 end); };

struct Action {
    Action(int time, std::function<void()> callback) {
        Time = time + GGame->TickCount();
        CallbackObject = callback; }

    void Call() {
        CallbackObject(); }

    std::function<void()> CallbackObject;
    int Time; };

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

struct ActionCastedSpell {
    ActionCastedSpell(int time, CastedSpell spell, std::function<void(CastedSpell)> callback) {
        Time = time + GGame->TickCount();
        CallbackObject = callback;
        Spell = spell; }

    void Call() {
        CallbackObject(Spell); }

    std::function<void(CastedSpell)> CallbackObject;
    int Time;
    CastedSpell Spell; };

class DelayAction {
    public:
        void OnGameUpdate() {
            Actions.erase(std::remove_if(Actions.begin(), Actions.end(), [](Action & args) {
                if (GGame->TickCount() >= args.Time) {
                    args.Call();
                    return true; }

                return false; }), Actions.end());

            ActionsIUnit.erase(std::remove_if(ActionsIUnit.begin(), ActionsIUnit.end(), [](ActionIUnit & args) {
                if (GGame->TickCount() >= args.Time) {
                    args.Call();
                    return true; }

                return false; }), ActionsIUnit.end());

            ActionsCastedSpell.erase(std::remove_if(ActionsCastedSpell.begin(), ActionsCastedSpell.end(), [](ActionCastedSpell & args) {
                if (GGame->TickCount() >= args.Time) {
                    args.Call();
                    return true; }

                return false; }), ActionsCastedSpell.end());

            ActionsPosition.erase(std::remove_if(ActionsPosition.begin(), ActionsPosition.end(), [](ActionPosition & args) {
                if (GGame->TickCount() >= args.Time) {
                    args.Call();
                    return true; }

                return false; }), ActionsPosition.end()); }

        void Add(int Time, std::function<void()> Callback) {
            Actions.emplace_back(Time, Callback); }
        void AddIUnit(int Time, IUnit * Unit, std::function<void(IUnit *)> Callback) {
            ActionsIUnit.emplace_back(Time, Unit, Callback); }
        void AddCastedSpell(int Time, CastedSpell & spell, std::function<void(CastedSpell &)> Callback) {
            ActionsCastedSpell.emplace_back(Time, spell, Callback); }
        void AddPosition(int Time, Vec3 Position, std::function<void(Vec3)> Callback) {
            ActionsPosition.emplace_back(Time, Position, Callback); }
    private:
        std::vector<Action> Actions;
        std::vector<ActionIUnit> ActionsIUnit;
        std::vector<ActionPosition> ActionsPosition;
        std::vector<ActionCastedSpell> ActionsCastedSpell; };