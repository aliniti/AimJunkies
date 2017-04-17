#include "ZZedModes.h"
#include "PluginSDK.h"
#include "ZZed.h"
#include <string>

void ZZedModes::OnUpdate() {

    if(GGame->IsChatOpen() || !GUtility->IsLeagueWindowFocused()) {
        return; }

    if(ZZed::Menu->AutoR->Enabled()) {
        auto beans = false;

        for(auto o : GEntityList->GetAllHeros(false, true)) {
            ZZed::UseR(o, beans); } }

    // auto e on enemies
    if(ZZed::E->IsReady() && ZZed::Menu->AutoEUnitInRage->Enabled()
        && ZZed::Player->GetMana() > ZZed::Menu->AutoEUnitInRagePct->GetInteger()) {

        if(ZZed::Ex->CountInRange(ZZed::Player, ZZed::E->GetSpellRange() + ZZed::Player->BoundingRadius(), GEntityList->GetAllHeros(false, true)) > 0) {
            if(!ZZed::Ex->UnderEnemyTurret(ZZed::Player)) {
                ZZed::E->CastOnPlayer(); } }

        for(auto entry : ZZed::Shadows) {
            auto shadow = entry.second;

            if(ZZed::Ex->CountInRange(shadow, ZZed::E->GetSpellRange() + ZZed::Player->BoundingRadius(), GEntityList->GetAllHeros(false, true)) > 0) {
                if(!ZZed::Ex->UnderEnemyTurret(ZZed::Player)) {
                    ZZed::E->CastOnPlayer(); } } } }

    if(ZZed::Ex->IsKeyDown(ZZed::Menu->ComboKey)) {
        auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::W->GetSpellRange() * 2);
        auto beans = false;

        ZZed::CanUlt(unit, beans);
        ZZed::UseR(unit, beans);

        if(!beans || ZZed::Marked.size() > 0) {

            ZZed::UseW(unit, false);
            ZZed::UseE(unit, false);
            ZZed::UseQ(unit, false); } }


    if(ZZed::Ex->IsKeyDown(ZZed::Menu->HarassKey)) {
        auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::W->GetSpellRange() * 2);

        ZZed::UseW(unit, true);
        ZZed::UseE(unit, true);
        ZZed::UseQ(unit, true); }

    //if(ZZed::Ex->IsKeyDown(ZZed::Menu->ClearKey)) {

    //}

    if(ZZed::Ex->IsKeyDown(ZZed::Menu->FleeKey)) {
        GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());

        if(ZZed::W->IsReady() && ZZed::Menu->UseFleeW->Enabled()) {
            ZZed::W->CastOnPosition(GGame->CursorPosition()); } }

    // itterrate marked targets
    for(auto i : ZZed::Marked) {
        auto key = i.first;
        auto unit = i.second;

        if(unit->IsDead() || unit == nullptr || !unit->IsValidTarget()) {
            ZZed::Marked.erase(key);
            break; }

        if(GGame->Time() - key > 6) {
            ZZed::Marked.erase(key);
            break; } }

    // itterate shadows
    for(auto i : ZZed::Shadows) {
        auto key = i.first;
        auto shadow = i.second;

        std::vector<void *> shadowBuffsPtr;
        shadow->GetAllBuffsData(shadowBuffsPtr);

        // remove invalid shadows
        if(shadow == nullptr
            || shadow->IsValidObject() == false
            || shadow->IsVisible() == false) {
            ZZed::Shadows.erase(key);
            break; }

        // remove shadows that aren't mine
        for(auto io : shadowBuffsPtr) {
            if(GBuffData->GetCaster(io)->GetNetworkId() != ZZed::Player->GetNetworkId()) {
                ZZed::Shadows.erase(key);
                break; } }

        // remove shadows after 10 seconds
        if(GGame->Time() - key > 10) {
            ZZed::Shadows.erase(key);
            break; } } }

void ZZedModes::OnSpellCast(const CastedSpell & args) {
    auto & player = ZZed::Player;
    auto & stamps = ZZed::Ticks;

    if(args.Caster_->GetNetworkId() == player->GetNetworkId()) {
        auto name = std::string(args.Name_);

        if(stamps.find(name) != stamps.end()) {
            stamps[name] = GGame->TickCount(); } } }

void ZZedModes::OnRender() {
    auto menu = ZZed::Menu;
    auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::W->GetSpellRange() * 2);

    if(menu->DrawPredictedShadow->Enabled() && unit != nullptr) {
        Vec3 wpos;
        ZZed::GetBestWPosition(unit, wpos, true);
        GRender->DrawCircle(wpos, ZZed::Player->BoundingRadius(), Vec4(255, 0, 0, 255), 4, false, true); }

    if(menu->DrawComboDamage->Enabled()) {
        Vec4 bb;
        double energy;
        menu->DrawComboDamageColor->GetColor(&bb);

        for(auto i : GEntityList->GetAllHeros(false, true)) {
            if(i->IsOnScreen() && i->IsVisible() && i->IsValidTarget()) {
                menu->Debug->Enabled()
                ? ZZed::Ex->DrawDamageOnChampionHPBar(i, ZZed::CDmg(i, energy), std::to_string(ZZed::CDmg(i, energy)).c_str(), bb)
                : ZZed::Ex->DrawDamageOnChampionHPBar(i, ZZed::CDmg(i, energy), bb); } } }

    for(auto u : ZZed::Shadows) {
        auto shadow = u.second;

        GRender->DrawCircle(shadow->GetPosition(), shadow->BoundingRadius() + 35, Vec4(121, 77, 255, 155)); } }

void ZZedModes::OnCreateObj(IUnit * source) {
    if(source != nullptr && source->GetClassId() == kobj_GeneralParticleEmitter) {
        if(strcmp(source->GetObjectName(), "Zed_Base_R_buf_tell.troy") == 0) {
            if(ZZed::CanSwap(ZZed::R) && ZZed::Menu->SwapRIfDead->Enabled()) {

                if(ZZed::Ex->Dist2D(ZZed::RShadow()) > 550) {
                    ZZed::R->CastOnPlayer(); }

                if(!ZZed::Ex->UnderEnemyTurret(ZZed::RShadow()) &&
                    ZZed::Ex->UnderEnemyTurret(ZZed::Player)) {
                    ZZed::R->CastOnPlayer(); } } } }

    if(source != nullptr && source->GetClassId() == kobj_AI_Minion) {
        if(source->GetTeam() == ZZed::Player->GetTeam()) {
            if(strcmp(source->GetObjectName(), "Shadow") == 0) {
                ZZed::Shadows[GGame->Time()] = source; } } } }

void ZZedModes::OnBuffAdd(IUnit * unit, void * buffdata) {
    auto player = ZZed::Player;

    if(unit->GetTeam() != player->GetTeam()) {
        if(strcmp(GBuffData->GetBuffName(buffdata), "zedrdeathmark") == 0) {
            ZZed::Marked[GGame->Time()] = unit; } } }

void ZZedModes::OnDoCast(const CastedSpell & args) {}

//void ZZedModes::WaveClear() {
//
//}
//
//void ZZedModes::Jungling() {
//    auto ex = ZZed::Ex;
//    auto player = ZZed::Player;
//
//    auto mobs = GEntityList->GetAllMinions(false, true, true);
//    std::sort(mobs.begin(), mobs.end(), [&](IUnit * a, IUnit * b) { return ZZed::GetPriorityJungleTarget(a, b); });
//
//    for(auto unit : mobs) {
//        if(unit->IsJungleCreep()) {
//
//        } }
//
//}