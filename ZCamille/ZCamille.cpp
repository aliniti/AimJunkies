#define _USE_MATH_DEFINES

#include "ZCamille.h"
#include "ZCamilleModes.h"
#include "PluginSDK.h"
#include <string>

PluginSetup("ZCamille");
DelayAction ZCamille::Delay;

int ZCamille::LastQ;
int ZCamille::LastW;
int ZCamille::LastE;
int ZCamille::LastR;

ISpell2 * ZCamille::Q;
ISpell2 * ZCamille::W;
ISpell2 * ZCamille::E;
ISpell2 * ZCamille::R;

IUnit * ZCamille::Player;
ZCamilleMenu * ZCamille::Menu;
ZCamilleModes * ZCamille::Modes;
ZCamilleExtensions * ZCamille::Ex;

IInventoryItem * ZCamille::Tiamat;
IInventoryItem * ZCamille::Hydra;
IInventoryItem * ZCamille::Youmuus;
IInventoryItem * ZCamille::Titanic;

std::map<std::string, ZCamilleAvoider *> ZCamille::DangerSpells;
std::map<float, ZCamilleAvoider *> ZCamille::DangerPoints;

PLUGIN_EVENT(void) OnGameUpdate() {
    ZCamille::Modes->OnUpdate(); }

PLUGIN_EVENT(void) OnCreateObject(IUnit * source) {
    ZCamille::Modes->OnCreateObj(source); }

PLUGIN_EVENT(bool) OnIssueOrderEx(IUnit * source, DWORD orderIdx, Vec3 position, IUnit * target) {
    return ZCamille::Modes->OnIssueOrder(source, orderIdx, position, target); }

PLUGIN_EVENT(void) OnDoCast(CastedSpell const & args) {
    ZCamille::Modes->OnDoCast(args); }

PLUGIN_EVENT(void) OnRender() {
    ZCamille::Modes->OnRender(); }

PLUGIN_API void OnLoad(IPluginSDK * sdk) {
    PluginSDKSetup(sdk);
    ZCamille::CreateItems();
    ZCamille::CreateSpells();

    if (strcmp(ZCamille::Player->ChampionName(), "Camille") == 0) {
        ZCamille::Menu = new ZCamilleMenu(GPluginSDK->AddMenu("ZCamille"));
        GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
        GEventManager->AddEventHandler(kEventOnIssueOrder, OnIssueOrderEx);
        GEventManager->AddEventHandler(kEventOnDoCast, OnDoCast);
        GEventManager->AddEventHandler(kEventOnCreateObject, OnCreateObject);
        GEventManager->AddEventHandler(kEventOnRender, OnRender);
        GGame->PrintChat("<font color=\"#794DFF\"><b>ZCamille</b></font><b><font color=\"#FFFFFF\"> Loaded!</font></b>"); } }

PLUGIN_API void OnUnload() {
    GOrbwalking->SetAttacksAllowed(true);
    GOrbwalking->SetMovementAllowed(true);
    GEventManager->RemoveEventHandler(kEventOnDoCast, OnDoCast);
    GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
    GEventManager->RemoveEventHandler(kEventOnIssueOrder, OnIssueOrderEx);
    GEventManager->RemoveEventHandler(kEventOnCreateObject, OnCreateObject);
    GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
    ZCamille::Menu->Menu->Remove(); }