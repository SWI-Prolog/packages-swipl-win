#--------------------------------------------------
# swipl-win.pro: SWI-Prolog / QT interface
#--------------------------------------------------
#
# REPL in QTextEdit on a background logic processor
#--------------------------------------------------
# Ing. Capelli Carlo - Brescia 2013

#-------------------------------------------------
# Project created by QtCreator 2013-07-10T10:26:52
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

macx {
    CONFIG += c++11
    ICON = swipl.icns
}

TARGET = swipl-win
TEMPLATE = app

# please, not obsolete compiler
QMAKE_CXXFLAGS += -std=c++0x

# provide appropriate linking mode for
# static compilation of pqConsole source files
DEFINES += PQCONSOLE_STATIC

# remove old defines in foreign predicates params
# due to conflicting Qt5 template arguments
DEFINES += PL_SAFE_ARG_MACROS

# QPlainTextEdit vs QTextEdit : faster lightweight format
# TBD evaluate timing difference
DEFINES += PQCONSOLE_NO_HTML

unix {
    # because SWI-Prolog is built from source
    CONFIG += link_pkgconfig
    PKGCONFIG += swipl
}

windows {
    SwiPl = "C:\Program Files\pl"
    INCLUDEPATH += $$SwiPl\include
    LIBS += -L$$SwiPl\bin -lswipl
}

mingw {
    SWIPL_BASE  = "../../"
    INCLUDEPATH += $$SWIPL_BASE/include ../cpp
    LIBS += -L$$SWIPL_BASE/lib/x64-win64 -lswipl
}

SOURCES += main.cpp \
    ../pqConsole/SwiPrologEngine.cpp \
    ../pqConsole/Swipl_IO.cpp \
    ../pqConsole/Preferences.cpp \
    ../pqConsole/pqMainWindow.cpp \
    ../pqConsole/pqConsole.cpp \
    ../pqConsole/FlushOutputEvents.cpp \
    ../pqConsole/ConsoleEdit.cpp \
    ../pqConsole/Completion.cpp \
    swipl_win.cpp

INCLUDEPATH += $$PWD/../pqConsole

RESOURCES += \
    swipl-win.qrc

OTHER_FILES += \
    README.md \
    ../pqConsole/swipl.png \
    MINGW.md

HEADERS += \
    ../pqConsole/SwiPrologEngine.h \
    ../pqConsole/Swipl_IO.h \
    ../pqConsole/Preferences.h \
    ../pqConsole/PREDICATE.h \
    ../pqConsole/pqMainWindow.h \
    ../pqConsole/pqConsole_global.h \
    ../pqConsole/pqConsole.h \
    ../pqConsole/FlushOutputEvents.h \
    ../pqConsole/do_events.h \
    ../pqConsole/ConsoleEdit.h \
    ../pqConsole/Completion.h \
    swipl_win.h
