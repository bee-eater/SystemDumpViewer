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

TRANSLATIONS = lang/lang_de.ts \
               lang/lang_en.ts \
               lang/lang_fr.ts \
               lang/lang_pl.ts \
               lang/lang_ru.ts

FORMS    += mainwindow.ui \
            optionwindow.ui \
    reportsettings.ui

RC_FILE = BR.SystemdumpViewer.rc

CONFIG += qwt \
          c++11

INCLUDEPATH += "$$_PRO_FILE_PWD_/libs/qwt/include"

LIBS += -lws2_32 \
        -lpsapi \
        "$$PWD/libs/qwt/libqwt.a"

#-------------------------------------------------
# Versioning
#-------------------------------------------------
MAJOR = 2
MINOR = 1
#VERSION_HEADER = ../BR.SystemdumpViewer/cpp/version.h

versiontarget.target = $$VERSION_HEADER
#versiontarget.commands = ../BR.SystemdumpViewer/cpp/version.exe $$MAJOR $$MINOR $$VERSION_HEADER
versiontarget.depends = FORCE

PRE_TARGETDEPS += $$VERSION_HEADER
QMAKE_EXTRA_TARGETS += versiontarget

