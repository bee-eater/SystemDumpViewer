#include "optionwindow.h"
#include "ui_optionwindow.h"

void OptionWindow::init_maps(){

    // Fill QMap with entries for dropdown list
    map_lf2str["lang_bg.qm"] = "Bulgarian";
    map_lf2str["lang_cs.qm"] = "Czech";
    map_lf2str["lang_da.qm"] = "Danish";
    map_lf2str["lang_de.qm"] = "German";
    map_lf2str["lang_el.qm"] = "Modern Greek";
    map_lf2str["lang_en.qm"] = "English";
    map_lf2str["lang_es.qm"] = "Spanish";
    map_lf2str["lang_et.qm"] = "Estonian";
    map_lf2str["lang_fi.qm"] = "Finnish";
    map_lf2str["lang_fr.qm"] = "French";
    map_lf2str["lang_he.qm"] = "Hebrew";
    map_lf2str["lang_hr.qm"] = "Croatian";
    map_lf2str["lang_hu.qm"] = "Hungarian";
    map_lf2str["lang_hy.qm"] = "Armenian";
    map_lf2str["lang_is.qm"] = "Icelandic";
    map_lf2str["lang_it.qm"] = "Italian";
    map_lf2str["lang_ja.qm"] = "Japanese";
    map_lf2str["lang_ko.qm"] = "Korean";
    map_lf2str["lang_lt.qm"] = "Lithuanian";
    map_lf2str["lang_lv.qm"] = "Latvian";
    map_lf2str["lang_nl.qm"] = "Dutch";
    map_lf2str["lang_pl.qm"] = "Polish";
    map_lf2str["lang_pt.qm"] = "Portuguese";
    map_lf2str["lang_ro.qm"] = "Romanian";
    map_lf2str["lang_ru.qm"] = "Russian";
    map_lf2str["lang_sk.qm"] = "Slovak";
    map_lf2str["lang_sl.qm"] = "Slovenian";
    map_lf2str["lang_sr.qm"] = "Serbian";
    map_lf2str["lang_sv.qm"] = "Swedish";
    map_lf2str["lang_uk.qm"] = "Ukrainian";
    map_lf2str["lang_zh.qm"] = "Chinese";

    // Fill QMap for backward direction
    map_str2lf["Bulgarian"] = "lang_bg.qm";
    map_str2lf["Czech"] = "lang_cs.qm";
    map_str2lf["Danish"] = "lang_da.qm";
    map_str2lf["German"] = "lang_de.qm";
    map_str2lf["Modern Greek"]= "lang_el.qm";
    map_str2lf["English"] = "lang_en.qm";
    map_str2lf["Spanish"] = "lang_es.qm";
    map_str2lf["Estonian"] = "lang_et.qm";
    map_str2lf["Finnish"] = "lang_fi.qm";
    map_str2lf["French"] = "lang_fr.qm";
    map_str2lf["Hebrew"] = "lang_he.qm";
    map_str2lf["Croatian"] = "lang_hr.qm";
    map_str2lf["Hungarian"] = "lang_hu.qm";
    map_str2lf["Armenian"] = "lang_hy.qm";
    map_str2lf["Icelandic"] = "lang_is.qm";
    map_str2lf["Italian"] = "lang_it.qm";
    map_str2lf["Japanese"] = "lang_ja.qm";
    map_str2lf["Korean"] = "lang_ko.qm";
    map_str2lf["Latvian"] = "lang_lv.qm";
    map_str2lf["Lithuanian"] = "lang_lt.qm";
    map_str2lf["Dutch"] = "lang_nl.qm";
    map_str2lf["Polish"] = "lang_pl.qm";
    map_str2lf["Portuguese"] = "lang_pt.qm";
    map_str2lf["Romanian"] = "lang_ro.qm";
    map_str2lf["Russian"] = "lang_ru.qm";
    map_str2lf["Slovak"] = "lang_sk.qm";
    map_str2lf["Slovenian"] = "lang_sl.qm";
    map_str2lf["Serbian"] = "lang_sr.qm";
    map_str2lf["Swedish"] = "lang_sv.qm";
    map_str2lf["Ukrainian"] = "lang_uk.qm";
    map_str2lf["Chinese"] = "lang_zh.qm";

}
