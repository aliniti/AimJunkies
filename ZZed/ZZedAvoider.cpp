#include "ZZed.h"
#include "ZZedAvoider.h"
#include <utility>
#include <map>

void ZZedAvoider::GenerateAvoidList() {
    std::map<std::string, ZZedAvoider *> avoidList;
    avoidList.insert(std::make_pair("infernalguardian", new ZZedAvoider("infernalguardian", SkillshotCircle, "Annie")));
    avoidList.insert(std::make_pair("curseofthesadmummy", new ZZedAvoider("curseofthesadmummy", SelfCast, "Amumu")));
    avoidList.insert(std::make_pair("enchantedcystalarrow", new ZZedAvoider("enchantedcystalarrow", SkillshotLine, "Ashe")));
    avoidList.insert(std::make_pair("aurelionsolr", new ZZedAvoider("aurelionsolr", SkillshotLine, "AurelionSol")));
    avoidList.insert(std::make_pair("azirr", new ZZedAvoider("azirr", SkillshotCircle, "Azir")));
    avoidList.insert(std::make_pair("cassiopeiar", new ZZedAvoider("cassiopeiar", SkillshotCircle, "Cassiopeia")));
    avoidList.insert(std::make_pair("feast", new ZZedAvoider("feast", Targeted, "Chogath")));
    avoidList.insert(std::make_pair("dariusexecute", new ZZedAvoider("dariusexecute", Targeted, "Darius")));
    avoidList.insert(std::make_pair("evelynnr", new ZZedAvoider("evelynnr", SkillshotCircle, "Evelynn")));
    avoidList.insert(std::make_pair("galioidolofdurand", new ZZedAvoider("galioidolofdurand", SelfCast, "Galio")));
    avoidList.insert(std::make_pair("gnarult", new ZZedAvoider("gnarult", SelfCast, "Gnar")));
    avoidList.insert(std::make_pair("garenr", new ZZedAvoider("egarenr", Targeted, "Garen")));
    avoidList.insert(std::make_pair("gravesr", new ZZedAvoider("gravesr", SkillshotLine, "Graves")));
    avoidList.insert(std::make_pair("hecarimult", new ZZedAvoider("hecarimult", SkillshotLine, "Hecarim")));
    avoidList.insert(std::make_pair("illaoir", new ZZedAvoider("illaoir", SelfCast, "Illaoi")));
    avoidList.insert(std::make_pair("jarvanivcataclysm", new ZZedAvoider("jarvanivcataclysm", Targeted, "JarvanIV")));
    avoidList.insert(std::make_pair("blindmonkrkick", new ZZedAvoider("blindmonkrkick", Targeted, "LeeSin")));
    avoidList.insert(std::make_pair("lissandrar", new ZZedAvoider("lissandrar", Targeted, "Lissandra")));
    avoidList.insert(std::make_pair("ufslash", new ZZedAvoider("ufslash", SkillshotCircle, "Malphite")));
    avoidList.insert(std::make_pair("monkeykingspintowin", new ZZedAvoider("monkeykingspintowin", SelfCast, "MonkeyKing")));
    avoidList.insert(std::make_pair("rivenizunablade", new ZZedAvoider("rivenizunablade", SkillshotLine, "Riven")));
    avoidList.insert(std::make_pair("sejuaniglacialprisoncast", new ZZedAvoider("sejuaniglacialprisoncast", SkillshotLine, "Sejuani")));
    avoidList.insert(std::make_pair("shyvanatrasformcast", new ZZedAvoider("shyvanatrasformcast", SkillshotLine, "Shyvana")));
    avoidList.insert(std::make_pair("sonar", new ZZedAvoider("sonar", SkillshotLine, "Sona")));
    avoidList.insert(std::make_pair("syndrar", new ZZedAvoider("syndrar", Targeted, "Syndra")));
    avoidList.insert(std::make_pair("varusr", new ZZedAvoider("varusr", SkillshotLine, "Varus")));
    avoidList.insert(std::make_pair("veigarprimordialburst", new ZZedAvoider("veigarprimordialburst", Targeted, "Veigar")));
    avoidList.insert(std::make_pair("viktorchaosstorm", new ZZedAvoider("viktorchaosstorm", SkillshotCircle, "Viktor")));
    ZZed::AvoidList = avoidList; }

