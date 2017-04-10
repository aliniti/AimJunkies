#include "PluginSDK.h"
#include "ZElise.h"

PluginSetup("ZElise");
IUnit * ZElise::Player;
ZEliseMenu * ZElise::Menu;
ZEliseModes * ZElise::Modes;
ZEliseExtensions * ZElise::Ex;
DelayAction * ZElise::Delay;

ISpell * ZElise::SpellQ;
ISpell * ZElise::SpellW;
ISpell * ZElise::SpellE;
ISpell * ZElise::SpellR;
ISpell * ZElise::SpellZ;

ISpell2 * ZElise::E;
IInventoryItem * ZElise::Protobelt;
std::map<std::string, ZEliseAvoider *> ZElise::AvoidList;
eCollisionFlags ZElise::ECollisionFlags;
std::map<std::string, int> ZElise::TimeStamps;

PLUGIN_EVENT(void) OnGameUpdate() {
    ZElise::Modes->OnUpdate(); }

PLUGIN_EVENT(void) OnCastSpell(CastedSpell & args) {
    ZElise::Modes->OnSpellCast(args);
    ZElise::Delay->AddCastedSpell(250, args, [&](CastedSpell & a) { ZElise::Modes->OnSpellCastDelayed(a); }); }

PLUGIN_EVENT(void) OnPlayAnimation(IUnit * unit, std::string args) {
    ZElise::Modes->OnPlayAnimation(unit, args); }

PLUGIN_EVENT(void) OnRender() {
    ZElise::Modes->OnRender(); }

PLUGIN_API void OnLoad(IPluginSDK * sdk) {
    PluginSDKSetup(sdk);

    ZElise::Delay = new DelayAction;
    ZElise::Player = GEntityList->Player();
    ZEliseAvoider::GenerateAvoidList();

    if(strcmp(ZElise::Player->ChampionName(), "Elise") == 0) {
        ZElise::OnBoot();
        ZElise::Menu = new ZEliseMenu(GPluginSDK->AddMenu("ZElise"));
        GEventManager->AddEventHandler(kEventOnSpellCast, OnCastSpell);
        GEventManager->AddEventHandler(kEventOnPlayAnimation, OnPlayAnimation);
        GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
        GEventManager->AddEventHandler(kEventOnRender, OnRender);
        GGame->PrintChat("<font color=\"#794DFF\"><b>ZElise</b></font><b><font color=\"#FFFFFF\"> Loaded!</font></b>"); } }

PLUGIN_API void OnUnload() {
    if(strcmp(ZElise::Player->ChampionName(), "Elise") == 0) {
        ZElise::OnShutdown();
        GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
        GEventManager->RemoveEventHandler(kEventOnPlayAnimation, OnPlayAnimation);
        GEventManager->RemoveEventHandler(kEventOnSpellCast, OnCastSpell);
        GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
        ZElise::Menu->Menu->Remove(); } }