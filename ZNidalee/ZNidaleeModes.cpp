#include "ZNidaleeModes.h"
#include "PluginSDK.h"
#include "ZNidalee.h"
#include <algorithm>

void ZNidaleeModes::OnUpdate() {
    auto ex = ZNidalee::Ex;
    auto menu = ZNidalee::Menu;
    auto & stamps = ZNidalee::TimeStamps;


    // remove hunted targets;
    for(auto & i : ZNidalee::TheHunted) {
        auto timestamp = i.first;

        if(GGame->Time() - timestamp > 4) {
            ZNidalee::TheHunted.erase(timestamp); } }

    // delay handler
    ZNidalee::Delay->OnGameUpdate();

    // static single target
    ZNidalee::STarg = GTargetSelector->FindTarget(QuickestKill, SpellDamage, ZNidalee::SpellE->GetSpellRange());

    // flee
    if(ZNidalee::Ex->IsKeyDown(ZNidalee::Menu->FleeButton)) {
        ZNidalee::Flee(); }

    // combat
    if(ZNidalee::Ex->IsKeyDown(ZNidalee::Menu->ComboButton)) {
        auto combo = std::string("co");
        auto solo = ZNidalee::Menu->CurrentPlayStyle->GetInteger() == 0;

        ZNidalee::CastSpear(solo ? ZNidalee::STarg : GTargetSelector->FindTarget(QuickestKill, SpellDamage, ZNidalee::SJavelin->Range()), combo);
        ZNidalee::PlaceTrap(solo ? ZNidalee::STarg : GTargetSelector->FindTarget(QuickestKill, SpellDamage, ZNidalee::SBushwhack->Range()), combo);
        ZNidalee::SwitchForm(solo ? ZNidalee::STarg : GTargetSelector->FindTarget(QuickestKill, SpellDamage, ZNidalee::SJavelin->Range()), combo);
        ZNidalee::Bite(solo ? ZNidalee::STarg : GTargetSelector->FindTarget(QuickestKill, SpellDamage, ZNidalee::SPounce->Range() * 2), combo);
        ZNidalee::Pounce(solo ? ZNidalee::STarg : GTargetSelector->FindTarget(QuickestKill, SpellDamage, ZNidalee::SPounce->Range() * 2), combo);
        ZNidalee::Swipe(solo ? ZNidalee::STarg : GTargetSelector->FindTarget(QuickestKill, SpellDamage, ZNidalee::SSwipe->Range()), combo); }

    if(ZNidalee::Ex->IsKeyDown(ZNidalee::Menu->HarassButton)) {
        auto harass = std::string("ha");
        ZNidalee::CastSpear(GTargetSelector->FindTarget(QuickestKill, SpellDamage, ZNidalee::SJavelin->Range()), harass);
        ZNidalee::SwitchForm(GTargetSelector->FindTarget(QuickestKill, SpellDamage, ZNidalee::SJavelin->Range()), harass);
        ZNidalee::Bite(GTargetSelector->FindTarget(QuickestKill, SpellDamage, ZNidalee::SPounce->Range() * 2), harass);
        ZNidalee::Swipe(GTargetSelector->FindTarget(QuickestKill, SpellDamage, ZNidalee::SSwipe->Range()), harass); }

    if(ZNidalee::CanUse(ZNidalee::SpellQ, true, "imm")) {
        for(auto e : GEntityList->GetAllHeros(false, true)) {
            if(ZNidalee::Ex->Dist2D(e) <= ZNidalee::SJavelin->Range()) {
                if(e->IsValidTarget()) {
                    auto pred = new AdvPredictionOutput();

                    if(ZNidalee::SJavelin->RunPrediction(e, false, ZNidalee::QCollisionFlags, pred)) {
                        if(pred->HitChance == kHitChanceImmobile) {
                            ZNidalee::SJavelin->CastOnPosition(pred->CastPosition); } } } } } }

    if(ZNidalee::CanUse(ZNidalee::SpellW, true, "imm")) {
        for(auto e : GEntityList->GetAllHeros(false, true)) {
            if(ZNidalee::Ex->Dist2D(e) <= ZNidalee::SBushwhack->Range()) {
                if(e->IsValidTarget()) {
                    auto pred = new AdvPredictionOutput();

                    if(ZNidalee::SBushwhack->RunPrediction(e, false, kCollidesWithNothing, pred)) {
                        if(pred->HitChance == kHitChanceImmobile) {
                            ZNidalee::SBushwhack->CastOnPosition(pred->CastPosition); } } } } } }

    if(ZNidalee::Ex->IsKeyDown(ZNidalee::Menu->ClearButton)) {
        auto waveclear = std::string("wc");
        auto jungle = std::string("jg");
        auto creeps = GEntityList->GetAllMinions(false, true, true);
        std::sort(creeps.begin(), creeps.end(), [&](IUnit * a, IUnit * b) { return ZNidalee::GetPriorityJungleTarget(a, b); });

        for(auto unit : creeps) {
            if(unit->IsJungleCreep()) {
                if(!strstr(unit->GetObjectName(), "Mini")) {
                    ZNidalee::CastSpear(unit, jungle);
                    ZNidalee::PlaceTrap(unit, jungle); }

                ZNidalee::SwitchForm(unit, jungle);
                ZNidalee::Pounce(unit, jungle);
                ZNidalee::Bite(unit, jungle);
                ZNidalee::Swipe(unit, jungle); }

            if(unit->IsCreep()) {
                ZNidalee::CastSpear(unit, waveclear);
                ZNidalee::PlaceTrap(unit, waveclear);
                ZNidalee::SwitchForm(unit, waveclear);
                ZNidalee::Pounce(unit, waveclear);
                ZNidalee::Bite(unit, waveclear);
                ZNidalee::Swipe(unit, waveclear); } } }

    // update spell states/timers
    for(auto & humQ : menu->HumanQMap) {
        auto map = menu->HumanQMap.find(humQ.first);

        if(map != menu->HumanQMap.end()) {
            map->second.second = (stamps.at("Javelin") - GGame->Time()) > 0 ? (stamps.at("Javelin") - GGame->Time()) : 0; } }

    for(auto & humW : menu->HumanWMap) {
        auto map = menu->HumanWMap.find(humW.first);

        if(map != menu->HumanWMap.end()) {
            map->second.second = (stamps.at("Bushwhack") - GGame->Time()) > 0 ? (stamps.at("Bushwhack") - GGame->Time()) : 0; } }

    for(auto & humE : menu->HumanEMap) {
        auto map = menu->HumanEMap.find(humE.first);

        if(map != menu->HumanEMap.end()) {
            map->second.second = (stamps.at("Primalsurge") - GGame->Time()) > 0 ? (stamps.at("Primalsurge") - GGame->Time()) : 0; } }

    for(auto & humR : menu->HumanRMap) {
        auto map = menu->HumanRMap.find(humR.first);

        if(map != menu->HumanRMap.end()) {
            map->second.second = (stamps.at("Aspect") - GGame->Time()) > 0 ? (stamps.at("Aspect") - GGame->Time()) : 0; } }

    for(auto & catQ : menu->CougarQMap) {
        auto map = menu->CougarQMap.find(catQ.first);

        if(map != menu->CougarQMap.end()) {
            map->second.second = (stamps.at("Takedown") - GGame->Time()) > 0 ? (stamps.at("Takedown") - GGame->Time()) : 0; } }

    for(auto & catW : menu->CougarWMap) {
        auto map = menu->CougarWMap.find(catW.first);

        if(map != menu->CougarWMap.end()) {
            map->second.second = (stamps.at("Pounce") - GGame->Time()) > 0 ? (stamps.at("Pounce") - GGame->Time()) : 0; } }

    for(auto & catE : menu->CougarEMap) {
        auto map = menu->CougarEMap.find(catE.first);

        if(map != menu->CougarEMap.end()) {
            map->second.second = (stamps.at("Swipe") - GGame->Time()) > 0 ? (stamps.at("Swipe") - GGame->Time()) : 0; } }

    for(auto & catR : menu->CougarRMap) {
        auto map = menu->HumanRMap.find(catR.first);

        if(map != menu->CougarRMap.end()) {
            map->second.second = (stamps.at("Aspect") - GGame->Time()) > 0 ? (stamps.at("Aspect") - GGame->Time()) : 0; } }

    // auto heal allies
    if(ZNidalee::CanUse(ZNidalee::SpellE, true, "on")) {
        auto player = ZNidalee::Player;

        if(!player->GetSpellBook()->IsChanneling() && !player->IsRecalling()) {
            if(!ex->IsKeyDown(ZNidalee::Menu->FleeButton) || !ZNidalee::CatForm()) {
                auto allies = GEntityList->GetAllHeros(true, false);
                std::sort(allies.begin(), allies.end(), [&](IUnit * a, IUnit * b) { return ZNidalee::GetPriorityHealTarget(a, b); });
                allies.erase(std::remove_if(allies.begin(), allies.end(), [&](IUnit * u) { return u->IsDead() || !u->IsValidTarget(); }), allies.end());

                for(auto a : allies) {
                    if(ex->Dist2D(a) > ZNidalee::SpellE->GetSpellRange()) {
                        return; }

                    if(ZNidalee::Menu->AllyHealMap.count(a->ChampionName() + std::string("on"))) {
                        if(ZNidalee::Menu->AllyHealMap.at(a->ChampionName() + std::string("on"))->Enabled()) {
                            if(a->HealthPercent() < ZNidalee::Menu->AllyHealMap.at(a->ChampionName() + std::string("pct"))->GetInteger()) {
                                if(GOrbwalking->GetOrbwalkingMode() == kModeNone || a->HealthPercent() <= 35 || !ZNidalee::CatForm()) {
                                    if(player->ManaPercent() > ZNidalee::Menu->HealingManaPct->GetInteger() || a->HealthPercent() <= 35) {

                                        if(!ZNidalee::CatForm()) {
                                            ZNidalee::SpellE->CastOnUnit(a); }
                                        else {
                                            if(ZNidalee::CanUse(ZNidalee::SpellR, false, "heal")) {
                                                ZNidalee::SpellR->CastOnPlayer(); } } } } } } } } } } } }

void ZNidaleeModes::OnSpellCast(CastedSpell & args) {
    auto menu = ZNidalee::Menu;
    auto player = ZNidalee::Player;
    auto & timestamps = ZNidalee::TimeStamps;

    // auto heal ally attacking a hero or turret
    if(args.Caster_->GetTeam() == player->GetTeam()) {
        auto allies = GEntityList->GetAllHeros(true, false);
        std::sort(allies.begin(), allies.end(), [&](IUnit * a, IUnit * b) { return ZNidalee::CompareDamage(a, b); });
        allies.erase(std::remove_if(allies.begin(), allies.end(), [&](IUnit * u) { return u->IsDead() || !u->IsValidTarget(); }), allies.end());

        if(args.Target_ == nullptr || ZNidalee::Ex->Dist2D(args.Caster_) > ZNidalee::SpellE->GetSpellRange()) {
            return; }

        for(auto a : allies) {
            if(a->GetNetworkId() == args.Caster_->GetNetworkId() &&
                    menu->AllyHealMap.at(a->ChampionName() + std::string("on"))->Enabled()) {
                if(ZNidalee::CanUse(ZNidalee::SpellE, true, "on") && (args.Target_->UnitFlags() == FL_TURRET || args.Target_->UnitFlags() == FL_HERO)) {
                    if(a->HealthPercent() <= 90) {
                        if(!player->GetSpellBook()->IsChanneling() && !player->IsRecalling()) {
                            if(GOrbwalking->GetOrbwalkingMode() != kModeNone || a->HealthPercent() <= 35 || !ZNidalee::CatForm()) {
                                if(player->ManaPercent() > menu->HealingManaPct->GetInteger() || a->HealthPercent() <= 35) {
                                    if(ZNidalee::CatForm() == false) {
                                        ZNidalee::SpellE->CastOnUnit(a); } } } } } } } } } }

void ZNidaleeModes::OnSpellCastDelayed(CastedSpell & args) {
    auto menu = ZNidalee::Menu;
    auto player = ZNidalee::Player;
    auto & timestamps = ZNidalee::TimeStamps;
    auto & hunted = ZNidalee::TheHunted;

    if(args.Caster_->GetNetworkId() == player->GetNetworkId()) {

        // l33t xd
        auto name = args.Name_;
        auto size = strlen(name);
        auto nameToLower = static_cast<char *>(malloc(size + 1));
        nameToLower[size] = 0;

        for(auto i = 0; i < size; i++) {
            nameToLower[i] = tolower(name[i]); }

        if(strcmp(nameToLower, "nidaleetakedownattack") == 0) {
            auto val = timestamps.find("Takedown");
            auto cd = player->GetSpellTotalCooldown(kSlotQ);

            if(menu->PracticeToolsDebug->Enabled()) {
                if(val != timestamps.end()) {
                    val->second = GGame->Time() + 1; }

                return; }

            if(val != timestamps.end()) {
                val->second = GGame->Time() + cd; } }

        if(strcmp(nameToLower, "pounce") == 0) {
            auto unit = args.Target_;
            auto val = timestamps.find("Pounce");
            auto cd = player->GetSpellTotalCooldown(kSlotW);

            if(menu->PracticeToolsDebug->Enabled()) {
                if(val != timestamps.end()) {
                    val->second = GGame->Time() + 1; }

                return; }

            if(val != timestamps.end()) {
                if(unit != nullptr && !unit->IsDead() && unit->IsValidObject() && ZNidalee::IsHunted(unit)) {
                    // checks if was a hunted pounce (targeted)
                    for(auto & i : hunted) {
                        auto networkId = i.second.first;
                        auto & pounces = i.second.second;

                        if(networkId == unit->GetNetworkId()) {
                            if(pounces < 1) {
                                // reduce cooldown by half
                                val->second = GGame->Time() + cd / 2;
                                pounces++; }
                            else {
                                // already reduced!
                                val->second = GGame->Time() + cd;
                                pounces++; } } } }
                else {
                    // regular pounce
                    val->second = GGame->Time() + cd; } } }

        if(strcmp(nameToLower, "swipe") == 0) {
            auto val = timestamps.find("Swipe");
            auto cd = player->GetSpellTotalCooldown(kSlotE);

            if(menu->PracticeToolsDebug->Enabled()) {
                if(val != timestamps.end()) {
                    val->second = GGame->Time() + 1; }

                return; }

            if(val != timestamps.end()) {
                val->second = GGame->Time() + cd; } }

        if(strcmp(nameToLower, "javelintoss") == 0) {

            auto val = timestamps.find("Javelin");
            auto cd = player->GetSpellTotalCooldown(kSlotQ);

            if(menu->PracticeToolsDebug->Enabled()) {

                if(val != timestamps.end()) {
                    val->second = GGame->Time() + 1; }

                return; }

            if(val != timestamps.end()) {
                val->second = GGame->Time() + cd; } }

        if(strcmp(nameToLower, "primalsurge") == 0) {
            auto val = timestamps.find("Primalsurge");
            auto cd = player->GetSpellTotalCooldown(kSlotE);

            if(menu->PracticeToolsDebug->Enabled()) {

                if(val != timestamps.end()) {
                    val->second = GGame->Time() + 1; }

                return; }

            if(val != timestamps.end()) {
                val->second = GGame->Time() + cd; } }

        if(strcmp(nameToLower, "bushwhack") == 0) {
            auto val = timestamps.find("Bushwhack");
            auto cd = player->GetSpellTotalCooldown(kSlotW);

            if(menu->PracticeToolsDebug->Enabled()) {

                if(val != timestamps.end()) {
                    val->second = GGame->Time() + 1; }

                return; }

            if(val != timestamps.end()) {
                val->second = GGame->Time() + cd; } }

        if(strcmp(nameToLower, "aspectofthecougar") == 0) {
            auto it = timestamps.find("Aspect");
            auto cd = player->GetSpellTotalCooldown(kSlotR);

            if(it != timestamps.end()) {
                it->second = GGame->Time() + cd; } } } }

void ZNidaleeModes::OnRender() {
    if(ZNidalee::Player->IsDead()) {
        return; }

    if(ZNidalee::Menu->DrawQ->Enabled() && !ZNidalee::CatForm()) {
        Vec4 rr;
        ZNidalee::Menu->DrawQColor->GetColor(&rr);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), rr, ZNidalee::SJavelin->Range()); }

    if(ZNidalee::Menu->DrawW2->Enabled()) {
        Vec4 ee;
        ZNidalee::Menu->DrawW2Color->GetColor(&ee);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), ee, ZNidalee::SPounce->Range() * 2); }

    if(ZNidalee::Menu->DrawE->Enabled() && ZNidalee::CatForm()) {
        Vec4 ww;
        ZNidalee::Menu->DrawEColor->GetColor(&ww);
        GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), ww, ZNidalee::SSwipe->Range()); }

    Vec2 pos;

    if(GGame->Projection(ZNidalee::Player->GetPosition(), &pos) && ZNidalee::Menu->DrawQTimer->Enabled()) {
        Vec4 ee;
        ZNidalee::Menu->DrawEColor->GetColor(&ee);
        GRender->DrawTextW(Vec2(pos.x + 52, pos.y + 10), ee, std::to_string(ZNidalee::Menu->HumanQMap.at("Javelinco").second).c_str()); } }

void ZNidaleeModes::OnGapCloser(const GapCloserSpell & args) {
    return;
    auto player = ZNidalee::Player;
    auto ex = ZNidalee::Ex;

    if(args.Sender->GetTeam() != player->GetTeam()) {
        if(args.Sender->IsValidTarget() && ex->Dist2D(args.Sender) <= 200) {
            if(!ex->UnderEnemyTurret(player)) {
                if(ZNidalee::CatForm()) {
                    ZNidalee::Swipe(args.Sender, "gap");
                    ZNidalee::SwitchForm(args.Sender, "gap"); }
                else {
                    ZNidalee::CastSpear(args.Sender, "gap");
                    ZNidalee::SwitchForm(args.Sender, "gap"); } } } } }

void ZNidaleeModes::OnBuffAdd(IUnit * unit, void * buffdata) {
    auto player = ZNidalee::Player;
    auto & huntedList = ZNidalee::TheHunted;

    if(strcmp(GBuffData->GetBuffName(buffdata), "NidaleePassiveHunted") == 0) {
        if(unit->GetTeam() != player->GetTeam()) {
            huntedList.insert(std::pair<float, std::pair<int, int>>(GGame->Time(), std::pair<int, int>(unit->GetNetworkId(), 0))); } } }

void ZNidaleeModes::Flee() {

}




