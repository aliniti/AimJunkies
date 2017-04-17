#pragma once

#include "PluginData.h"

class ZZedModes {
    public:
        void OnUpdate();
        //void WaveClear();
        //void Jungling();
        void OnSpellCast(const CastedSpell & args);
        void OnRender();
        void OnCreateObj(IUnit * unit);
        void OnBuffAdd(IUnit * unit, void * buffdata);
        void OnDoCast(const CastedSpell & args); };


