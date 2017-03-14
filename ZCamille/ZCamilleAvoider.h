#pragma once
#include "PluginData.h"

enum EvadeType {
    Targeted,
    SkillshotLine,
    SkillshotCircle,
    SelfCast };

enum AvoidType {
    Inside,
    Outside };

class ZCamilleAvoider {
    public:

        float Radius;
        AvoidType aType;
        EvadeType eType;
        IUnit * Emitter;

        std::string SpellName;
        std::string ChampName;

        ZCamilleAvoider(IUnit * emitter, AvoidType type, float radius): eType() {
            this->aType = type;
            this->Emitter = emitter;
            this->Radius = radius; }

        ZCamilleAvoider(std::string name, EvadeType type, std::string champname): Radius(0), aType(), Emitter(nullptr) {
            this->SpellName = name;
            this->eType = type;
            this->ChampName = champname; }

        static void GenerateAvoidList(); };

