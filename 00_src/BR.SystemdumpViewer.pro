#-------------------------------------------------
#
# Project created by QtCreator 2014-02-26T16:05:20
#
#-------------------------------------------------

QT       += core gui network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BR.SystemdumpViewer
TEMPLATE = app

SOURCES += main.cpp\
    cL_clickedLabel.cpp \
    main_displayValues.cpp \
    main_mapXml.cpp \
    main_readXml.cpp \
    main_resetfunctions.cpp \
    main_whatsNew.cpp \
    mainwindow.cpp \
    optwindow.cpp \
    opt_slots.cpp \
    opt_functions.cpp \
    main_initFunctions.cpp \
    reportsettings.cpp

RESOURCES += \
            images.qrc


HEADERS  += mainwindow.h \
    systemdump.h \
    includes/rapidxml_utils.hpp \
    includes/clickLabel.h \
    includes/SimpleIni.h \
    optionwindow.h \
    version.h \
    reportsettings.h

TRANSLATIONS = lang/lang_bg.ts \
               lang/lang_cs.ts \
               lang/lang_da.ts \
               lang/lang_de.ts \
               lang/lang_el.ts \
               lang/lang_en.ts \
               lang/lang_es.ts \
               lang/lang_et.ts \
               lang/lang_fi.ts \
               lang/lang_fr.ts \
               lang/lang_he.ts \
               lang/lang_hr.ts \
               lang/lang_hu.ts \
               lang/lang_hy.ts \
               lang/lang_is.ts \
               lang/lang_it.ts \
               lang/lang_ja.ts \
               lang/lang_ko.ts \
               lang/lang_lt.ts \
               lang/lang_lv.ts \
               lang/lang_nl.ts \
               lang/lang_pl.ts \
               lang/lang_pt.ts \
               lang/lang_ro.ts \
               lang/lang_ru.ts \
               lang/lang_sk.ts \
               lang/lang_sl.ts \
               lang/lang_sr.ts \
               lang/lang_sv.ts \
               lang/lang_uk.ts \
               lang/lang_zh.ts


INCLUDEPATH += ../../../../../PortableApps/Qt/qwt-6.1.3_mingw49_32/include

FORMS    += mainwindow.ui \
            optionwindow.ui \
    reportsettings.ui

RC_FILE = BR.SystemdumpViewer.rc

CONFIG += qwt \
          c++11

LIBS += -lws2_32
LIBS += -lpsapi
LIBS += ../../../../../PortableApps/Qt/qwt-6.1.3_mingw49_32/lib/libqwt.a

#-------------------------------------------------
# Versioning
#-------------------------------------------------
MAJOR = 2
MINOR = 1
VERSION_HEADER = ../BR.SystemdumpViewer/cpp/version.h

versiontarget.target = $$VERSION_HEADER
#versiontarget.commands = ../BR.SystemdumpViewer/cpp/version.exe $$MAJOR $$MINOR $$VERSION_HEADER
versiontarget.depends = FORCE

PRE_TARGETDEPS += $$VERSION_HEADER
QMAKE_EXTRA_TARGETS += versiontarget

OTHER_FILES +=
