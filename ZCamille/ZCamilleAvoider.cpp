#include "ZCamille.h"
#include "ZCamilleAvoider.h"

void ZCamilleAvoider::GenerateAvoidList() {
    std::map<std::string, ZCamilleAvoider *> avoidList;
    avoidList.insert(std::make_pair("infernalguardian", new ZCamilleAvoider("infernalguardian", SkillshotCircle, "Annie")));
    avoidList.insert(std::make_pair("curseofthesadmummy", new ZCamilleAvoider("curseofthesadmummy", SelfCast, "Amumu")));
    avoidList.insert(std::make_pair("enchantedcystalarrow", new ZCamilleAvoider("enchantedcystalarrow", SkillshotLine, "Ashe")));
    avoidList.insert(std::make_pair("aurelionsolr", new ZCamilleAvoider("aurelionsolr", SkillshotLine, "AurelionSol")));
    avoidList.insert(std::make_pair("azirr", new ZCamilleAvoider("azirr", SkillshotCircle, "Azir")));
    avoidList.insert(std::make_pair("cassiopeiar", new ZCamilleAvoider("cassiopeiar", SkillshotCircle, "Cassiopeia")));
    avoidList.insert(std::make_pair("feast", new ZCamilleAvoider("feast", Targeted, "Chogath")));
    avoidList.insert(std::make_pair("dariusexecute", new ZCamilleAvoider("dariusexecute", Targeted, "Darius")));
    avoidList.insert(std::make_pair("evelynnr", new ZCamilleAvoider("evelynnr", SkillshotCircle, "Evelynn")));
    avoidList.insert(std::make_pair("galioidolofdurand", new ZCamilleAvoider("galioidolofdurand", SelfCast, "Galio")));
    avoidList.insert(std::make_pair("gnarult", new ZCamilleAvoider("gnarult", SelfCast, "Gnar")));
    avoidList.insert(std::make_pair("garenr", new ZCamilleAvoider("egarenr", Targeted, "Garen")));
    avoidList.insert(std::make_pair("gravesr", new ZCamilleAvoider("gravesr", SkillshotLine, "Graves")));
    avoidList.insert(std::make_pair("hecarimult", new ZCamilleAvoider("hecarimult", SkillshotLine, "Hecarim")));
    avoidList.insert(std::make_pair("illaoir", new ZCamilleAvoider("illaoir", SelfCast, "Illaoi")));
    avoidList.insert(std::make_pair("jarvanivcataclysm", new ZCamilleAvoider("jarvanivcataclysm", Targeted, "JarvanIV")));
    avoidList.insert(std::make_pair("blindmonkrkick", new ZCamilleAvoider("blindmonkrkick", Targeted, "LeeSin")));
    avoidList.insert(std::make_pair("lissandrar", new ZCamilleAvoider("lissandrar", Targeted, "Lissandra")));
    avoidList.insert(std::make_pair("ufslash", new ZCamilleAvoider("ufslash", SkillshotCircle, "Malphite")));
    avoidList.insert(std::make_pair("monkeykingspintowin", new ZCamilleAvoider("monkeykingspintowin", SelfCast, "MonkeyKing")));
    avoidList.insert(std::make_pair("rivenizunablade", new ZCamilleAvoider("rivenizunablade", SkillshotLine, "Riven")));
    avoidList.insert(std::make_pair("sejuaniglacialprisoncast", new ZCamilleAvoider("sejuaniglacialprisoncast", SkillshotLine, "Sejuani")));
    avoidList.insert(std::make_pair("shyvanatrasformcast", new ZCamilleAvoider("shyvanatrasformcast", SkillshotLine, "Shyvana")));
    avoidList.insert(std::make_pair("sonar", new ZCamilleAvoider("sonar", SkillshotLine, "Sona")));
    avoidList.insert(std::make_pair("syndrar", new ZCamilleAvoider("syndrar", Targeted, "Syndra")));
    avoidList.insert(std::make_pair("varusr", new ZCamilleAvoider("varusr", SkillshotLine, "Varus")));
    avoidList.insert(std::make_pair("veigarprimordialburst", new ZCamilleAvoider("veigarprimordialburst", Targeted, "Veigar")));
    avoidList.insert(std::make_pair("viktorchaosstorm", new ZCamilleAvoider("viktorchaosstorm", SkillshotCircle, "Viktor")));
    ZCamille::AvoidList = avoidList; }

