#pragma once
#include "PluginData.h"


class ZNidaleeModes {
    public:
        void OnUpdate();
        void OnSpellCast(CastedSpell & args);
        void OnSpellCastDelayed(CastedSpell & args);
        void OnRender();
        void OnGapCloser(const GapCloserSpell & args); };

