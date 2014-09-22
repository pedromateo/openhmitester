# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------
QT += network \
    xml
CONFIG += qtestlib \
    debug
TARGET = LibPreload
TEMPLATE = lib
DEFINES += LIBPRELOAD_LIBRARY

INCLUDEPATH += ../hmi_tester

SOURCES += preloadcontroller.cpp \
    eventconsumer.cpp \
    ./Qt_Imp/qteventconsumer.cpp \
    ./Qt_Imp/qteventexecutor.cpp \
    preloadingcontrol.cpp \
    ./Qt_Imp/qtx11preloadingcontrol.cpp \
    ./Qt_Imp/qtownevents.cpp \
    ./Qt_Imp/qwidgetutils.cpp \
    ./Qt_Imp/qwidgetadapter.cpp
HEADERS += preloadcontroller.h \
    LibPreload_global.h \
    eventconsumer.h \
    ./Qt_Imp/qteventconsumer.h \
    eventexecutor.h \
    ./Qt_Imp/qteventexecutor.h \
    preloadingcontrol.h \
    ./Qt_Imp/qtx11preloadingcontrol.h \
    ./Qt_Imp/qtownevents.h \
    ./Qt_Imp/qwidgetutils.h \
    ./Qt_Imp/qwidgetadapter.h
LIBS += -lboost_serialization \
    -lX11
