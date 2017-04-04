#include "ZEliseModes.h"
#include "ZElise.h"
#include <string>

void ZEliseModes::OnUpdate() {
    auto menu = ZElise::Menu;
    auto & stamps = ZElise::TimeStamps;

    ZElise::Delay->OnGameUpdate();
    ZElise::HandleAttacks();

    if(ZElise::Ex->IsKeyDown(menu->ComboButton)) {
        auto mode = std::string("co");
        auto unit = GTargetSelector->FindTarget(QuickestKill, SpellDamage, ZElise::E->Range());

        ZElise::CocoonE(unit, mode);

        if(ZElise::BurstCheck(unit, mode)) {
            ZElise::UseProtobelt(unit, mode);
            ZElise::NeurotoxinQ(unit, mode);
            ZElise::VolatileW(unit, mode); }

        ZElise::SwitchForm(unit, mode);
        ZElise::FrenzyW(unit, mode);
        ZElise::BiteQ(unit, mode);
        ZElise::RappelE(unit, mode); }

    if(ZElise::Ex->IsKeyDown(menu->FarmButton)) {
        auto jungle = std::string("jg");
        auto creeps = GEntityList->GetAllMinions(false, true, true);
        std::sort(creeps.begin(), creeps.end(), [&](IUnit * a, IUnit * b) { return ZElise::GetPriorityJungleTarget(a, b); });

        for(auto unit : creeps) {
            if(unit->IsJungleCreep()) {
                if(!strstr(unit->GetObjectName(), "Mini")) {
                    ZElise::CocoonE(unit, jungle); }

                if(ZElise::BurstCheck(unit, jungle)) {
                    //ZElise::NeurotoxinQ(unit, jungle);
                    //ZElise::VolatileW(unit, jungle);

                }

                //ZElise::SwitchForm(unit, jungle);
                //ZElise::BiteQ(unit, jungle);
                //ZElise::FrenzyW(unit, jungle);
                //ZElise::RappelE(unit, jungle);


            } }

    }

    // update spell states/timers
    for(auto & HumanQ : menu->HumanQMap) {
        auto map = menu->HumanQMap.find(HumanQ.first);

        if(map != menu->HumanQMap.end()) {
            map->second.second = (stamps.at("HumanQ") - GGame->Time()) > 0 ? (stamps.at("HumanQ") - GGame->Time()) : 0; } }

    for(auto & HumanW : menu->HumanWMap) {
        auto map = menu->HumanWMap.find(HumanW.first);

        if(map != menu->HumanWMap.end()) {
            map->second.second = (stamps.at("HumanW") - GGame->Time()) > 0 ? (stamps.at("HumanW") - GGame->Time()) : 0; } }

    for(auto & HumanE : menu->HumanEMap) {
        auto map = menu->HumanEMap.find(HumanE.first);

        if(map != menu->HumanEMap.end()) {
            map->second.second = (stamps.at("HumanE") - GGame->Time()) > 0 ? (stamps.at("HumanE") - GGame->Time()) : 0; } }

    for(auto & HumanR : menu->HumanRMap) {
        auto map = menu->HumanRMap.find(HumanR.first);

        if(map != menu->HumanRMap.end()) {
            map->second.second = (stamps.at("HumanR") - GGame->Time()) > 0 ? (stamps.at("HumanR") - GGame->Time()) : 0; } }

    for(auto & spiderQ : menu->SpiderQMap) {
        auto map = menu->SpiderQMap.find(spiderQ.first);

        if(map != menu->SpiderQMap.end()) {
            map->second.second = (stamps.at("SpiderQ") - GGame->Time()) > 0 ? (stamps.at("SpiderQ") - GGame->Time()) : 0; } }

    for(auto & spiderW : menu->SpiderWMap) {
        auto map = menu->SpiderWMap.find(spiderW.first);

        if(map != menu->SpiderWMap.end()) {
            map->second.second = (stamps.at("SpiderW") - GGame->Time()) > 0 ? (stamps.at("SpiderW") - GGame->Time()) : 0; } }

    for(auto & spiderE : menu->SpiderEMap) {
        auto map = menu->SpiderEMap.find(spiderE.first);

        if(map != menu->SpiderEMap.end()) {
            map->second.second = (stamps.at("SpiderE") - GGame->Time()) > 0 ? (stamps.at("SpiderE") - GGame->Time()) : 0; } }

    for(auto & spiderR : menu->SpiderRMap) {
        auto map = menu->SpiderRMap.find(spiderR.first);

        if(map != menu->SpiderRMap.end()) {
            map->second.second = (stamps.at("SpiderR") - GGame->Time()) > 0 ? (stamps.at("SpiderR") - GGame->Time()) : 0; } } }

void ZEliseModes::OnSpellCast(CastedSpell & args) {

}

void ZEliseModes::OnSpellCastDelayed(CastedSpell & args) {
    auto menu = ZElise::Menu;
    auto & player = ZElise::Player;
    auto & stamps = ZElise::TimeStamps;

    if(args.Caster_->GetNetworkId() == player->GetNetworkId()) {

        auto name = args.Name_;
        auto size = strlen(name);
        auto nameToLower = static_cast<char *>(malloc(size + 1));
        nameToLower[size] = 0;

        for(auto i = 0; i < size; i++) {
            nameToLower[i] = tolower(name[i]); }

        if(strcmp(nameToLower, "elisehumanq") == 0) {
            auto val = stamps.find("HumanQ");
            auto cd = player->GetSpellTotalCooldown(kSlotQ);

            if(menu->PracticeToolsDebug->Enabled()) {
                if(val != stamps.end()) {
                    val->second = GGame->Time() + 1; }

                return; }

            if(val != stamps.end()) {
                val->second = GGame->Time() + cd; } }

        if(strcmp(nameToLower, "elisehumanw") == 0) {
            auto val = stamps.find("HumanW");
            auto cd = player->GetSpellTotalCooldown(kSlotW);

            if(menu->PracticeToolsDebug->Enabled()) {
                if(val != stamps.end()) {
                    val->second = GGame->Time() + 1; }

                return; }

            if(val != stamps.end()) {
                val->second = GGame->Time() + cd; } }

        if(strcmp(nameToLower, "elisehumane") == 0) {
            auto val = stamps.find("HumanE");
            auto cd = player->GetSpellTotalCooldown(kSlotE);

            if(menu->PracticeToolsDebug->Enabled()) {
                if(val != stamps.end()) {
                    val->second = GGame->Time() + 1; }

                return; }

            if(val != stamps.end()) {
                val->second = GGame->Time() + cd; } }

        if(strcmp(nameToLower, "elisespiderqcast") == 0) {
            auto val = stamps.find("SpiderQ");
            auto cd = player->GetSpellTotalCooldown(kSlotQ);

            if(menu->PracticeToolsDebug->Enabled()) {
                if(val != stamps.end()) {
                    val->second = GGame->Time() + 1; }

                return; }

            if(val != stamps.end()) {
                val->second = GGame->Time() + cd; } }

        if(strcmp(nameToLower, "elisespiderw") == 0) {
            auto val = stamps.find("SpiderW");
            auto cd = player->GetSpellTotalCooldown(kSlotW);

            if(menu->PracticeToolsDebug->Enabled()) {
                if(val != stamps.end()) {
                    val->second = GGame->Time() + 1; }

                return; }

            if(val != stamps.end()) {
                val->second = GGame->Time() + cd; } }

        if(strstr(nameToLower, "eliser")) {
            auto vala = stamps.find("SpiderR");
            auto valb = stamps.find("HumanR");
            auto cd = player->GetSpellTotalCooldown(kSlotR);

            if(menu->PracticeToolsDebug->Enabled()) {
                if(vala != stamps.end()) {
                    vala->second = GGame->Time() + 1; }

                if(valb != stamps.end()) {
                    valb->second = GGame->Time() + 1; }

                return; }

            if(vala != stamps.end()) {
                vala->second = GGame->Time() + cd; }

            if(valb != stamps.end()) {
                valb->second = GGame->Time() + cd; } } } }

void ZEliseModes::OnPlayAnimation(IUnit * unit, const std::string & cs) {
    auto player = ZElise::Player;
    auto menu = ZElise::Menu;
    auto & stamps = ZElise::TimeStamps;

    if(unit->GetNetworkId() == player->GetNetworkId()) {
        // to get when we descend from rappel,
        // only workarround to get the real cooldown
        if(strcmp("0080edf8", cs.c_str()) == 0) {
            auto val = stamps.find("SpiderE");
            auto cd = player->GetSpellTotalCooldown(kSlotE);

            if(menu->PracticeToolsDebug->Enabled()) {
                if(val != stamps.end()) {
                    val->second = GGame->Time() + 1; }

                return; }

            if(val != stamps.end()) {
                val->second = GGame->Time() + cd; } } } }

void ZEliseModes::OnRender() {
    if(ZElise::Player->IsDead()) {
        return; }

    if(ZElise::Menu->DrawQ->Enabled()) {
        Vec4 qq;
        ZElise::Menu->DrawQColor->GetColor(&qq);

        if(!ZElise::SpiderForm()) {
            GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), qq, ZElise::SpellQ->GetSpellRange()); }
        else {
            GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), qq, ZElise::SpellZ->GetSpellRange()); } }

    if(ZElise::Menu->DrawE->Enabled()) {
        Vec4 ee;
        ZElise::Menu->DrawEColor->GetColor(&ee);

        if(!ZElise::SpiderForm()) {
            GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), ee, ZElise::E->Range()); }
        else {
            GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), ee, ZElise::SpellE->GetSpellRange()); } } }

