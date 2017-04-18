#include "LPPConstants.h"
#include "PluginSDK.h"
#include "ZZed.h"

PluginSetup("ZZed");

IUnit * ZZed::Player;
ZZedMenu * ZZed::Menu;
ZZedModes * ZZed::Modes;
ZZedExtensions * ZZed::Ex;

ISpell * ZZed::W;
ISpell * ZZed::E;
ISpell * ZZed::R;
ISpell * ZZed::Ignite;
ISpell2 * ZZed::Q;
ISpell2 * ZZed::Z;

IInventoryItem * ZZed::Hydra;
IInventoryItem * ZZed::Tiamat;
IInventoryItem * ZZed::Titanic;
IInventoryItem * ZZed::Youmuus;

std::map<float, IUnit *> ZZed::Marked;
std::map<float, IUnit *> ZZed::Shadows;
std::map<std::string, float> ZZed::Ticks;

PLUGIN_EVENT(void) OnGameUpdate() {
    ZZed::Modes->OnUpdate(); }

PLUGIN_EVENT(void) OnCreateObject(IUnit * source) {
    ZZed::Modes->OnCreateObj(source); }

PLUGIN_EVENT(void) OnCastSpell(CastedSpell & args) {
    ZZed::Modes->OnSpellCast(args); }

PLUGIN_EVENT(void) OnBuffAdd(IUnit * source, void * buffdata) {
    ZZed::Modes->OnBuffAdd(source, buffdata); }

PLUGIN_EVENT(void) OnRender() {
    ZZed::Modes->OnRender(); }

PLUGIN_EVENT(void) OnDoCast(CastedSpell const & args) {
    ZZed::Modes->OnDoCast(args); }


PLUGIN_API void OnLoad(IPluginSDK * sdk) {
    PluginSDKSetup(sdk);

    ZZed::Player = GEntityList->Player();

    if(strcmp(ZZed::Player->ChampionName(), "Zed") == 0) {
        ZZed::OnBoot();
        ZZed::Menu = new ZZedMenu(GPluginSDK->AddMenu("ZZed"));
        GEventManager->AddEventHandler(kEventOnSpellCast, OnCastSpell);
        GEventManager->AddEventHandler(kEventOnBuffAdd, OnBuffAdd);
        GEventManager->AddEventHandler(kEventOnCreateObject, OnCreateObject);
        GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
        GEventManager->AddEventHandler(kEventOnRender, OnRender);
        GGame->PrintChat("<font color=\"#794DFF\"><b>ZZed</b></font><b><font color=\"#FFFFFF\"> Loaded!</font></b>"); } }

PLUGIN_API void OnUnload() {
    if(strcmp(ZZed::Player->ChampionName(), "Zed") == 0) {
        ZZed::OnShutdown();
        GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
        GEventManager->RemoveEventHandler(kEventOnBuffAdd, OnBuffAdd);
        GEventManager->RemoveEventHandler(kEventOnCreateObject, OnCreateObject);
        GEventManager->RemoveEventHandler(kEventOnSpellCast, OnCastSpell);
        GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
        ZZed::Menu->Menu->SaveSettings();
        ZZed::Menu->Menu->Remove(); } }


