#include "ZCamille.h"
#include "ZCamilleModes.h"
#include "PluginData.h"
#include "ZCamilleAvoider.h"
#include <string>

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
            if (!ZCamille::Menu->KeyState) {
                if (menu->DontEUnderTurret->Enabled() == false) {
                    menu->DontEUnderTurret->UpdateInteger(1); }

                else {
                    menu->DontEUnderTurret->UpdateInteger(0); }

                ZCamille::Menu->KeyState = true; } }

        else {
            ZCamille::Menu->KeyState = false; }

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

    for (auto i : GEntityList->GetAllMinions(false, false, true)) {
        if (ex->CountInRange(player, 1000, GEntityList->GetAllHeros(false, true)) < 1 || menu->FarmNearEnemies->Enabled()) {
            if (i->IsValidObject() && i->IsJungleCreep() && !i->IsDead() && i->IsVisible() &&
                    ex->Dist2D(i) <= 635) {
                Vec3 pos;
                int numberHit;
                ZCamille::W->FindBestCastPosition(true, false, pos, numberHit);

                if (menu->JungleClearW->Enabled()) {
                    if (ex->IsValid(pos)) {
                        if (menu->MagnetWClear->Enabled()) {
                            ZCamille::LockW(pos); }

                        if (numberHit > 0) {
                            ZCamille::W->CastOnPosition(pos); } } }

                if (!ZCamille::W->IsReady() || !menu->JungleClearW->Enabled()) {
                    if (!ZCamille::ChargingW() && menu->JungleClearE->Enabled()) {
                        ZCamille::UseE(pos); } } } } } }

void ZCamilleModes::WaveClear() {
    auto ex = ZCamille::Ex;
    auto player = ZCamille::Player;
    auto menu = ZCamille::Menu;

    for (auto i : GEntityList->GetAllMinions(false, true, false)) {
        if (ex->CountInRange(player, 1000, GEntityList->GetAllHeros(false, true)) < 1 || menu->FarmNearEnemies->Enabled()) {
            if (i->IsValidObject() && i->IsCreep() && !i->IsDead() && i->IsVisible() && ex->Dist2D(i) <= 635) {
                Vec3 pos;
                int numberHit;
                ZCamille::W->FindBestCastPosition(true, false, pos, numberHit);

                if (menu->WaveClearW->Enabled()) {
                    if (ex->IsValid(pos)) {
                        if (menu->MagnetWClear->Enabled()) {
                            ZCamille::LockW(pos); }

                        if (numberHit > 0) {
                            ZCamille::W->CastOnPosition(pos); } } } } } } }

void ZCamilleModes::OnRender() {
    if (ZCamille::Player->IsDead()) {
        return; }

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
        ZCamille::Menu->DrawRColor->GetColor(&ee);
        GRender->DrawTextW(Vec2(pos.x - 52, pos.y), ee,  ZCamille::Menu->DontEUnderTurret->Enabled() ? "E Under Turret: ON" : "E Under Turret: OFF"); } }

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
