#include "ZCamille.h"
#include "ZCamilleModes.h"
#include "PluginData.h"
#include "ZCamilleAvoider.h"
#include <string>

void ZCamilleModes::OnCastSpell(CastedSpell const & args) {
    if (args.Caster_ != nullptr) {
        auto ex = ZCamille::Ex;
        auto player = ZCamille::Player;

        if (args.Caster_->IsHero() && args.Caster_->GetTeam() != ZCamille::Player->GetTeam()) {
            auto bestTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZCamille::R->Range());
            auto name = args.Name_;
            auto size = strlen(name);
            auto nameToLower = static_cast<char *>(malloc(size + 1));
            nameToLower[size] = 0;

            for (auto i = 0; i < size; i++) {
                nameToLower[i] = tolower(name[i]); }

            if (bestTarget == nullptr) {
                return; }

            if (ZCamille::R->IsReady() && ZCamille::Menu->UseRAvoider->Enabled()) {
                auto rrange = ZCamille::R->Range();

                for (auto entry : ZCamille::AvoidList) {
                    if (strcmp(entry.first.c_str(), nameToLower) == 0) {
                        if (ZCamille::Menu->SpellsToAvoid.at(nameToLower)->Enabled()) {
                            if (entry.second->eType == Targeted) {
                                if (args.Target_ != nullptr && args.Target_->GetNetworkId() == player->GetNetworkId()) {
                                    ZCamille::UseR(bestTarget, true); } }

                            if (entry.second->eType == SelfCast) {
                                if (args.Target_ != nullptr && ex->Dist2D(args.Target_) <= entry.second->Radius) {
                                    ZCamille::UseR(bestTarget, true); } }

                            if (entry.second->eType == SkillshotCircle) {
                                auto cStart = ex->To2D(args.Position_);
                                auto cEnd = ex->To2D(args.EndPosition_);

                                if (ex->Dist2D(cStart, cEnd) > ZCamille::R->Range()) {
                                    cEnd = cStart + (cEnd - cStart).VectorNormalize() * rrange; }

                                if (ex->Dist2D(cEnd, player->ServerPosition()) <= rrange) {
                                    ZCamille::UseR(bestTarget, true); } }

                            if (entry.second->eType == SkillshotLine) {
                                auto lStart = ex->To2D(args.Position_);
                                auto lEnd = ex->To2D(args.EndPosition_);

                                if (ex->Dist2D(lStart, lEnd) < rrange) {
                                    lEnd = lStart + (lEnd - lStart).VectorNormalize() * (rrange + 55); }

                                if (ex->Dist2D(lStart, lEnd) > rrange) {
                                    lEnd = lStart + (lEnd - lStart).VectorNormalize() * (rrange * 2); }

                                if (ex->ProjectOn(ex->To2D(player->ServerPosition()), lStart, lEnd)->IsOnSegment) {
                                    ZCamille::UseR(bestTarget, true); } } } } } } } } }

void ZCamilleModes::OnUpdate() {
    auto ex = ZCamille::Ex;
    auto menu = ZCamille::Menu;

    // remove invalid danger points
    for (auto entry : ZCamille::DangerPoints) {
        auto key = entry.first;
        auto ultimatum = entry.second->Emitter;

        if (ultimatum->IsValidObject() == false || ultimatum->IsVisible() == false) {
            ZCamille::DangerPoints.erase(key);
            break; }

        auto timestamp = entry.first;

        if (GGame->Time() - timestamp > 4) {
            ZCamille::DangerPoints.erase(key);
            break; } }

    // turn off orbwalk attack while charging to allow movement
    GOrbwalking->SetAttacksAllowed(!ZCamille::ChargingW());

    // force orbwalk lock on tethered target
    if (menu->ForceROrbwalk->Enabled()) {
        for (auto i : GEntityList->GetAllHeros(false, true)) {
            if (i != nullptr && i->HasBuff("camillertether") && i->IsValidTarget()) {
                GTargetSelector->SetOverrideFocusedTarget(i);
                GOrbwalking->SetOverrideTarget(i); } } }

    if (GUtility->IsLeagueWindowFocused() && !GGame->IsChatOpen()) {
        if (ex->IsKeyDown(menu->DontEUnderTurretToggle)) {
            if (!ZCamille::KeyState) {
                if (menu->DontEUnderTurret->Enabled() == false) {
                    menu->DontEUnderTurret->UpdateInteger(1); }

                else {
                    menu->DontEUnderTurret->UpdateInteger(0); }

                ZCamille::KeyState = true; } }

        else {
            ZCamille::KeyState = false; }

        if (ex->CanFlee()) {
            ZCamille::Modes->Flee(); }

        if (!ZCamille::IsDashing() && !ZCamille::OnWall() && !ZCamille::Player->IsDead()) {
            if (ex->CanCombo()) {
                ZCamille::Modes->Combo(); }

            if (ex->CanHarass()) {
                ZCamille::Modes->Harass(); }

            if (ex->CanJungleClear()) {
                ZCamille::Modes->JungleClear(); }

            if (ex->CanWaveClear()) {
                ZCamille::Modes->WaveClear(); } } } }

void ZCamilleModes::Combo() {
    auto menu = ZCamille::Menu; // ZCamilleMenu.h
    auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZCamille::E->IsReady() ? ZCamille::E->Range() * 2 : ZCamille::W->Range());

    if (target != nullptr && target->IsValidTarget()) {
        if (menu->MagnetW->Enabled() && menu->MagnetWCombo->Enabled()) {
            ZCamille::LockW(target); }

        if (menu->ComboW->Enabled() && (!ZCamille::E->IsReady() || !menu->ComboE->Enabled())) {
            ZCamille::UseW(target); }

        if (menu->ComboE->Enabled()) {
            ZCamille::UseE(target->ServerPosition(), true); }

        if (menu->ComboR->Enabled()) {
            ZCamille::UseR(target, false); } } }

void ZCamilleModes::Harass() {
    auto menu = ZCamille::Menu; // ZCamilleMenu.h
    auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZCamille::W->Range());

    if (target != nullptr && target->IsValidTarget()) {
        if (menu->MagnetW->Enabled()) {
            if (menu->MagnetWHarass->Enabled()) {
                ZCamille::LockW(target); } }

        if (menu->HarassW->Enabled()) {
            ZCamille::UseW(target); } } }

void ZCamilleModes::Flee() {
    GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());

    if (ZCamille::Menu->FleeE->Enabled()) {
        ZCamille::UseE(GGame->CursorPosition(), false); } }

void ZCamilleModes::JungleClear() {
    auto ex = ZCamille::Ex;
    auto player = ZCamille::Player;
    auto menu = ZCamille::Menu;

    if ((ZCamille::W->IsReady() || ZCamille::E->IsReady())) {
        for (auto i : GEntityList->GetAllMinions(false, false, true)) {
            if (menu->FarmNearEnemies->Enabled() || ex->CountInRange(player, 1000, GEntityList->GetAllHeros(false, true)) < 1) {
                if (i->IsValidObject() && i->IsJungleCreep() && !i->IsDead() && i->IsVisible() && ex->Dist2D(i) <= 635) {
                    Vec3 pos;
                    int numberHit;
                    ZCamille::W->FindBestCastPosition(true, false, pos, numberHit);

                    if (menu->JungleClearW->Enabled()) {
                        if (ex->IsValid(pos)) {
                            if (menu->MagnetWClear->Enabled()) {
                                ZCamille::LockW(pos); }

                            if (numberHit > 0 && ZCamille::ChargingW() == false) {
                                ZCamille::W->CastOnPosition(pos); } } }

                    if (ZCamille::W->IsReady() == false || !menu->JungleClearW->Enabled()) {
                        if (ZCamille::ChargingW() == false && menu->JungleClearE->Enabled()) {
                            ZCamille::UseE(pos); } } } } } } }

void ZCamilleModes::WaveClear() {
    auto menu = ZCamille::Menu;

    if (!ZCamille::ChargingW() && ZCamille::W->IsReady() && menu->WaveClearW->Enabled()) {
        auto player = ZCamille::Player;
        auto ex = ZCamille::Ex;

        for (auto i : GEntityList->GetAllMinions(false, true, false)) {
            if (menu->FarmNearEnemies->Enabled() || ex->CountInRange(player, 1000, GEntityList->GetAllHeros(false, true)) < 1) {
                if (i->IsValidObject() && i->IsCreep() && !i->IsDead() && i->IsVisible() && ex->Dist2D(i) <= 635) {
                    if (menu->WaveClearW->Enabled() && ex->IsValid(i->ServerPosition())) {
                        if (menu->MagnetWClear->Enabled()) {
                            ZCamille::LockW(i->ServerPosition()); }

                        if (ZCamille::ChargingW() == false) {
                            ZCamille::W->CastOnPosition(i->ServerPosition()); } } } } } } }

void ZCamilleModes::OnRender() {
    if (ZCamille::Player->IsDead()) {
        return; }

    if (ZCamille::Menu->DrawComboDamage->Enabled()) {
        Vec4 bb;
        ZCamille::Menu->DrawComboDamageColor->GetColor(&bb);

        for (auto i : GEntityList->GetAllHeros(false, true)) {
            if (i->IsOnScreen() && i->IsVisible() && i->IsValidTarget()) {
                ZCamille::DrawDamageOnChampionHPBar(i, ZCamille::CDmg(i), bb); } } }

    if (ZCamille::Menu->DrawR->Enabled()) {
        Vec4 rr;
        ZCamille::Menu->DrawRColor->GetColor(&rr);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), rr, ZCamille::R->Range()); }

    if (ZCamille::Menu->DrawE->Enabled()) {
        Vec4 ee;
        ZCamille::Menu->DrawEColor->GetColor(&ee);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), ee, ZCamille::E->Range()); }

    if (ZCamille::Menu->DrawW->Enabled()) {
        Vec4 ww;
        ZCamille::Menu->DrawWColor->GetColor(&ww);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), ww, ZCamille::W->Range()); }

    Vec2 pos;

    if (GGame->Projection(ZCamille::Player->GetPosition(), &pos)) {
        Vec4 ee;
        ZCamille::Menu->DrawEColor->GetColor(&ee);
        GRender->DrawTextW(Vec2(pos.x - 52, pos.y), ee, ZCamille::Menu->DontEUnderTurret->Enabled() ? "E Under Turret: ON" : "E Under Turret: OFF"); } }

void ZCamilleModes::OnCreateObj(IUnit * source) {
    if (source != nullptr && strcmp(source->GetClassIdentifier(), "obj_GeneralParticleEmitter") == 0) {
        if (strcmp(source->GetObjectName(), "Camille_Base_R_Indicator_Edge.troy") == 0) {
            ZCamille::DangerPoints.insert(std::make_pair(GGame->Time(), new ZCamilleAvoider(source, Outside, 450))); }

        if (strcmp(source->GetObjectName(), "Veigar_Base_E_Cage_Red.troy") == 0) {
            ZCamille::DangerPoints.insert(std::make_pair(GGame->Time(), new ZCamilleAvoider(source, Inside,  400))); } } }

void ZCamilleModes::OnDoCast(CastedSpell const & args) {
    auto ex = ZCamille::Ex;
    auto menu = ZCamille::Menu;
    auto player = ZCamille::Player;

    if (args.AutoAttack_ && args.Caster_->GetNetworkId() == player->GetNetworkId()) {
        auto hydra = ZCamille::Hydra;
        auto tiamat = ZCamille::Tiamat;
        auto titanic = ZCamille::Titanic;

        if (args.Target_ != nullptr) {
            if (args.Target_->IsValidTarget() && args.Target_->UnitFlags() == FL_HERO) {
                if (!ex->UnderEnemyTurret(player) || ex->CanCombo()) {
                    if (!ZCamille::Q->IsReady() || ZCamille::HasQ() && !ZCamille::HasQ2()) {
                        if (tiamat->IsOwned() && tiamat->IsReady()) {
                            tiamat->CastOnPlayer(); }

                        if (hydra->IsOwned() && hydra->IsReady()) {
                            hydra->CastOnPlayer(); }

                        if (titanic->IsOwned() && titanic->IsReady()) {
                            titanic->CastOnPlayer(); } } }

                if (ex->CanCombo()) {
                    if (menu->ComboQ->Enabled()) {
                        ZCamille::UseQ(args.Target_); } }

                if (ex->CanHarass()) {
                    if (menu->HarassQ->Enabled()) {
                        ZCamille::UseQ(args.Target_); } } }

            if (args.Target_->IsValidTarget() && args.Target_->UnitFlags() == FL_CREEP) {
                if (ex->UnderAllyTurret(player) || ex->CountInRange(player, 1000, GEntityList->GetAllHeros(false, true)) < 1 || menu->FarmNearEnemies->Enabled()) {
                    if (!ZCamille::Q->IsReady() || ZCamille::HasQ() && !ZCamille::HasQ2()) {
                        if (menu->FarmingWithItems->Enabled()) {
                            if (tiamat->IsOwned() && tiamat->IsReady()) {
                                tiamat->CastOnPlayer(); }

                            if (hydra->IsOwned() && hydra->IsReady()) {
                                hydra->CastOnPlayer(); }

                            if (titanic->IsOwned() && titanic->IsReady()) {
                                titanic->CastOnPlayer(); } } }

                    if (ex->CanJungleClear() && args.Target_->IsJungleCreep()) {
                        if (menu->JungleClearQ->Enabled()) {
                            ZCamille::UseQ(args.Target_); } }

                    if (ex->CanWaveClear() && args.Target_->GetTeam() != kTeamNeutral) {
                        if (menu->WaveClearQ->Enabled()) {
                            ZCamille::UseQ(args.Target_); } } }

                if (!args.Target_->IsDead() && args.Target_->IsTargetable()) {
                    if (!args.Target_->IsCreep() && !args.Target_->IsJungleCreep()) {
                        if (!ex->UnderEnemyTurret(player) ||
                            ex->CountInRange(player, 1000, GEntityList->GetAllHeros(false, true)) <= 0) {
                            if (args.Target_->GetTeam() != player->GetTeam()) {
                                // turrets etc
                                ZCamille::UseQ(args.Target_); } } } } } } } }

bool ZCamilleModes::OnIssueOrder(IUnit * source, DWORD orderIdx, Vec3 position, IUnit * target) {
    auto ex = ZCamille::Ex;
    auto player = ZCamille::Player;
    auto menu = ZCamille::Menu;

    if (ZCamille::OnWall() && ZCamille::E->IsReady() && ex->IsKeyDown(menu->JungleClearKey)) {
        auto issueOrderPos = position;
        auto neutralCreeps = std::vector<IUnit *>();

        if (source->GetNetworkId() == player->GetNetworkId()) {
            if (strcmp(std::to_string(orderIdx).c_str(), "2") == 0) {
                Vec3 pos;
                int numberHit;
                ZCamille::E->FindBestCastPosition(true, false, pos, numberHit);

                if (ex->IsValid(pos) && numberHit > 0) {
                    GGame->IssueOrderEx(player, kMoveTo, pos, false);
                    return false; } } } }

    if (ZCamille::OnWall() && ZCamille::E->IsReady() && ex->IsKeyDown(menu->ComboKey)) {
        auto issueOrderPos = position;

        if (source->GetNetworkId() == player->GetNetworkId()) {
            if (strcmp(std::to_string(orderIdx).c_str(), "2") == 0) {
                auto issueOrderDirection = (issueOrderPos - player->ServerPosition()).VectorNormalize();
                auto etarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, ZCamille::E->Range() + 100);

                if (etarget != nullptr && etarget->IsValidTarget()) {
                    auto heroDirection = (etarget->ServerPosition() - player->ServerPosition()).VectorNormalize();
                    auto anyDangerousPos = false;
                    auto dashEndPos = player->ServerPosition() + heroDirection * ex->Dist2D(etarget);

                    for (auto entry : ZCamille::DangerPoints) {
                        auto obj = entry.second;

                        if (obj->aType == Outside && ex->Dist2D(dashEndPos, obj->Emitter->GetPosition()) > obj->Radius) {
                            anyDangerousPos = true;
                            break; }

                        if (obj->aType == Inside) {
                            auto pos = ex->To2D(entry.second->Emitter->GetPosition());
                            auto end = ex->To2D(dashEndPos);
                            auto startPos = ex->To2D(player->ServerPosition());
                            auto proj = ex->ProjectOn(pos, startPos, end);

                            if (proj->IsOnSegment && ex->Dist2D(proj->SegmentPoint, pos) <= obj->Radius + player->BoundingRadius()) {
                                anyDangerousPos = true;
                                break; } } }

                    if (ex->UnderEnemyTurret(dashEndPos) && menu->DontEUnderTurret->Enabled()) {
                        anyDangerousPos = true; }

                    if (anyDangerousPos) {
                        return false; }

                    if (strcmp("1", std::to_string(menu->ComboEPred->GetInteger()).c_str()) == 0) {
                        auto pred = new AdvPredictionOutput();

                        if (ZCamille::E->RunPrediction(etarget, false, kCollidesWithNothing, pred)) {
                            if (pred->HitChance >= kHitChanceHigh) {
                                GGame->IssueOrderEx(player, kMoveTo, pred->CastPosition, false);
                                return false; } } }
                    else {
                        GGame->IssueOrderEx(player, kMoveTo, etarget->ServerPosition(), false);
                        return false; } }
                else {
                    return false; } } } }

    return true; }
