#pragma once
#include "PluginSDK.h"

class ZCamilleModes {
    public:

        // my modes
        void OnCastSpell(CastedSpell const & args);
        void OnUpdate();
        void Combo();
        void Harass();
        void Flee();
        void JungleClear();
        void WaveClear();
        void OnRender();
        void OnCreateObj(IUnit * source);
        void OnDoCast(CastedSpell const & args);
        bool OnIssueOrder(IUnit * source, DWORD orderIdx, Vec3 position, IUnit * target); };

