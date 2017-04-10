#include "ZElise.h"
#include "ZEliseAvoider.h"

void ZEliseAvoider::GenerateAvoidList() {
    std::map<std::string, ZEliseAvoider *> avoidList;
    avoidList.insert(std::make_pair("infernalguardian", new ZEliseAvoider("infernalguardian", SkillshotCircle, "Annie")));
    avoidList.insert(std::make_pair("curseofthesadmummy", new ZEliseAvoider("curseofthesadmummy", SelfCast, "Amumu")));
    avoidList.insert(std::make_pair("enchantedcystalarrow", new ZEliseAvoider("enchantedcystalarrow", SkillshotLine, "Ashe")));
    avoidList.insert(std::make_pair("aurelionsolr", new ZEliseAvoider("aurelionsolr", SkillshotLine, "AurelionSol")));
    avoidList.insert(std::make_pair("azirr", new ZEliseAvoider("azirr", SkillshotCircle, "Azir")));
    avoidList.insert(std::make_pair("cassiopeiar", new ZEliseAvoider("cassiopeiar", SkillshotCircle, "Cassiopeia")));
    avoidList.insert(std::make_pair("feast", new ZEliseAvoider("feast", Targeted, "Chogath")));
    avoidList.insert(std::make_pair("dariusexecute", new ZEliseAvoider("dariusexecute", Targeted, "Darius")));
    avoidList.insert(std::make_pair("evelynnr", new ZEliseAvoider("evelynnr", SkillshotCircle, "Evelynn")));
    avoidList.insert(std::make_pair("galioidolofdurand", new ZEliseAvoider("galioidolofdurand", SelfCast, "Galio")));
    avoidList.insert(std::make_pair("gnarult", new ZEliseAvoider("gnarult", SelfCast, "Gnar")));
    avoidList.insert(std::make_pair("garenr", new ZEliseAvoider("egarenr", Targeted, "Garen")));
    avoidList.insert(std::make_pair("gravesr", new ZEliseAvoider("gravesr", SkillshotLine, "Graves")));
    avoidList.insert(std::make_pair("hecarimult", new ZEliseAvoider("hecarimult", SkillshotLine, "Hecarim")));
    avoidList.insert(std::make_pair("illaoir", new ZEliseAvoider("illaoir", SelfCast, "Illaoi")));
    avoidList.insert(std::make_pair("jarvanivcataclysm", new ZEliseAvoider("jarvanivcataclysm", Targeted, "JarvanIV")));
    avoidList.insert(std::make_pair("blindmonkrkick", new ZEliseAvoider("blindmonkrkick", Targeted, "LeeSin")));
    avoidList.insert(std::make_pair("lissandrar", new ZEliseAvoider("lissandrar", Targeted, "Lissandra")));
    avoidList.insert(std::make_pair("ufslash", new ZEliseAvoider("ufslash", SkillshotCircle, "Malphite")));
    avoidList.insert(std::make_pair("monkeykingspintowin", new ZEliseAvoider("monkeykingspintowin", SelfCast, "MonkeyKing")));
    avoidList.insert(std::make_pair("rivenizunablade", new ZEliseAvoider("rivenizunablade", SkillshotLine, "Riven")));
    avoidList.insert(std::make_pair("sejuaniglacialprisoncast", new ZEliseAvoider("sejuaniglacialprisoncast", SkillshotLine, "Sejuani")));
    avoidList.insert(std::make_pair("shyvanatrasformcast", new ZEliseAvoider("shyvanatrasformcast", SkillshotLine, "Shyvana")));
    avoidList.insert(std::make_pair("sonar", new ZEliseAvoider("sonar", SkillshotLine, "Sona")));
    avoidList.insert(std::make_pair("syndrar", new ZEliseAvoider("syndrar", Targeted, "Syndra")));
    avoidList.insert(std::make_pair("varusr", new ZEliseAvoider("varusr", SkillshotLine, "Varus")));
    avoidList.insert(std::make_pair("veigarprimordialburst", new ZEliseAvoider("veigarprimordialburst", Targeted, "Veigar")));
    avoidList.insert(std::make_pair("viktorchaosstorm", new ZEliseAvoider("viktorchaosstorm", SkillshotCircle, "Viktor")));
    ZElise::AvoidList = avoidList; }
