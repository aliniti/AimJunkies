#include "ZZedModes.h"
#include "PluginSDK.h"
#include "ZZed.h"
#include <string>

void ZZedModes::Combo() {

    auto ultunit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::Menu->AssassinRange->GetInteger());
    auto mainunit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::W->GetSpellRange() * 2);
    auto canr = ZZed::Menu->UseComboR->Enabled() && ZZed::R->IsReady();
    auto engage = false;

    ZZed::CanUlt(ultunit, engage);
    ZZed::UseR(ultunit, engage);

    if (!engage || !canr || ZZed::Marked.size() > 0) {
        ZZed::UseW(mainunit, false);
        ZZed::UseE(mainunit, false);
        ZZed::UseQ(mainunit, false); } }

void ZZedModes::Harass() {

    auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::W->GetSpellRange() * 2);

    ZZed::UseW(unit, true);
    ZZed::UseE(unit, true);
    ZZed::UseQ(unit, true); }

void ZZedModes::Flee() {

    GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());

    if (ZZed::W->IsReady() && ZZed::Menu->UseFleeW->Enabled()) {
        ZZed::W->CastOnPosition(GGame->CursorPosition()); } }

void ZZedModes::WaveClear() {
    auto menu = ZZed::Menu;
    auto player = ZZed::Player;
    auto utils = ZZed::Ex;

    if (menu->LastHitQ->Enabled() && menu->LastHitQCount->GetInteger() > 1) {

        //if (ZZed::Q->IsReady()) {
        //    double energy = 0;

        //    Vec3 castposition;
        //    std::vector<IUnit *> v;

        //    GPrediction->FindBestCastPositionEx(player->ServerPosition(), 0.25, ZZed::Q->Range(), ZZed::Q->Radius(), true, true, false, castposition, v);

        //    v.erase(std::remove_if(v.begin(), v.end(), [&](IUnit * u) { return ZZed::QDmg(player, u, energy) < u->GetHealth(); }));
        //    v.erase(std::remove_if(v.begin(), v.end(), [&](IUnit * u) { return !u->IsValidTarget() || u->IsDead(); }));

        //    if (v.size() > 0) {

        //        if (v.size() >= menu->LastHitQCount->GetInteger()) {
        //            if (utils->UnderAllyTurret(player) && player->GetMana() >= menu->MinimumLastHitEnergyTower->GetInteger()) {
        //                ZZed::Q->CastOnPosition(castposition); }

        //            if (utils->UnderAllyTurret(player) == false && player->GetMana() >= menu->MinimumLastHitEnergy->GetInteger()) {
        //                ZZed::Q->CastOnPosition(castposition); } } } }
    }

    if (menu->LastHitE->Enabled() && menu->LastHitECount->GetInteger() > 1) {
        double energy = 0;
        auto v = GEntityList->GetAllMinions(false, true, false);

        v.erase(std::remove_if(v.begin(), v.end(), [&](IUnit * u) {
            return ZZed::EDmg(u, energy) < u->GetHealth() || !u->IsValidTarget() || u->IsDead();; }));

        if (utils->CountInRange(player, ZZed::E->GetSpellRange(), v) >= menu->LastHitECount->GetInteger()) {
            if (ZZed::Ex->UnderEnemyTurret(player) == false) {
                ZZed::E->CastOnPlayer(); } } }

    if (menu->LastHitEUnderTower->Enabled()) {
        double energy = 0;

        if (utils->UnderAllyTurret(player)) {
            for (auto m : GEntityList->GetAllMinions(false, true, false)) {
                if (ZZed::EDmg(m, energy) >= m->GetHealth()) {
                    ZZed::UseEEx(m, false); } } } }

    if (menu->LastHitQUnderTower->Enabled()) {
        double energy = 0;

        if (utils->UnderAllyTurret(player)) {
            for (auto m : GEntityList->GetAllMinions(false, true, false)) {
                if (ZZed::QDmg(player, m, energy) >= m->GetHealth()) {
                    ZZed::UseQEx(m, false); } } } } }

void ZZedModes::Jungling() {
    auto menu = ZZed::Menu;
    auto player = ZZed::Player;
    auto exten = ZZed::Ex;

    if (player->GetMana() > menu->MinimumClearEnergy->GetInteger()) {
        auto mobs = GEntityList->GetAllMinions(false, true, true);
        std::sort(mobs.begin(), mobs.end(), [&](IUnit * a, IUnit * b) { return ZZed::GetPriorityJungleTarget(a, b); });

        for (auto unit : mobs) {
            if (unit->IsJungleCreep() && exten->Dist(unit) <= 655) {
                if (unit->IsValidObject() && !unit->IsDead() && unit->IsVisible()) {
                    if (!menu->DontWJungleNearEnemy->Enabled() || exten->CountInRange(player, 1000, GEntityList->GetAllHeros(false, true)) < 1) {
                        if (menu->UseJungleW->Enabled()) {
                            ZZed::UseWEx(unit, true); } }

                    if (menu->UseJungleE->Enabled()) {
                        ZZed::UseEEx(unit, true); }

                    if (menu->UseJungleQ->Enabled()) {
                        ZZed::UseQEx(unit, true); } } } } } }

void ZZedModes::OnUpdate() {

    // iterate marked targets
    for (auto i : ZZed::Marked) {
        auto key = i.first;
        auto unit = i.second;

        // remove invalid marked units
        if (unit->IsDead() || unit == nullptr || !unit->IsValidTarget()) {
            ZZed::Marked.erase(key);
            break; }

        // removed marked targets after 6 seconds
        if (GGame->Time() - key > 6) {
            ZZed::Marked.erase(key);
            break; } }

    // iterate shadow positions
    for (auto i : ZZed::WPositions) {
        auto timestamp = i.first;

        // remove shadows after casts
        if (GGame->Time() - timestamp - ZZed::Q->GetDelay() > 1 + GGame->Latency() / 1000) {
            ZZed::WPositions.erase(timestamp);
            break; } }

    // iterate shadows
    for (auto i : ZZed::Shadows) {
        auto key = i.first;
        auto shadow = i.second;

        std::vector<void *> shadowBuffsPtr;
        shadow->GetAllBuffsData(shadowBuffsPtr);

        // remove invalid shadows
        if (shadow == nullptr
            || shadow->IsValidObject() == false
            || shadow->IsVisible() == false) {
            ZZed::Shadows.erase(key);
            break; }

        // remove shadows that aren't mine
        for (auto io : shadowBuffsPtr) {
            if (GBuffData->GetCaster(io)->GetNetworkId() != ZZed::Player->GetNetworkId()) {
                ZZed::Shadows.erase(key);
                break; } }

        // remove shadows after 10 seconds
        if (GGame->Time() - key > 10) {
            ZZed::Shadows.erase(key);
            break; } }

    if (GGame->IsChatOpen() || !GUtility->IsLeagueWindowFocused()) {
        return; }

    ZZed::Modes->Auto();

    if (ZZed::Ex->IsKeyDown(ZZed::Menu->ComboKey)) {
        ZZed::Modes->Combo(); }

    if (ZZed::Ex->IsKeyDown(ZZed::Menu->HarassKey)) {
        ZZed::Modes->Harass(); }

    if (ZZed::Ex->IsKeyDown(ZZed::Menu->ClearKey)) {
        ZZed::Modes->WaveClear();
        ZZed::Modes->Jungling(); }

    if (ZZed::Ex->IsKeyDown(ZZed::Menu->FleeKey)) {
        ZZed::Modes->Flee(); } }

void ZZedModes::OnSpellCast(const CastedSpell & args) {
    auto & player = ZZed::Player;
    auto & stamps = ZZed::Ticks;

    if (args.Caster_->GetNetworkId() == player->GetNetworkId()) {
        auto name = std::string(args.Name_);

        if (stamps.find(name) != stamps.end()) {
            stamps[name] = GGame->TickCount(); }

        // store shadow position temporarily for slightly faster combo.
        // slightly faster than waiting for on object create (usefull for zed W->E)
        if (name == "ZedW") {

            auto end = args.EndPosition_;
            auto start = args.Caster_->ServerPosition();

            auto dir = (end - start).VectorNormalize();

            if (ZZed::Ex->Dist2D(start, end) > ZZed::W->GetSpellRange()) {
                end = start + dir * ZZed::W->GetSpellRange(); }

            if (ZZed::Ex->Dist2D(args.Caster_->ServerPosition(), end) < 350) {
                if (!ZZed::WShadowExists(true)) {
                    end = start + dir * 350; } }

            ZZed::WPositions[GGame->Time()] = end; } }

    // recycle op :^)
    if (args.Caster_ != nullptr) {
        auto ex = ZZed::Ex;

        if (args.Caster_->IsHero() && args.Caster_->GetTeam() != ZZed::Player->GetTeam()) {
            auto bestTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::R->GetSpellRange());
            auto name = args.Name_;
            auto size = strlen(name);
            auto nameToLower = static_cast<char *>(malloc(size + 1));
            nameToLower[size] = 0;

            for (auto i = 0; i < size; i++) {
                nameToLower[i] = tolower(name[i]); }

            if (bestTarget == nullptr) {
                return; }

            if (ZZed::R->IsReady() && ZZed::Menu->UseRAvoider->Enabled()) {
                auto rrange = ZZed::R->GetSpellRange();

                for (auto entry : ZZed::AvoidList) {
                    if (strcmp(entry.first.c_str(), nameToLower) == 0) {
                        if (ZZed::Menu->SpellsToAvoid.at(nameToLower)->Enabled()) {
                            if (entry.second->eType == Targeted) {
                                if (args.Target_ != nullptr && args.Target_->GetNetworkId() == player->GetNetworkId()) {
                                    if (!ZZed::RShadowExists()) {
                                        ZZed::R->CastOnUnit(bestTarget); } }

                                if (entry.second->eType == SelfCast) {
                                    if (args.Target_ != nullptr && ex->Dist2D(args.Target_) <= entry.second->Radius) {
                                        if (!ZZed::RShadowExists()) {
                                            ZZed::R->CastOnUnit(bestTarget); } } }

                                if (entry.second->eType == SkillshotCircle) {
                                    auto cStart = ex->To2D(args.Position_);
                                    auto cEnd = ex->To2D(args.EndPosition_);

                                    if (ex->Dist2D(cStart, cEnd) > ZZed::R->GetSpellRange()) {
                                        cEnd = cStart + (cEnd - cStart).VectorNormalize() * rrange; }

                                    if (ex->Dist2D(cEnd, player->ServerPosition()) <= rrange) {
                                        if (!ZZed::RShadowExists()) {
                                            ZZed::R->CastOnUnit(bestTarget); } } } } } } } } } } }

void ZZedModes::OnRender() {
    auto menu = ZZed::Menu;
    auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::W->GetSpellRange() * 2);
    auto xunit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::Menu->AssassinRange->GetInteger());

    if (ZZed::Player->IsDead()) {
        return; }

    if (menu->DrawAssassinRange->Enabled()) {
        Vec4 aa;
        bool engage;
        ZZed::CanUlt(xunit, engage);
        menu->DrawAssassinRangeColor->GetColor(&aa);

        if (menu->UseComboR->Enabled() && ZZed::R->IsReady() && engage) {
            GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), aa, menu->AssassinRange->GetInteger()); } }

    if (menu->DrawQ->Enabled()) {
        Vec4 qq;
        menu->DrawQColor->GetColor(&qq);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), qq, ZZed::Q->Range()); }

    if (menu->DrawE->Enabled()) {
        Vec4 ee;
        menu->DrawEColor->GetColor(&ee);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), ee, ZZed::E->GetSpellRange()); }

    if (menu->DrawW->Enabled()) {
        Vec4 ww;
        menu->DrawWColor->GetColor(&ww);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), ww, ZZed::W->GetSpellRange()); }

    if (menu->DrawR->Enabled()) {
        Vec4 rr;
        menu->DrawRColor->GetColor(&rr);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), rr, ZZed::R->GetSpellRange()); }

    if (menu->DrawPredictedShadow->Enabled() && unit != nullptr) {
        Vec3 wpos;
        ZZed::GetBestWPosition(unit, wpos, false, true);
        GRender->DrawCircle(wpos, ZZed::Player->BoundingRadius(), Vec4(255, 0, 0, 255), 5, false, true); }

    if (menu->DrawComboDamage->Enabled()) {
        Vec4 bb;
        double energy;
        menu->DrawComboDamageColor->GetColor(&bb);

        for (auto i : GEntityList->GetAllHeros(false, true)) {
            if (i->IsOnScreen() && i->IsVisible() && i->IsValidTarget()) {
                menu->Debug->Enabled()
                ? ZZed::Ex->DrawDamageOnChampionHPBar(i, ZZed::CDmg(i, energy), std::to_string(ZZed::CDmg(i, energy)).c_str(), bb)
                : ZZed::Ex->DrawDamageOnChampionHPBar(i, ZZed::CDmg(i, energy), bb); } } }

    for (auto u : ZZed::Shadows) {
        auto shadow = u.second;

        GRender->DrawCircle(shadow->GetPosition(), shadow->BoundingRadius() + 35, Vec4(102, 204, 204, 200), 9, false, true); } }

void ZZedModes::OnCreateObj(IUnit * source) {
    if (source != nullptr && source->GetClassId() == kobj_GeneralParticleEmitter) {
        if (strcmp(source->GetObjectName(), "Zed_Base_R_buf_tell.troy") == 0) {
            ZZed::Ticks["Lethal"] = GGame->TickCount();

            if (ZZed::CanSwap(ZZed::R) && ZZed::Menu->SwapRIfDead->Enabled()) {
                if (ZZed::Ex->UnderEnemyTurret(ZZed::RShadow()) == false &&
                    ZZed::Ex->UnderEnemyTurret(ZZed::Player)) {
                    ZZed::R->CastOnPlayer();
                    return; }

                if (ZZed::Ex->Dist2D(ZZed::RShadow()) > ZZed::W->GetSpellRange()) {
                    ZZed::R->CastOnPlayer();
                    return; } }

            // sometimes its not the best idea to always swap back to your ult shadow everytime.
            // if the above conditions weren't met sometimes a user wasnt sure if the target is dead from the mark.
            // the death mark icon also isn't always noticeable in the heat of the moment.

            // this is not even for the bm imo.
            // it just signafies if the target is dead (before heals/buffs)
            if (ZZed::Menu->LaughIfDead->Enabled()) {
                GGame->Say("/l"); } } }

    if (source != nullptr && source->GetClassId() == kobj_AI_Minion) {
        if (source->GetTeam() == ZZed::Player->GetTeam()) {
            if (strcmp(source->GetObjectName(), "Shadow") == 0) {
                ZZed::Shadows[GGame->Time()] = source; } } } }

void ZZedModes::OnBuffAdd(IUnit * unit, void * buffdata) {
    auto player = ZZed::Player;

    if (unit->GetTeam() != player->GetTeam()) {
        if (strcmp(GBuffData->GetBuffName(buffdata), "zedrdeathmark") == 0) {
            ZZed::Marked[GGame->Time()] = unit; } } }

void ZZedModes::OnDoCast(const CastedSpell & args) {
    if (args.Caster_->GetNetworkId() == ZZed::Player->GetNetworkId()) {
        if (args.AutoAttack_) {
            if (args.Target_ != nullptr && args.Target_->IsValidTarget() && !args.Target_->IsDead()) {
                if (args.Target_->IsHero() && (!ZZed::Ex->UnderEnemyTurret(args.Target_) || ZZed::Ex->IsKeyDown(ZZed::Menu->ComboKey))) {
                    if (ZZed::Tiamat->IsOwned() && ZZed::Tiamat->IsReady()) {
                        GPluginSDK->DelayFunctionCall(100 + GGame->Latency(), [&]() { ZZed::Tiamat->CastOnPlayer(); }); }

                    if (ZZed::Hydra->IsOwned() && ZZed::Hydra->IsReady()) {
                        GPluginSDK->DelayFunctionCall(100 + GGame->Latency(), [&]() { ZZed::Hydra->CastOnPlayer(); }); }

                    if (ZZed::Titanic->IsOwned() && ZZed::Titanic->IsReady()) {
                        GPluginSDK->DelayFunctionCall(100 + GGame->Latency(), [&]() { ZZed::Titanic->CastOnPlayer(); }); } } } } } }

void ZZedModes::Auto() {

    auto engage = false;
    auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::W->GetSpellRange() * 2);

    ZZed::CanUlt(unit, engage);

    if (ZZed::Menu->AutoR->Enabled()) {
        ZZed::UseR(unit, engage, true); }

    // auto e on enemies
    if (ZZed::E->IsReady() && ZZed::Menu->AutoEUnitInRage->Enabled() && !engage
        && ZZed::Player->GetMana() > ZZed::Menu->AutoEUnitInRagePct->GetInteger()) {

        if (ZZed::Ex->CountInRange(ZZed::Player, ZZed::E->GetSpellRange() + ZZed::Player->BoundingRadius(), GEntityList->GetAllHeros(false, true)) > 0) {
            if (ZZed::Ex->UnderEnemyTurret(ZZed::Player) == false) {
                ZZed::E->CastOnPlayer(); } }

        for (auto entry : ZZed::Shadows) {
            auto shadow = entry.second;

            if (ZZed::Ex->CountInRange(shadow, ZZed::E->GetSpellRange() + ZZed::Player->BoundingRadius(), GEntityList->GetAllHeros(false, true)) > 0) {
                if (ZZed::Ex->UnderEnemyTurret(ZZed::Player) == false) {
                    ZZed::E->CastOnPlayer(); } } } } }

void ZZedModes::OnNonKillableMinion(IUnit * unit) {

    if (ZZed::Menu->LastHitE->Enabled() && ZZed::E->IsReady()) {
        double energy = 0;

        if (ZZed::Menu->LastHitEUnkillable->Enabled() && ZZed::EDmg(unit, energy) >= unit->GetHealth()) {
            if (ZZed::Ex->IsKeyDown(ZZed::Menu->ClearKey)) {
                if (ZZed::Ex->UnderEnemyTurret(ZZed::Player) == false) {
                    ZZed::UseEEx(unit, false); } } } }

    else if (ZZed::Menu->LastHitQ->Enabled() && ZZed::Q->IsReady()) {
        double energy = 0;

        if (ZZed::Menu->LastHitQUnkillable->Enabled() && ZZed::QDmg(ZZed::Player, unit, energy) >= unit->GetHealth()) {
            if (ZZed::Ex->IsKeyDown(ZZed::Menu->ClearKey) && (ZZed::Ex->Dist2D(unit) > ZZed::E->GetSpellRange()
                    || !ZZed::E->IsReady()
                    || !ZZed::Menu->LastHitEUnkillable->Enabled())) {
                if (ZZed::Ex->UnderEnemyTurret(ZZed::Player) == false) {
                    ZZed::UseQEx(unit, false); } } } } }

