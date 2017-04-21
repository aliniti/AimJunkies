#include "ZZedModes.h"
#include "PluginSDK.h"
#include "ZZed.h"
#include <string>

void ZZedModes::Combo() {

    auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::W->GetSpellRange() * 2);
    auto canr = ZZed::Menu->UseComboR->Enabled() && ZZed::R->IsReady();
    auto beans = false;

    ZZed::CanUlt(unit, beans);
    ZZed::UseR(unit, beans);

    if(!beans || !canr || ZZed::Marked.size() > 0) {
        ZZed::UseW(unit, false);
        ZZed::UseE(unit, false);
        ZZed::UseQ(unit, false); } }

void ZZedModes::Harass() {

    auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::W->GetSpellRange() * 2);

    ZZed::UseW(unit, true);
    ZZed::UseE(unit, true);
    ZZed::UseQ(unit, true); }

void ZZedModes::Flee() {

    GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());

    if(ZZed::W->IsReady() && ZZed::Menu->UseFleeW->Enabled()) {
        ZZed::W->CastOnPosition(GGame->CursorPosition()); } }

void ZZedModes::WaveClear() {

}

void ZZedModes::Jungling() {
    auto menu = ZZed::Menu;
    auto player = ZZed::Player;
    auto exten = ZZed::Ex;

    if(player->GetMana() > menu->MinimumClearEnergy->GetInteger()) {
        auto mobs = GEntityList->GetAllMinions(false, true, true);
        std::sort(mobs.begin(), mobs.end(), [&](IUnit * a, IUnit * b) { return ZZed::GetPriorityJungleTarget(a, b); });

        for(auto unit : mobs) {
            if(unit->IsJungleCreep() && ZZed::Ex->Dist(unit) <= 655) {
                if(unit->IsValidObject() && !unit->IsDead() && unit->IsVisible()) {
                    if(menu->UseJungleWNearEnemy->Enabled() || exten->CountInRange(player, 1000, GEntityList->GetAllHeros(false, true)) < 1) {
                        if(menu->UseJungleW->Enabled()) {
                            ZZed::UseWEx(unit, true); } }

                    if(menu->UseJungleE->Enabled()) {
                        ZZed::UseEEx(unit, true); }

                    if(menu->UseJungleQ->Enabled()) {
                        ZZed::UseQ(unit, false); } } } } } }

void ZZedModes::OnUpdate() {

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
            break; } }

    if(GGame->IsChatOpen() || !GUtility->IsLeagueWindowFocused()) {
        return; }

    ZZed::Modes->Auto();

    if(ZZed::Ex->IsKeyDown(ZZed::Menu->ComboKey)) {
        ZZed::Modes->Combo(); }

    if(ZZed::Ex->IsKeyDown(ZZed::Menu->HarassKey)) {
        ZZed::Modes->Harass(); }

    if(ZZed::Ex->IsKeyDown(ZZed::Menu->ClearKey)) {
        ZZed::Modes->WaveClear();
        ZZed::Modes->Jungling(); }

    if(ZZed::Ex->IsKeyDown(ZZed::Menu->FleeKey)) {
        ZZed::Modes->Flee(); } }

void ZZedModes::OnSpellCast(const CastedSpell & args) {
    auto & player = ZZed::Player;
    auto & stamps = ZZed::Ticks;

    if(args.Caster_->GetNetworkId() == player->GetNetworkId()) {
        auto name = std::string(args.Name_);

        if(strcmp("ZedQ", name.c_str()) == 0) {
            ZZed::Q->SetRangeCheckFrom(ZZed::Player->ServerPosition()); }

        if(stamps.find(name) != stamps.end()) {
            stamps[name] = GGame->TickCount(); } }

    // recycle op :^)
    if(args.Caster_ != nullptr) {
        auto ex = ZZed::Ex;

        if(args.Caster_->IsHero() && args.Caster_->GetTeam() != ZZed::Player->GetTeam()) {
            auto bestTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::R->GetSpellRange());
            auto name = args.Name_;
            auto size = strlen(name);
            auto nameToLower = static_cast<char *>(malloc(size + 1));
            nameToLower[size] = 0;

            for(auto i = 0; i < size; i++) {
                nameToLower[i] = tolower(name[i]); }

            if(bestTarget == nullptr) {
                return; }

            if(ZZed::R->IsReady() && ZZed::Menu->UseRAvoider->Enabled()) {
                auto rrange = ZZed::R->GetSpellRange();

                for(auto entry : ZZed::AvoidList) {
                    if(strcmp(entry.first.c_str(), nameToLower) == 0) {
                        if(ZZed::Menu->SpellsToAvoid.at(nameToLower)->Enabled()) {
                            if(entry.second->eType == Targeted) {
                                if(args.Target_ != nullptr && args.Target_->GetNetworkId() == player->GetNetworkId()) {
                                    if(!ZZed::RShadowExists()) {
                                        ZZed::R->CastOnUnit(bestTarget); } }

                                if(entry.second->eType == SelfCast) {
                                    if(args.Target_ != nullptr && ex->Dist2D(args.Target_) <= entry.second->Radius) {
                                        if(!ZZed::RShadowExists()) {
                                            ZZed::R->CastOnUnit(bestTarget); } } }

                                if(entry.second->eType == SkillshotCircle) {
                                    auto cStart = ex->To2D(args.Position_);
                                    auto cEnd = ex->To2D(args.EndPosition_);

                                    if(ex->Dist2D(cStart, cEnd) > ZZed::R->GetSpellRange()) {
                                        cEnd = cStart + (cEnd - cStart).VectorNormalize() * rrange; }

                                    if(ex->Dist2D(cEnd, player->ServerPosition()) <= rrange) {
                                        if(!ZZed::RShadowExists()) {
                                            ZZed::R->CastOnUnit(bestTarget); } } } } } } } } } } }

void ZZedModes::OnRender() {
    auto menu = ZZed::Menu;
    auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::W->GetSpellRange() * 2);

    if(ZZed::Player->IsDead()) {
        return; }

    if(ZZed::Menu->DrawQ->Enabled()) {
        Vec4 qq;
        ZZed::Menu->DrawQColor->GetColor(&qq);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), qq, ZZed::Q->Range()); }

    if(ZZed::Menu->DrawE->Enabled()) {
        Vec4 ee;
        ZZed::Menu->DrawEColor->GetColor(&ee);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), ee, ZZed::E->GetSpellRange()); }

    if(ZZed::Menu->DrawW->Enabled()) {
        Vec4 ww;
        ZZed::Menu->DrawWColor->GetColor(&ww);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), ww, ZZed::W->GetSpellRange()); }

    if(ZZed::Menu->DrawR->Enabled()) {
        Vec4 rr;
        ZZed::Menu->DrawRColor->GetColor(&rr);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), rr, ZZed::R->GetSpellRange()); }

    if(menu->DrawPredictedShadow->Enabled() && unit != nullptr) {
        Vec3 wpos;
        ZZed::GetBestWPosition(unit, wpos, false, true);
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
        auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::W->GetSpellRange() * 2);

        if(source->GetTeam() == ZZed::Player->GetTeam()) {
            if(strcmp(source->GetObjectName(), "Shadow") == 0) {

                if(unit != nullptr && !unit->IsDead() && unit->IsValidTarget()) {
                    if(ZZed::Ex->Dist2D(unit) > ZZed::Q->Range()) {
                        ZZed::Q->SetRangeCheckFrom(source->ServerPosition()); }
                    else {
                        ZZed::Q->SetRangeCheckFrom(ZZed::Player->ServerPosition()); } }

                ZZed::Shadows[GGame->Time()] = source; } } } }

void ZZedModes::OnBuffAdd(IUnit * unit, void * buffdata) {
    auto player = ZZed::Player;

    if(unit->GetTeam() != player->GetTeam()) {
        if(strcmp(GBuffData->GetBuffName(buffdata), "zedrdeathmark") == 0) {
            ZZed::Marked[GGame->Time()] = unit; } } }

void ZZedModes::OnDoCast(const CastedSpell & args) {
    if(args.Caster_->GetNetworkId() == ZZed::Player->GetNetworkId()) {
        if(args.AutoAttack_) {
            if(args.Target_ != nullptr && args.Target_->IsValidTarget() && !args.Target_->IsDead()) {
                if(args.Target_->IsHero() && (!ZZed::Ex->UnderEnemyTurret(args.Target_) || ZZed::Ex->IsKeyDown(ZZed::Menu->ComboKey))) {
                    if(ZZed::Tiamat->IsOwned() && ZZed::Tiamat->IsReady()) {
                        ZZed::Tiamat->CastOnPlayer(); }

                    if(ZZed::Hydra->IsOwned() && ZZed::Hydra->IsReady()) {
                        ZZed::Hydra->CastOnPlayer(); }

                    if(ZZed::Titanic->IsOwned() && ZZed::Titanic->IsReady()) {
                        ZZed::Titanic->CastOnPlayer(); } } } } } }

void ZZedModes::Auto() {

    auto morebeans = false;
    auto unit = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZZed::W->GetSpellRange() * 2);

    ZZed::CanUlt(unit, morebeans);

    if(ZZed::Menu->AutoR->Enabled()) {
        ZZed::UseR(unit, morebeans, true); }

    // auto e on enemies
    if(ZZed::E->IsReady() && ZZed::Menu->AutoEUnitInRage->Enabled() && !morebeans
        && ZZed::Player->GetMana() > ZZed::Menu->AutoEUnitInRagePct->GetInteger()) {

        if(ZZed::Ex->CountInRange(ZZed::Player, ZZed::E->GetSpellRange() + ZZed::Player->BoundingRadius(), GEntityList->GetAllHeros(false, true)) > 0) {
            if(!ZZed::Ex->UnderEnemyTurret(ZZed::Player)) {
                ZZed::E->CastOnPlayer(); } }

        for(auto entry : ZZed::Shadows) {
            auto shadow = entry.second;

            if(ZZed::Ex->CountInRange(shadow, ZZed::E->GetSpellRange() + ZZed::Player->BoundingRadius(), GEntityList->GetAllHeros(false, true)) > 0) {
                if(!ZZed::Ex->UnderEnemyTurret(ZZed::Player)) {
                    ZZed::E->CastOnPlayer(); } } } } }

