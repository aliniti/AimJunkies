#include "ZCamille.h"
#include "ZCamilleAvoider.h"

void ZCamilleAvoider::GenerateEvadeList() {
    auto evadeList = ZCamille::DangerSpells;
    evadeList.insert(std::make_pair("infernalguardian", new ZCamilleAvoider("infernalguardian", SkillshotCircle, "Annie")));
    evadeList.insert(std::make_pair("curseofthesadmummy", new ZCamilleAvoider("curseofthesadmummy", SelfCast, "Amumu")));
    evadeList.insert(std::make_pair("enchantedcystalarrow", new ZCamilleAvoider("enchantedcystalarrow", SkillshotLine, "Ashe")));
    evadeList.insert(std::make_pair("aurelionsolr", new ZCamilleAvoider("aurelionsolr", SkillshotLine, "AurelionSol")));
    evadeList.insert(std::make_pair("azirr", new ZCamilleAvoider("azirr", SkillshotCircle, "Azir")));
    evadeList.insert(std::make_pair("cassiopeiar", new ZCamilleAvoider("cassiopeiar", SkillshotCircle, "Cassiopeia")));
    evadeList.insert(std::make_pair("feast", new ZCamilleAvoider("feast", Targeted, "Chogath")));
    evadeList.insert(std::make_pair("dariusexecute", new ZCamilleAvoider("dariusexecute", Targeted, "Darius")));
    evadeList.insert(std::make_pair("evelynnr", new ZCamilleAvoider("evelynnr", SkillshotCircle, "Evelynn")));
    evadeList.insert(std::make_pair("galioidolofdurand", new ZCamilleAvoider("galioidolofdurand", SelfCast, "Galio")));
    evadeList.insert(std::make_pair("gnarult", new ZCamilleAvoider("gnarult", SelfCast, "Gnar")));
    evadeList.insert(std::make_pair("garenr", new ZCamilleAvoider("egarenr", Targeted, "Garen")));
    evadeList.insert(std::make_pair("gravesr", new ZCamilleAvoider("gravesr", SkillshotLine, "Graves")));
    evadeList.insert(std::make_pair("hecarimult", new ZCamilleAvoider("hecarimult", SkillshotLine, "Hecarim")));
    evadeList.insert(std::make_pair("illaoir", new ZCamilleAvoider("illaoir", SelfCast, "Illaoi")));
    evadeList.insert(std::make_pair("jarvanivcataclysm", new ZCamilleAvoider("jarvanivcataclysm", Targeted, "JarvanIV")));
    evadeList.insert(std::make_pair("blindmonkrkick", new ZCamilleAvoider("blindmonkrkick", Targeted, "LeeSin")));
    evadeList.insert(std::make_pair("lissandrar", new ZCamilleAvoider("lissandrar", Targeted, "Lissandra")));
    evadeList.insert(std::make_pair("ufslash", new ZCamilleAvoider("ufslash", SkillshotCircle, "Malphite")));
    evadeList.insert(std::make_pair("monkeykingspintowin", new ZCamilleAvoider("monkeykingspintowin", SelfCast, "MonkeyKing")));
    evadeList.insert(std::make_pair("rivenizunablade", new ZCamilleAvoider("rivenizunablade", SkillshotLine, "Riven")));
    evadeList.insert(std::make_pair("sejuaniglacialprisoncast", new ZCamilleAvoider("sejuaniglacialprisoncast", SkillshotLine, "Sejuani")));
    evadeList.insert(std::make_pair("shyvanatrasformcast", new ZCamilleAvoider("shyvanatrasformcast", SkillshotLine, "Shyvana")));
    evadeList.insert(std::make_pair("sonar", new ZCamilleAvoider("sonar", SkillshotLine, "Sona")));
    evadeList.insert(std::make_pair("syndrar", new ZCamilleAvoider("syndrar", Targeted, "Syndra")));
    evadeList.insert(std::make_pair("varusr", new ZCamilleAvoider("varusr", SkillshotLine, "Varus")));
    evadeList.insert(std::make_pair("veigarprimordialburst", new ZCamilleAvoider("veigarprimordialburst", Targeted, "Veigar")));
    evadeList.insert(std::make_pair("viktorchaosstorm", new ZCamilleAvoider("viktorchaosstorm", SkillshotCircle, "Viktor"))); }

