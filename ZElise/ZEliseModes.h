#pragma once
#include "PluginSDK.h"

class ZEliseModes {
    public:
        void OnUpdate();
        void OnSpellCast(CastedSpell & args);
        void OnSpellCastDelayed(CastedSpell & args);
        void OnPlayAnimation(IUnit * unit, const std::string & cs);
        void OnRender(); };


